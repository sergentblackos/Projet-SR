#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include "parser.h"
#include "fonctionnalite.c"
#include "fonctionnalite.h"

// Fonction end_child sert la gestion des signaux SIGCHLD
void end_child()
{
    wait(NULL);
}

// Execution argc = noPort et argv = un fichier ou autre mot ?
int main(int argc, char **argv)
{ // Controle des arguments
    if (argc != 3)
    {
        printf("Il n'y a pas le nombre de paramètres adéquats.\n Il doit en contenir 3");
        return -1;
    }
    // Initialisation de noPort en tant qu'entier que l'on rentre en param
    int noPort = atoi(argv[1]);
    const char *nomFichier = argv[2];

    // création de la socket d'écoute du serveur.
    int socketEcoute = socket(AF_INET, SOCK_STREAM, 0);
    // Gestion d'erreur de la socket d'écoute
    if (socketEcoute == -1)
    {
        perror("socket");
        exit(-1);
    }

    // création de la structure infoEcoute et définition des variables de la structure
    struct sockaddr_in infoEcoute;
    infoEcoute.sin_family = AF_INET;                // protocole internet
    infoEcoute.sin_addr.s_addr = htonl(INADDR_ANY); // toutes les adresses IP de la machine
    infoEcoute.sin_port = htons(noPort);            // port d'écoute par-dessus les ports réservés

    // Attachement de la socket d'écoute en utilisant la structure définie dans infoEcoute
    int valideBind = bind(socketEcoute, (struct sockaddr *)&infoEcoute, sizeof(struct sockaddr));

    // Gestion d'erreur du bind de la socket
    if (valideBind == -1)
    {
        perror("bind");
        exit(-1);
    }

    // Ouverture de service sur la socket d'écoute
    int valideListen = listen(socketEcoute, 10);

    // Gestion d'erreur de la mise en écoute de la socket
    if (valideListen == -1)
    {
        perror("listen");
        exit(-1);
    }

    // création de la structure infoEcoute et définition des variables de la structure afin de
    // gérer la mort de chaque fils
    struct sigaction ac;
    ac.sa_handler = end_child;
    ac.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &ac, NULL);

    // création de la boucle infinie

    while (1)
    {
        // création de la structure infoClient pour stocker les données client
        unsigned int longueurInfoClient;
        struct sockaddr_in infoClient;

        // Acceptation d'une connexion sur la socket d'écoute,
        // récupération des informations sur le client dans struct infoClient
        // et création de la socket de service.
        printf("En attente de connexion...\n");
        int socketService = accept(socketEcoute, (struct sockaddr *)&infoClient, &longueurInfoClient);

        // Gestion d'erreur de la création de la socket de service
        if (socketService == -1)
        {
            perror("accept");
            exit(-1);
        }
        printf("Connexion établie\n");

        int continuer = 1;

        // création du processus fils chargé de satisfaire les requêtes du client.
        switch (fork())
        {
        case -1:
            perror("fork");
            exit(-1);

        case 0:
            // Comportement du processus fils

            // on ferme la socket d'écoute
            close(socketEcoute);

            // boucle de communication avec le client
            // dans le cas où le client nous envoie 1 nous devons réceptionner la ville de départ puis la ville d'arrivée
            // puis nous devons utiliser une fonction qui nous retourne le numéro du train, l'heure de départ, l'heure d'arrivée
            // et le prix
            // ensuite nous envoyons le résultat de la fonction au client
            while (continuer)
            {
                int tailleMessage = 1;

                // réception du choix
                read(socketService, &tailleMessage, sizeof(int));

                printf("Choix: %d\n", tailleMessage);
                // nous adaptons les fonctionnalités en fonction du choix envoyé par le client
                switch (tailleMessage)
                {
                case 1:
                {
                    int taille = 0;
                    char requete[256];

                    // lecture de la taille du prochain message  contenant la ville de départ
                    // et stockage de la taille du message dans la variable taille
                    read(socketService, &taille, sizeof(int));

                    // affichage de la taille du message
                    printf("Octets lus : %d\n", taille);

                    // déclaration d'une variable qui stocke la ville de départ
                    char villeDeDepart[taille];

                    // lecture du message qui contient la ville de départ
                    read(socketService, requete, taille);

                    // affichage du message lu
                    printf("Bien reçu : %s\n", requete);

                    // stockage de la ville de depart dans la variable villeDeDepart
                    strcpy(villeDeDepart, requete);
                    printf("Ville stockée : %s\n", villeDeDepart);

                    // fonction qui permet de mettre dans la zone memoire
                    memset(requete, 0, 256);

                    // lecture de la taille du prochain message contenant la ville d'arrivee
                    // et stockage de la taille du message dans la variable taille
                    int tailleVA;
                    read(socketService, &tailleVA, sizeof(int));

                    // affichage de la taille du message
                    printf("Octets lus : %d\n", tailleVA);

                    // declaration d'une variable qui stocke la ville de d'arrivee
                    char villeDArrivee[tailleVA];

                    // lecture du message qui contitent la ville d'arrivee
                    read(socketService, requete, tailleVA);
                    printf("Bien reçu : %s\n", requete);

                    // stockage de la ville d'arrivee dans la variable villeDArrivee
                    strcpy(villeDArrivee, requete);
                    printf("Ville stockée : %s\n", villeDArrivee);

                    // fonction qui permet de mettre dans  0 la zone memoire
                    memset(requete, 0, 256);

                    // lecture de la taille du prochain message contenant l'heure d'arrivee
                    // et stockage de la taille du message dans la variable taille
                    read(socketService, &taille, sizeof(int));

                    // affichage de la taille du message
                    printf("Octets lus : %d\n", taille);

                    // declaration d'une variable qui stocke l'heure de depart
                    char HeuredeDepart[taille];

                    // lecture du message qui contitent la l'heure de depart
                    read(socketService, requete, taille);
                    printf("Bien reçu: %s\n", requete);

                    // stockage de la ville d'arrivee dans la variable HeureDeDepart
                    strcpy(HeuredeDepart, requete);
                    printf("Ville de depart: %s , ville d'arrivée: %s, heure de depart, %s \n", villeDeDepart, villeDArrivee, HeuredeDepart);

                    // Utilisation de la fct et init de struct data
                    struct TrainData data = parseur(nomFichier);
                    char *reponse = rechercheTrainVilleDepartArriveeHeure(data.listeTrains, data.nbTrains, villeDeDepart, villeDArrivee, HeuredeDepart);
                    int tailleR;
                    tailleR = strlen(reponse);
                    write(socketService, &tailleR, sizeof(int));
                    // envoi du message dans la socket
                    write(socketService, reponse, tailleR);
                    // envoie le taille de la chaine puis la chaine
                    free(reponse);
                    // fonction qui renvoie le numero du train, son heure de depart,
                    break;
                }

                case 2:
                {
                    int taille = 0;
                    char requete[256];

                    // lecture de la taille du prochain message  contenant la ville de depart
                    // et stockage de la taille du message dans la variable taille
                    read(socketService, &taille, sizeof(int));

                    // affichage de la taille du message
                    printf("Octets lus : %d\n", taille);

                    // declaration d'une variable qui stocke la ville de depart
                    char villeDeDepart[taille];

                    // lecture du message qui contient la ville de depart
                    read(socketService, requete, taille);

                    // affichage du message lu
                    printf("Bien reçu: %s\n", requete);

                    // stockage de la ville de depart dans la variable villeDeDepart
                    strcpy(villeDeDepart, requete);
                    printf("Ville stocké: %s\n", villeDeDepart);

                    // fonction qui permet de mettre dans  0 la zone memoire
                    memset(requete, 0, 256);

                    // lecture de la taille du prochain message contenant la ville d'arrivee
                    // et stockage de la taille du message dans la variable taille
                    int tailleVA;
                    read(socketService, &tailleVA, sizeof(int));

                    // affichage de la taille du message
                    printf("octets lus : %d\n", tailleVA);

                    // declaration d'une variable qui stocke la ville de d'arrivee
                    char villeDArrivee[tailleVA];

                    // lecture du message qui contitent la ville d'arrivee
                    read(socketService, requete, tailleVA);
                    printf("Bien reçu: %s\n", requete);

                    // stockage de la ville d'arrivee dans la variable villeDArrivee
                    strcpy(villeDArrivee, requete);
                    printf("Ville stockée : %s\n", villeDArrivee);

                    // fonction qui permet de mettre dans  0 la zone memoire
                    memset(requete, 0, 256);

                    // lecture de la taille du prochain message contenant l'heure d'arrivee
                    // et stockage de la taille du message dans la variable taille
                    read(socketService, &taille, sizeof(int));

                    // affichage de la taille du message
                    printf("Octets lus : %d\n", taille);

                    // declaration d'une variable qui stocke l'heure de depart
                    char HeuredeDepart[taille];

                    // lecture du message qui contitent la l'heure de depart
                    read(socketService, requete, taille);
                    printf("Bien reçu: %s\n", requete);

                    // stockage de la ville d'arrivee dans la variable HeureDeDepart
                    strcpy(HeuredeDepart, requete);
                    printf("heure d'arrivée stockée : %s\n", HeuredeDepart);

                    // fonction qui permet de mettre dans 0 la zone memoire
                    memset(requete, 0, 256);

                    // lecture du message qui contitent la l'heure de depart
                    read(socketService, &taille, sizeof(int));
                    // declaration d'une variable qui stocke l'heure de depart
                    char HeuredeFin[taille];
                    printf("Bien reçu: %s\n", requete);
                    read(socketService, requete, taille);
                    // stockage de la ville d'arrivee dans la variable HeureDeDepart
                    strcpy(HeuredeFin, requete);
                    printf("heure de depart : %s \n, heuredeFin : %s\n", HeuredeDepart, HeuredeFin);

                    // lecture de la taille du prochain message contenant l'heure d'arrivee
                    // et stockage de la taille du message dans la variable taille

                    // Utilisation de la fct et init de struct data
                    struct TrainData data = parseur(nomFichier);
                    char *reponse = afficheTrainsDansTrancheHoraire(data.listeTrains, data.nbTrains, villeDeDepart, villeDArrivee, HeuredeDepart, HeuredeFin);
                    int tailleR;
                    tailleR = strlen(reponse);
                    write(socketService, &tailleR, sizeof(int));
                    // envoi du message dans la socket
                    write(socketService, reponse, tailleR);
                    // envoie le taille de la chaine puis la chaine

                    free(reponse);
                    break;
                }

                case 3:
                {
                    // Code pour le cas 3
                    int taille = 0;
                    char requete[256];

                    printf("On va lire\n");

                    // lecture de la taille du prochain message  contenant la ville de depart
                    // et stockage de la taille du message dans la variable taille
                    read(socketService, &taille, sizeof(int));

                    // affichage de la taille du message
                    printf("Octets lus : %d\n", taille);

                    // declaration d'une variable qui stocke la ville de depart
                    char villeDeDepart[taille];

                    // lecture du message qui contient la ville de depart
                    read(socketService, requete, taille);

                    // affichage du message lu
                    printf("Bien reçu : %s\n", requete);

                    // stockage de la ville de depart dans la variable villeDeDepart
                    strcpy(villeDeDepart, requete);
                    printf("Ville stockée : %s\n", villeDeDepart);

                    // fonction qui permet de mettre dans 0 la zone memoire
                    memset(requete, 0, 256);

                    // lecture de la taille du prochain message contenant la ville d'arrivee
                    // et stockage de la taille du message dans la variable taille
                    int tailleVA;
                    read(socketService, &tailleVA, sizeof(int));

                    // affichage de la taille du message
                    printf("Octets lus : %d\n", tailleVA);

                    // declaration d'une variable qui stocke la ville de d'arrivee
                    char villeDArrivee[tailleVA];

                    // lecture du message qui contitent la ville d'arrivee
                    read(socketService, requete, tailleVA);
                    printf("Bien reçu : %s\n", requete);

                    // stockage de la ville d'arrivee dans la variable villeDarrivée
                    strcpy(villeDArrivee, requete);
                    printf("Ville stockée : %s\n", villeDArrivee);

                    // fonction qui permet de mettre dans la zone mémoire
                    memset(requete, 0, 256);

                    // Utilisation de la fct et init de struct data
                    struct TrainData data = parseur(nomFichier);
                    char *reponse = afficheTousTrains(data.listeTrains, data.nbTrains, villeDeDepart, villeDArrivee);
                    int tailleAR;
                    tailleAR = strlen(reponse);
                    printf("%d\n", tailleAR);
                    write(socketService, &tailleAR, sizeof(int));
                    // envoi du message dans la socket
                    write(socketService, reponse, tailleAR);
                    // réception du choix

                    // réception du choix
                    int tailleTrajet;
                    read(socketService, &tailleTrajet, sizeof(int));
                    if (tailleTrajet == 1)
                    {
                        // déclaration + intialisation du pointeur meilleurPrix appelant une fonction meilleurTrajet1
                        char *mPrix = meilleurTrajet1(reponse);
                        int x = strlen(mPrix);
                        write(socketService, &x, sizeof(int));
                        // envoi du message dans la socket
                        write(socketService, reponse, x);
                        free(mPrix);
                    }
                    else
                    {
                        char *mDist = meilleurTrajetDuree(reponse);
                        int t = strlen(mDist);
                        write(socketService, &t, sizeof(int));
                        // envoi du message dans la socket
                        write(socketService, reponse, t);
                        free(mDist);
                    }
                    free(reponse);

                    break;
                }
                case 4:
                    continuer = 0;
                    printf("Fin de la connexion\n");

                default:
                    break;
                }
            }

        default:
            // on ferme la socket service
            close(socketService);
            break;
        }
    }
    return 0;
}