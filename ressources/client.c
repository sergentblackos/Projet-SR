#include "stdio.h"
#include "stdlib.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <regex.h>


// une methode qui affiche un cadre pour le menu du terminal
void cadre()
{
    printf("----------------------------------------------------------------------------------\n");
}


// Cette méthode renvoie une heure sous forme d'une chaîne de caractères "HH:MM"
void regexheure(char heure[6])
{
    // Stocke l'heure saisie
    do
    {
        printf("Saisir l'heure de départ : (format hh:mm) ");
        scanf("%5s", heure);

        // Vérifie que l'heure respecte le format hh:mm
        if (sscanf(heure, "%2d:%2d", &(int){0}, &(int){0}) != 2)
        {
            printf("Format d'heure incorrect. Veuillez ressayer. \n");
        }
        else
        {
            // Heures et minutes sous forme d'entiers
            int heures, minutes;

            // Conversion des heures et minutes
            sscanf(heure, "%d:%d", &heures, &minutes);

            // Vérifie que l'horaire est cohérent
            if (heures < 0 || heures > 23 || minutes < 0 || minutes > 59)
            {
                printf("Heure ou minutes invalides. Veuillez réessayer. \n");
            }
            else
            {
                // Sort de la boucle si l'heure est valide
                break;
            }
        }
    } while (1);
}


// Cette méthode renvoie 0 si le mot que l'utilisateur entre n'est pas un nom de ville
int regexVille(char nomVille[256])
{
    // Vérifier si le mot commence par une majuscule
    if (!isupper(nomVille[0]))
    {
        // Le mot ne commence pas par une majuscule, donc ce n'est pas un nom de ville
        return 0;
    }

    // Parcourir les caractères du mot
    for (int i = 1; nomVille[i] != '\0'; i++)
    {
        // Vérifier si chaque caractère est alphabétique
        if (!isalpha(nomVille[i]))
        {
            return 0; // Le mot contient des caractères non vérifié
        }
    }

    // Le mot est un nom de ville valide
    return 1;
}


// Récupère la chaîne de caractères envoyée par le serveur et la découpe en éléments du train pour les afficher
void formatResultat(char chaine[256], char villeDepart[256], char villeArrivee[256], char *messagePerso)
{
    // Utiliser strtok pour découper la chaîne de caractères avec des points-virgules pour récupérer les éléments et les stocker dans des variables
    if (strncmp(chaine, "*", 1) == 0)
    {
        // Si la chaîne commence par "*", afficher le message personnalisé suivant la requete
        printf("%s", messagePerso);
    }
    else
    {
        char *formats = strtok(chaine, ";");
        char *numero = formats;
        formats = strtok(NULL, ";");
        char *heureDep = formats;
        formats = strtok(NULL, ";");
        char *heureArv = formats;
        formats = strtok(NULL, ";");
        char *prix = formats;

        // Afficher les détails du train formatés
        printf("\nNumero : %s "
               "  Ville de départ : %s "
               "  Ville d'arrivée : %s "
               "  Heure de départ : %s "
               "  Heure d'arrivée : %s "
               "  Prix : %s euros \n",
               numero, villeDepart, villeArrivee, heureDep, heureArv, prix);
    }
}


// Récupère une chaîne de caractères contenant plusieurs lignes de trains au format struct.
// Ensuite, pour chaque ligne, appelle la méthode formatResultat qui récupère les éléments de chaque train et affiche le résultat.
void formatResultatMultiple(char chaines[256], char villeDepart[256], char villeArrivee[256], char *messagePerso)
{
    char tableau[256][256];

    // Initialiser le tableau avec des chaînes vides
    for (int i = 0; i < 100; i++)
    {
        memset(tableau[i], 0, sizeof(tableau[i]));
    }

    // Utiliser strtok pour découper la chaîne en lignes individuelles et les stocker dans le tableau
    char *formatsMultiple = strtok(chaines, "\n");
    int i = 0;

    while (formatsMultiple != NULL)
    {
        strcpy(tableau[i], formatsMultiple);
        i++;
        formatsMultiple = strtok(NULL, "\n");
    }

    // Pour chaque ligne dans le tableau, appelle de la fonction formatResultat et affiche le résultat
    for (int j = 0; j < i; j++)
    {
        formatResultat(tableau[j], villeDepart, villeArrivee, messagePerso);
        cadre(); 
    }
}


// affichage du menu
int afficherMenu()
{
    char numero[40];
    cadre();
    printf("                                 Bonjour \n");
    cadre();
    printf("Menu :\n");
    printf("1. Recherche de trains par ville de départ et d'arrivée avec heure précise \n");
    printf("2. Recherche de trains par ville de départ et d'arrivée dans une plage horaires\n");
    printf("3. Recherche de trains par ville de départ et d'arrivée \n");
    printf("4. Quitter\n");
    cadre();
    printf("Veuillez choisir une option  : ");
    scanf("%s", numero);
    cadre();
    char num = numero[0];

    // Repeter jusqu'à ce que l'utilisateur rentre un numéro compris entre 1 et 4
    while (num != '1' && num != '2' && num != '3' && num != '4')
    {
        memset(numero, 0, 40);
        printf("Veuillez choisir un numéro compris entre 1 et 4 : \n");
        scanf("%s", numero);
        cadre();
        num = numero[0];
    }
    int n = (int)num - '0';
    return n;
}

