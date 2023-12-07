#include "stdio.h"
#include "stdlib.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

// creer une methode  qui scan et qui write au meme temps
void scanwrite(int resSocket, char *message)
{
    scanf("%s", message);
    // calcul de la taille du message
    int tailleMessage = strlen(message);
    // envoi de la taille du message dans la socket
    write(resSocket, &tailleMessage, sizeof(int));
    // envoi du message dans la socket
    write(resSocket, message, tailleMessage);
}
// une methode qui affiche un cadre pour le menu du terminal
void cadre()
{
    printf("----------------------------------------------------------------------------------\n");
}

// Recupere la chaine de caractere envoyer par le serveur et la decouper en element du train pour recuperer les  afin de les afficher
void formatResultat(char chaine[256], char villeDepart[256], char villeArrivee[256]) {
        // utiliser strtok pour decouper la chaine de caractere avec des points virgules pour recuperer les elements et les stocker dans des variable
if(strcmp(chaine, "0") == 0){
    printf("Aucun train ne correspond Ãƒ  vos critÃƒÂ¨res") ;
}else{
    char *formats = strtok(chaine, ";");
    char *numero = formats;
    formats = strtok(NULL, ";");
    char *heureDep = formats;
    formats = strtok(NULL, ";");
    char *heureArv = formats;
    formats = strtok(NULL, ";");
    char *prix = formats;
    // afficher  les variables
    printf("Numero:%s "
           "  ville de depart:%s "
           "  ville d'arriver: %s "
           "  Heure de dÃƒÂ©part: %s "
           "  Heure d'arrivÃƒÂ©e: %s "
           "  Prix: %s \n",
           numero,  villeDepart, villeArrivee, heureDep,heureArv, prix);
        }
}
        // recupere une chaine de caractere dedans  plusieurs ligne de train comme struct ,puis recuperer chaque ligne et faire appel a la methode 