// Execution argc = le port et et argv correspond au localhost ou adresse ou nom de la machine
int main(int argc, char **argv)
{
    // Controle des arguments
    if (argc != 3)
    {
        printf("Il manque des parametres");
        return -1;
    }

    // Nom du serveur
    char *host = argv[1];

    // Controle du serveur?

    // Initialisation de noPort en tant qu'entier que l'on rentre en param
    int noPort = atoi(argv[2]);
    // Vérifier si la conversion a réussi et si le port est dans la plage autorisée
    if (noPort <= 0 || noPort > 65535) {
        fprintf(stderr, "Le port doit être un entier compris entre 1 et 65535.\n");
        return EXIT_FAILURE;
    }

    

    // création de la socket
    int resSocket;
    resSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Gestion des erreurs de socket
    if (resSocket == -1)
    {
        perror("socket");
        exit(-1);
    }

    // création des types struct pour stocker les informations
    struct hostent *infMachine = malloc(sizeof(struct hostent));
    struct sockaddr_in adserv;

    // récupération des informations server dans infMachine
    infMachine = gethostbyname(host);

    adserv.sin_family = AF_INET;
    adserv.sin_port = htons(noPort);

    // copie de l'information h_addr de infMachine dans s_addr de adserv
    memcpy(&adserv.sin_addr.s_addr, infMachine->h_addr, sizeof(__u_long));

    // Connexion au serveur
    int valideConnect;
    valideConnect = connect(resSocket, (struct sockaddr *)&adserv, sizeof(struct sockaddr));

    // Gestion d'erreur de connexion
    if (valideConnect == -1)
    {
        perror("connect");
        exit(-1);
    }
    printf("Connexion établie\n");

    // Boucle d'écriture de messages (msg intialiser de maniere lambda)
    int tailleMessage;
    char message[256] = "bonjour";

    while (strcmp(message, "fin") != 0)
    {
        // affichage du menu
        int choix = afficherMenu();

        // envoi de la taille du choix
        write(resSocket, &choix, sizeof(int));

        // affichage du choix 
        printf("Vous avez choisi l'option :%d \n", choix);

        // si le choix = 4, alors l'utilisateur va quitter le menu donc on ferme le client
        if (choix == 4)
        {
            cadre();
            printf("Merci ,au revoir \n");
            break;
        }
        else
        {

            do
            {
                // On demande la ville de départ à  l'utilisateur
                printf("Veuillez indiquer votre ville de départ :\n");

                // Faire un scan et vérifier si c'est conforme au nom de ville
                scanf("%s", message);

                if (!regexVille(message))
                {
                    printf("Veuillez indiquer votre ville avec une majuscule au début et contenant uniquement des lettres vérifiées en minuscule :\n");
                }

            } while (!regexVille(message));

            // à ce stade, le message est conforme au nom de ville
            printf("Ville de départ enregistrée : %s\n", message);

            tailleMessage = strlen(message);
            // envoi de la taille du message dans la socket
            write(resSocket, &tailleMessage, sizeof(int));
            // envoi du message dans la socket
            write(resSocket, message, tailleMessage);

            // stocker la ville départ dans une variable pour l'utiliser apres dans le format a afficher pour le client
          char villeDep[256];
            strcpy(villeDep, message);
            //
            cadre();

            // on demande la ville d'arriver à l'utilisateur
            do
            {
                // On demande la ville de départ à l'utilisateur
                printf("Veuillez indiquer votre ville d'arrivée :\n");

                // Faire un scan et vérifier si c'est conforme au nom de ville
                scanf("%s", message);

                if (!regexVille(message))
                {
                    printf("Veuillez indiquer votre ville avec une majuscule au début et contenant uniquement des lettres alphabétiques en minuscule :\n");
                }

            } while (!regexVille(message));

            // à ce stade, le message est conforme au nom de ville
            printf("Ville d'arrivée enregistré : %s\n", message);
            tailleMessage = strlen(message);
            // envoi de la taille du message dans la socket
            write(resSocket, &tailleMessage, sizeof(int));
            // envoi du message dans la socket
            write(resSocket, message, tailleMessage);

            // stocker la ville d'arriver dans une variable pour l'utiliser apres dans le format à afficher pour le client
            char villeArv[256];
            strcpy(villeArv, message);
            //
            cadre();

            // suivant le choix renseignàƒÂ© par l'utilisateur nous lui proposons de demander des différentees options
            switch (choix)
            {
                // nous souhaitons seulement afficher le prochain train ayant pour horaire de départ
                //  une horaire >= à  l'horaire renseigner par l'utilisateur
                //  nous envoyons juste l'heure au serveur
                //  ce dernier nous renverra le numéro du train, son heure de départ, son heure d'arrivée, et son prix
            case 1:
                printf("Veuillez indiquer l'heure exacte à laquelle vous souhaitez partir :  \n");
                
                // envoi de l'horaire
                regexheure(message) ; 
                // calcul de la taille du message
                tailleMessage = strlen(message);
                // envoi de la taille du message dans la socket
                 write(resSocket, &tailleMessage, sizeof(int));
                 // envoi du message dans la socket
                write(resSocket, message, tailleMessage);
                // réception des résultats fournis par le serveur sous un format donné
                int tailleReponse;
                read(resSocket, &tailleReponse, sizeof(int));
                // lecture du message qui contitent la ville d'arrivée
                read(resSocket, message, tailleReponse);
                // décodage du format et insertion dans une structure
                // return le resultat à l'utilisateur
                formatResultat(message, villeDep, villeArv, "\nNous sommes désolés, il n'y a plus de trains disponible à ce jour.\n");
                break;
                // nous souhaitons afficher les trains dans une plage horaire renseigné par l'utilisateur
                // l'utilisateur donne 2 horaires
                // les horaires des trains affichés devront etre compris en l'heure minimale et l'heure maximale de départ
                // le serveur renverra les numéros, les heures de départ, les heures d'arrivée, puis les prix de chaque trains
            case 2:

                // nous demandons à l'utilisateur l'heure minimale de départ
                printf("Veuillez indiquer la première heure à laquelle vous souhaitez partir :  \n");

                // nous envoyons l'heure minimale au serveur
                regexheure(message) ; 
                // calcul de la taille du message
                tailleMessage = strlen(message);
                // envoi de la taille du message dans la socket
                 write(resSocket, &tailleMessage, sizeof(int));
                 // envoi du message dans la socket
                write(resSocket, message, tailleMessage);

                // nous demandons à l'utilisateur l'heure maximale de départ
                printf("Veuillez indiquer l'heure maximale à laquelle vous souhaitez partir :  \n");

                // nous envoyons l'heure maximale au serveur
                regexheure(message) ; 
                // calcul de la taille du message
                tailleMessage = strlen(message);
                // envoi de la taille du message dans la socket
                 write(resSocket, &tailleMessage, sizeof(int));
                 // envoi du message dans la socket
                write(resSocket, message, tailleMessage);

                // réception des résultats fournis par le serveur sous un format donné
                int tailleReponse1;
                read(resSocket, &tailleReponse1, sizeof(int));
                // lecture du message qui contitent la ville d'arrivée
                read(resSocket, message, tailleReponse1);

                // affichage des résultat à l'utilisateur
                printf("Voici les trains possibles pour la tranche d'horaire : \n");
                cadre();

                formatResultatMultiple(message, villeDep, villeArv,"Nous sommes désolés, il n'y pas de train disponible dans ces tranches horaires.\n");

                break;

                // nous souhaitons afficher tous les trains dans un ordre, choisi par l'utilisateur, ayant
                // la meme gare de départ et d'arrivée
                // l'utilisateur peut choisir d'afficher les trains ayant les prix du moins élevé au plus élevé ou
                // d'afficher les trains ayant les heures de trajets des moins importantes au plus importantes

            case 3:
                int tailleReponse2;
                read(resSocket, &tailleReponse2, sizeof(int));
                // lecture du message qui contitent la ville d'arrivée
                read(resSocket, message, tailleReponse2);
                formatResultatMultiple(message, villeDep, villeArv,"Nous sommes désolés, il n'y pas de train disponible pour ce trajet\n");
                // nous demandons l'ordre d'affichage à l'utilisateur
                int trajet;
                printf("\n\nVeuillez choisir une option : \n");
                printf("\n1. Recherche le train au meilleur prix   \n");
                printf("2. Recherche le train avec le trajet de durée optimal \n");
                scanf("%d", &trajet);

                // gestion de l'erreur si l'utilisateur ne renseigne pas la bonne option
                while (trajet != 1 && trajet != 2)
                {
                    printf("Veuillez choisir un numéro compris entre 1 ou 2 : \n");
                    scanf("%d", &trajet);
                    cadre();
                }
                write(resSocket, &trajet, sizeof(int));
                switch (trajet)
                {
                case 1:
                    printf("Vous avez choisi l'option :%d \n", trajet);
                    int tailleReponse2;
                    read(resSocket, &tailleReponse2, sizeof(int));
                    // lecture du message qui contitent la ville d'arrivée
                    read(resSocket, message, tailleReponse2);

                    formatResultat(message, villeDep, villeArv," ");

                    break;
                case 2:
                    printf("Vous avez choisi l'option :%d \n", trajet);

                    int tailleReponse3;
                    read(resSocket, &tailleReponse3, sizeof(int));
                    // lecture du message qui contitent la ville d'arrivée
                    read(resSocket, message, tailleReponse3);
                    formatResultat(message, villeDep, villeArv, " ");

                    break;
                }

            default:
                break;
            }
        }
    }
    // fermeture du socket
    close(resSocket);
    return 0;
}