// formatsresultat qui recupere les element de chaque train er afficher le resultats 
void formatResultatMultiple(char chaines[256], char villeDepart[256], char villeArrivee[256]) {
    char tableau[256][256];
    for (int i = 0; i < 100; i++) {
        memset(tableau[i], 0, sizeof(tableau[i]));
    } 
    char *formatsMultiple = strtok(chaines, "\n");
    int i = 0;
    
    while (formatsMultiple != NULL) {
        strcpy(tableau[i], formatsMultiple);
        i++;
        formatsMultiple = strtok(NULL, "\n");
    }
    for ( int j= 0; j < i ; j++)
    {
        formatResultat(tableau[j], villeDepart,  villeArrivee) ; 
        cadre() ; 

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
    printf("1. Recherche de trains par ville de dÃƒÂ©part et d'arrivÃƒÂ©e avec heure prÃƒÂ©cise \n");
    printf("2. Recherche de trains par ville de dÃƒÂ©part et d'arrivÃƒÂ©e dans une plage horaires\n");
    printf("3. Recherche de trains par ville de dÃƒÂ©part et d'arrivÃƒÂ©e \n");
    printf("4. Quitter\n");
    cadre();
    printf("Veuillez choisir une option  : ");
    scanf("%s", numero);
    cadre();
    char num = numero[0];

    // Repeter jusqu'Ãƒ  ce que l'utilisateur rentre un numÃƒÂ©ro compris entre 1 et 4
    while (num != '1' && num != '2' && num != '3' && num != '4')
    {
        memset(numero, 0, 40);
        printf("Veuillez choisir un numÃƒÂ©ro compris entre 1 et 4 : \n");
        scanf("%s", numero);
        cadre();
        num = numero[0];
    }
    int n = (int)num - '0';
    return n;
}

// Execution argc = le port et et argv correpond au localhost (localhost a ÃƒÂ©crire)
int main(int argc, char **argv)
{
    // Controle des arguments
    if (argc != 3)
    {
        printf("Il manque des parametres");
        return -1;
    }
    // Initialisation de noPort en tant qu'entier que l'on rentre en param
    int noPort = atoi(argv[1]);

    // Nom du serveur
    char *host = argv[2];

    // CrÃƒÂ©ation de la socket
    int resSocket;
    resSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Gestion des erreurs de socket
    if (resSocket == -1)
    {
        perror("socket");
        exit(-1);
    }

    // crÃƒÂ©ation des types struct pour stocker les informations
    struct hostent *infMachine = malloc(sizeof(struct hostent));
    struct sockaddr_in adserv;

    // rÃƒÂ©cupÃƒÂ©ration des informations server dans infMachine
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
        perror("Connect");
        exit(-1);
    }
    printf("Connexion ÃƒÂ©tablie\n");

    // Boucle d'ÃƒÂ©criture de messages (msg intialiser de maniere lambda)
    int tailleMessage;
    char message[256] = "bonjour";
    char *pointeurSurChaine = message;

    while (strcmp(message, "fin") != 0)
    {
        // affichage du menu
        int choix = afficherMenu();

        
        // envoi de la taille du choix
        write(resSocket, &choix, sizeof(int));

        // // conversion du choix en chaine de caractÃƒÂ¨re afin de pouvoir etre lu par le serveur
        // sprintf(message, "%d", choix);

        // // envoi du choix dans la socket
        // write(resSocket, message, choix);

        // affichage du choix dÃƒÂ©sirÃƒÂ©
        printf("Vous avez choisie l'option :%d \n", choix);

        // si le choix = 4, l'utilisateur dÃƒÂ©sire de quitter le menu donc on ferme le client
        if (choix == 4)
        {
            strcmp(message, "4") == 0;
            cadre();
            printf("Merci ,au revoir \n");
            break;
        }
        else
        {

            // on demande la ville de dÃƒÂ©part Ãƒ  l'utilisateur
            printf("Veuillez indiquer votre ville de depart :\n");
            scanwrite(resSocket, message);

            // stocker la ville de depart dans une variable pour l'utiliser apres dans le format a afficher pour le client
            char villeDep[256] ;
             strcpy(villeDep,message);
            //
            cadre();

            // on demande la ville de dÃƒÂ©part Ãƒ  l'utilisateur
            printf("Veuillez indiquer votre ville d'arrivee :\n");
            scanwrite(resSocket, message);
            // stocker la ville d'arriver dans une variable pour l'utiliser apres dans le format a afficher pour le client
            char villeArv[256] ;
            strcpy(villeArv,message);
            //
            cadre();

            // suivant le choix renseignÃƒÂ© par l'utilisateur nous lui proposons de demander des diffÃƒÂ¨rentes options
            switch (choix)
            {
                // nous souhaitons  seulement afficher le prochain train ayant pour horaire de dÃƒÂ©part
                //  une horaire >= Ãƒ  l'horaire renseignÃƒÂ©e par l'utilisateur
                //  nous envoyons juste l'heure au serveur
                //  ce dernier nous renverra le numÃƒÂ©ro du train, son heure de dÃƒÂ©part, son heure d'arrivÃƒÂ©e, et son prix
            case 1:
                printf("Veuillez indiquer l'heure exacte Ãƒ  laquelle vous souhaitez partir :  \n");
                // envoi de l'horaire
                scanwrite(resSocket, message);
                // rÃƒÂ©ception des rÃƒÂ©sultats fournis par le serveur sous un format donnÃƒÂ©
                int tailleReponse;
                read(resSocket, &tailleReponse, sizeof(int));
                // lecture du message qui contitent la ville d'arrivÃƒÂ©e
                read(resSocket, message, tailleReponse);
                // dÃƒÂ©codage du format et insertion dans une structure
                // return le resultat Ãƒ  l'utilisateur
                // !!!!! rayane !!! ca c'est un test pour voir que ma methode marche !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                formatResultat(message, villeDep, villeArv);
                break;
                // nous souhaitons afficher les trains dans une plage horaire reseignÃƒÂ©e par l'utilisateur
                // l'utilisateur donne 2 horaires
                // les horaires des trains affichÃƒÂ©s devront etre compris en l'heure minimale et l'heure maximale de dÃƒÂ©part
                // le serveur renverra les numÃƒÂ©ros, les heures de dÃƒÂ©part, les heures d'arrivÃƒÂ©s, puis les prix de chaque trains
            case 2:

                // nous demandons Ãƒ  l'utilisateur l'heure minimale de dÃƒÂ©part
                printf("Veuillez indiquer la premiÃƒÂ¨re heure  Ãƒ  laquelle vous souhaitez partir :  \n");

                // nous envoyons l'heure minimale au serveur
                scanwrite(resSocket, message);

                // nous demandons Ãƒ  l'utilisateur l'heure maximale de dÃƒÂ©part
                printf("Veuillez indiquer l'heure maximale Ãƒ  laquelle vous souhaitez partir :  \n");

                // nous envoyons l'heure maximale au serveur
                scanwrite(resSocket, message);

                // rÃƒÂ©ception des rÃƒÂ©sultats fournis par le serveur sous un format donnÃƒÂ©
                int tailleReponse1;
                read(resSocket, &tailleReponse1, sizeof(int));
                // lecture du message qui contitent la ville d'arrivÃƒÂ©e
                read(resSocket, message, tailleReponse1);
                // dÃƒÂ©codage du format et insertion dans une structure

                // affichage des rÃƒÂ©sulats Ãƒ  l'utilisateur
                printf("Voici les trains possibles pour la tranche d'horaire : \n");
                cadre();

                formatResultatMultiple(message, villeDep, villeArv);

                break;

                // nous souhaitons afficher tous les trains dans un ordre, choisi par l'utilisateur, ayant
                // la meme gare de dÃƒÂ©part et d'arrivÃƒÂ©e
                // l'utilisateur peut choisir d'afficher les trains ayant les prix du moins ÃƒÂ©levÃƒÂ© au plus ÃƒÂ©levÃƒÂ© ou
                // d'afficher les trains ayant les heures de trajets des moins importantes au plus importantes

            case 3 :

                // // nous demandons l'ordre d'affichage Ãƒ  l'utilisateur
                // int trajet ;
                // printf("Veuillez choisir une option : ");
                // printf("1. Recherche le train au meilleure prix   \n");
                // printf("2. Recherche le train avec le trajet de durÃƒÂ©e optimal \n");
                // scanf("%d", &trajet);
            
                // // gestion de l'erreur si l'utilisateur ne renseigne pas la bonne option
                // while (trajet != 1 && trajet != 2)
                // {
                //     printf("Veuillez choisir un numÃƒÂ©ro compris entre 1 ou 2 : \n");
                //     scanf("%d", &trajet);
                //     cadre();
                // }
                // write(resSocket, &trajet, sizeof(int));
                // switch(trajet)
                // {
                //     case 1 :
                //         printf("Vous avez choisie l'option :%d \n", trajet);
                //         int tailleReponse2;
                // read(resSocket, &tailleReponse2, sizeof(int));
                // // lecture du message qui contitent la ville d'arrivÃƒÂ©e
                // read(resSocket, message, tailleReponse2);

                //         formatResultat(message, villeDep, villeArv);
                
                //         break;
                //     case 2 :
                //         printf("Vous avez choisie l'option :%d \n", trajet);

                //          int tailleReponse3;
                // read(resSocket, &tailleReponse3, sizeof(int));
                // // lecture du message qui contitent la ville d'arrivÃƒÂ©e
                // read(resSocket, message, tailleReponse3);
                //         formatResultat(message, villeDep, villeArv);
                

                        break;
                //}
                // envoi du choix au serveur

                // demande de la ville de dÃƒÂ©part Ãƒ  l'utilisateur

                // envoi de la ville de dÃƒÂ©part Ãƒ  l'utilisateur

                // demande de la ville d'arrivÃƒÂ©e Ãƒ  l'utilisateur

                // envoi de la ville d'arrivÃƒÂ©e Ãƒ  l'utilisateur

                // rÃƒÂ©ception des rÃƒÂ©sultats fournis par le rÃƒÂ©sultat serveur

                // dÃƒÂ©codage du format et insertion dans une structure

                // affichage des rÃƒÂ©sultats

            default:
                break;
            }
        }
    }
    // fermeture du socket
    close(resSocket);
    return 0;
}


/*
regler les erreurs d'entrer
creer 2 methode une pr bien ecrire les villes la deuxiemme pour les heures


*/