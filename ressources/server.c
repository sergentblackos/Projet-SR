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

// Fonction end_child - sert Ãƒ  la gestion eds signaux SIGCHLD
void end_child()
{
    wait(NULL);
}

// Execution argc = noPort et argv = un fichier ou autre mot ?
int main(int argc, char **argv)
{ // Controle des arguments
    if (argc != 2)
    {
        printf("Il n'y pas le nombre de parametre adÃƒÂ©quats");
        return -1;
    }
    // Initialisation de noPort en tant qu'entier que l'on rentre en param
    int noPort = atoi(argv[1]);

    // CrÃƒÂ©ation de la socket d'ÃƒÂ©coute du serveur.
    int socketEcoute = socket(AF_INET, SOCK_STREAM, 0);
    // Gestion d'erreur de la socket d'ÃƒÂ©coute
    if (socketEcoute == -1)
    {
        perror("Socket");
        exit(-1);
    }

    // CrÃƒÂ©ation de la structure infoEcoute et dÃƒÂ©finition des variables de la structure
    struct sockaddr_in infoEcoute;
    infoEcoute.sin_family = AF_INET;                // protocole internet
    infoEcoute.sin_addr.s_addr = htonl(INADDR_ANY); // toutes les adresses IP de la machine
    infoEcoute.sin_port = htons(noPort);            // port d'ÃƒÂ©coute par dessus les ports rÃƒÂ©servÃƒÂ©s

    // Attachement de la socket d'ÃƒÂ©coute en utilisant la structure dÃƒÂ©finie dans infoEcoute
    int valideBind = bind(socketEcoute, (struct sockaddr *)&infoEcoute, sizeof(struct sockaddr));

    // Gestion d'erreur du bind de la socket
    if (valideBind == -1)
    {
        perror("bind");
        exit(-1);
    }

    // Ouverture de service sur la socket d'ÃƒÂ©coute
    int valideListen = listen(socketEcoute, 10);

    // Gestion d'erreur de la mise en ÃƒÂ©coute de la socket
    if (valideListen == -1)
    {
        perror("listen");
        exit(-1);
    }

    // CrÃƒÂ©ation de la structure infoEcoute et dÃƒÂ©finition des variables de la structure afin de
    // gÃƒÂ©rer la mort de chaque fils
    struct sigaction ac;
    ac.sa_handler = end_child;
    ac.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &ac, NULL);

    // CrÃƒÂ©ation de la boucle infinie

    while (1)
    {
        // CrÃƒÂ©ation de la structure infoClient pour stocker les donnÃƒÂ©es client
        unsigned int longueurInfoClient;
        struct sockaddr_in infoClient;

        // Acceptation d'une connexion sur la socket d'ÃƒÂ©coute,
        // rÃƒÂ©cupÃƒÂ©ration des informations sur le client dans struct infoClient
        // et crÃƒÂ©ation de la socket de service.
        printf("En attente de connexion...\n");
        int socketService = accept(socketEcoute, (struct sockaddr *)&infoClient, &longueurInfoClient);

        // gestion d'erreur de la crÃƒÂ©ation de socket de service
        if (socketService == -1)
        {
            perror("Erreur accept");
            exit(-1);
        }
        printf("Connexion ÃƒÂ©tablie\n");

        int continuer = 1;

        // CrÃƒÂ©ation du processus fils chargÃƒÂ© de satisfaire les requÃƒÂªtes du client.
        switch (fork())
        {
        case -1:
            perror("Erreur fork");
            exit(-1);

        case 0:
            // Comportement du processus fils

            // on ferme la socket d'ÃƒÂ©coute
            close(socketEcoute);

            // boucle de communication avec le client
            // dans le cas ou le client nous envoie 1 nous devons rÃƒÂ©ceptionner la ville de dÃƒÂ©part puis la ville d'arrivÃƒÂ©e
            // puis nous denons utiliser une fonction qui nous retourne le numÃƒÂ©ro du train, l'heure de dÃƒÂ©part, l'heure d'arrivÃƒÂ©e
            // et le prix
            // ensuite nous envoyons le rÃƒÂ©sultat de la fonction au client
            while (continuer)
            {
                int tailleMessage = 1;
                char message[256];
                char *pointeurSurChaine = message;

                // rÃƒÂ©ception du choix
                read(socketService, &tailleMessage, sizeof(int));

                printf("choix: %d\n", tailleMessage);
                // nous adaptons les fonctionnalitÃƒÂ©s en fonction du choix envoyÃƒÂ© par le client

                printf("nous rentrons dans les choix\n");
                switch (tailleMessage)
                {
                case 1:
                {
                    int taille = 0;
                    char requete[256];
                    char *pointeurSurRequete = requete;

                    printf("on vas lire\n");

                    // lecture de la taille du prochain message  contenant la ville de dÃƒÂ©part
                    // et stockage de la taille du message dans la variable taille
                    read(socketService, &taille, sizeof(int));

                    // affichage de la taille du message
                    printf("octets lus : %d\n", taille);

                    // dÃƒÂ©claration d'une variable qui stocke la ville de dÃƒÂ©part
                    char villeDeDepart[taille];

                    // lecture du message qui contient la ville de dÃƒÂ©part
                    read(socketService, requete, taille);

                    // affichage du message lu
                    printf("Bien recu : %s\n", requete);

                    // stockage de la ville de dÃƒÂ©part dans la variable villeDeDepart
                    strcpy(villeDeDepart, requete);
                    printf("ville stockÃƒÂ©e : %s\n", villeDeDepart);

                    // fonction qui permet de mettre Ãƒ  0 la zone mÃƒÂ©moire
                    memset(requete, 0, 256);

                    // lecture de la taille du prochain message contenant la ville d'arrivÃƒÂ©e
                    // et stockage de la taille du message dans la variable taille
                    int tailleVA;
                    read(socketService, &tailleVA, sizeof(int));

                    // affichage de la taille du message
                    printf("octets lus : %d\n", tailleVA);

                    // dÃƒÂ©claration d'une variable qui stocke la ville de d'arrivÃƒÂ©e
                    char villeDArrivee[tailleVA];

                    // lecture du message qui contitent la ville d'arrivÃƒÂ©e
                    read(socketService, requete, tailleVA);
                    printf("Bien recu : %s\n", requete);

                    // stockage de la ville d'arrivÃƒÂ©e dans la variable villeDArrivee
                    strcpy(villeDArrivee, requete);
                    printf("ville stockÃƒÂ©e : %s\n", villeDArrivee);

                    // fonction qui permet de mettre Ãƒ  0 la zone mÃƒÂ©moire
                    memset(requete, 0, 256);

                    // lecture de la taille du prochain message contenant l'heure d'arrivÃƒÂ©e
                    // et stockage de la taille du message dans la variable taille
                    read(socketService, &taille, sizeof(int));

                    // affichage de la taille du message
                    printf("octets lus : %d\n", taille);

                    // dÃƒÂ©claration d'une variable qui stocke l'heure de dÃƒÂ©part
                    char HeuredeDepart[taille];

                    // lecture du message qui contitent la l'heure de dÃƒÂ©part
                    read(socketService, requete, taille);
                    printf("Bien recu : %s\n", requete);

                    // stockage de la ville d'arrivÃƒÂ©e dans la variable HeureDeDepart
                    strcpy(HeuredeDepart, requete);
                    printf("heure d'arrivÃƒÂ©e stockÃƒÂ©e : %s\n", HeuredeDepart);

                    printf("ville de dÃƒÂ©part: %s , ville d'arrivÃƒÂ©e: %s, heure de dÃƒÂ©part, %s \n", villeDeDepart, villeDArrivee, HeuredeDepart);

                    // Utilisation de la fct et init de struct data
                    struct TrainData data = parseur();
                    char *reponse = rechercheTrainVilleDepartArriveeHeure(data.listeTrains, data.nbTrains, villeDeDepart, villeDArrivee, HeuredeDepart);
                    printf("%s", reponse);
                    printf("%ld\n", strlen(reponse));
                    int tailleAR;
                    tailleAR = strlen(reponse);
                    printf("%d\n", tailleAR);
                    write(socketService, &tailleAR, sizeof(int));
                    // envoi du message dans la socket
                    write(socketService, reponse, tailleAR);
                    // envoie le taille de la chaine puis la chaine

                    free(reponse);
                    // fonction qui renvoie le numÃƒÂ©ro du train, son heure de dÃƒÂ©part,
                    // son heure d'arrivÃƒÂ©e et son prix

                    // stockage du rÃƒÂ©sultat dans une variable resultat

                    // envoi de la variable resultat au client
                    break;
                }

                case 2:
                {
                    int taille = 0;
                    char requete[256];
                    char *pointeurSurRequete = requete;

                    printf("on vas lire\n");

                    // lecture de la taille du prochain message  contenant la ville de dÃƒÂ©part
                    // et stockage de la taille du message dans la variable taille
                    read(socketService, &taille, sizeof(int));

                    // affichage de la taille du message
                    printf("octets lus : %d\n", taille);

                    // dÃƒÂ©claration d'une variable qui stocke la ville de dÃƒÂ©part
                    char villeDeDepart[taille];

                    // lecture du message qui contient la ville de dÃƒÂ©part
                    read(socketService, requete, taille);

                    // affichage du message lu
                    printf("Bien recu : %s\n", requete);

                    // stockage de la ville de dÃƒÂ©part dans la variable villeDeDepart
                    strcpy(villeDeDepart, requete);
                    printf("ville stockÃƒÂ©e : %s\n", villeDeDepart);

                    // fonction qui permet de mettre Ãƒ  0 la zone mÃƒÂ©moire
                    memset(requete, 0, 256);

                    // lecture de la taille du prochain message contenant la ville d'arrivÃƒÂ©e
                    // et stockage de la taille du message dans la variable taille
                    int tailleVA;
                    read(socketService, &tailleVA, sizeof(int));

                    // affichage de la taille du message
                    printf("octets lus : %d\n", tailleVA);

                    // dÃƒÂ©claration d'une variable qui stocke la ville de d'arrivÃƒÂ©e
                    char villeDArrivee[tailleVA];

                    // lecture du message qui contitent la ville d'arrivÃƒÂ©e
                    read(socketService, requete, tailleVA);
                    printf("Bien recu : %s\n", requete);

                    // stockage de la ville d'arrivÃƒÂ©e dans la variable villeDArrivee
                    strcpy(villeDArrivee, requete);
                    printf("ville stockÃƒÂ©e : %s\n", villeDArrivee);

                    // fonction qui permet de mettre Ãƒ  0 la zone mÃƒÂ©moire
                    memset(requete, 0, 256);

                    // lecture de la taille du prochain message contenant l'heure d'arrivÃƒÂ©e
                    // et stockage de la taille du message dans la variable taille
                    read(socketService, &taille, sizeof(int));

                    // affichage de la taille du message
                    printf("octets lus : %d\n", taille);

                    // dÃƒÂ©claration d'une variable qui stocke l'heure de dÃƒÂ©part
                    char HeuredeDepart[taille];

                    // lecture du message qui contitent la l'heure de dÃƒÂ©part
                    read(socketService, requete, taille);
                    printf("Bien recu : %s\n", requete);

                    // stockage de la ville d'arrivÃƒÂ©e dans la variable HeureDeDepart
                    strcpy(HeuredeDepart, requete);
                    printf("heure d'arrivÃƒÂ©e stockÃƒÂ©e : %s\n", HeuredeDepart);

                    // fonction qui permet de mettre Ãƒ  0 la zone mÃƒÂ©moire
                    memset(requete, 0, 256);

                    // lecture du message qui contitent la l'heure de dÃƒÂ©part
                    read(socketService, &taille, sizeof(int));
                    // dÃƒÂ©claration d'une variable qui stocke l'heure de dÃƒÂ©part
                    char HeuredeFin[taille];
                    printf("Bien recu : %s\n", requete);
                    read(socketService, requete, taille);
                    // stockage de la ville d'arrivÃƒÂ©e dans la variable HeureDeDepart
                    strcpy(HeuredeFin, requete);
                    printf("heure de dÃƒÂ©part : %s \n, heuredeFin : %s\n", HeuredeDepart, HeuredeFin);

                    // lecture de la taille du prochain message contenant l'heure d'arrivÃƒÂ©e
                    // et stockage de la taille du message dans la variable taille

                    // Utilisation de la fct et init de struct data
                    struct TrainData data = parseur();
                    char *reponse = afficheTrainsDansTrancheHoraire(data.listeTrains, data.nbTrains, villeDeDepart, villeDArrivee, HeuredeDepart, HeuredeFin);
                    printf("%s", reponse);
                    printf("%ld\n", strlen(reponse));
                    int tailleAR;
                    tailleAR = strlen(reponse);
                    printf("%d\n", tailleAR);
                    write(socketService, &tailleAR, sizeof(int));
                    // envoi du message dans la socket
                    write(socketService, reponse, tailleAR);
                    // envoie le taille de la chaine puis la chaine

                    free(reponse);
                    // fonction qui renvoie le numÃƒÂ©ro du train, son heure de dÃƒÂ©part,
                    // son heure d'arrivÃƒÂ©e et son prix

                    // stockage du rÃƒÂ©sultat dans une variable resultat

                    // envoi de la variable resultat au client

                    // nous avons terminÃƒÂ©, nous attendons un nouveau choix du client
                    // on ferme la socket de service
                    break;
                }

                case 3:
                {
                    // Code pour le cas 3
                    int taille = 0;
                    char requete[256];
                    char *pointeurSurRequete = requete;

                    printf("on vas lire\n");

                    // lecture de la taille du prochain message  contenant la ville de dÃƒÂ©part
                    // et stockage de la taille du message dans la variable taille
                    read(socketService, &taille, sizeof(int));

                    // affichage de la taille du message
                    printf("octets lus : %d\n", taille);

                    // dÃƒÂ©claration d'une variable qui stocke la ville de dÃƒÂ©part
                    char villeDeDepart[taille];

                    // lecture du message qui contient la ville de dÃƒÂ©part
                    read(socketService, requete, taille);

                    // affichage du message lu
                    printf("Bien recu : %s\n", requete);

                    // stockage de la ville de dÃƒÂ©part dans la variable villeDeDepart
                    strcpy(villeDeDepart, requete);
                    printf("ville stockÃƒÂ©e : %s\n", villeDeDepart);

                    // fonction qui permet de mettre Ãƒ  0 la zone mÃƒÂ©moire
                    memset(requete, 0, 256);

                    // lecture de la taille du prochain message contenant la ville d'arrivÃƒÂ©e
                    // et stockage de la taille du message dans la variable taille
                    int tailleVA;
                    read(socketService, &tailleVA, sizeof(int));

                    // affichage de la taille du message
                    printf("octets lus : %d\n", tailleVA);

                    // dÃƒÂ©claration d'une variable qui stocke la ville de d'arrivÃƒÂ©e
                    char villeDArrivee[tailleVA];

                    // lecture du message qui contitent la ville d'arrivÃƒÂ©e
                    read(socketService, requete, tailleVA);
                    printf("Bien recu : %s\n", requete);

                    // stockage de la ville d'arrivÃƒÂ©e dans la variable villeDArrivee
                    strcpy(villeDArrivee, requete);
                    printf("ville stockÃƒÂ©e : %s\n", villeDArrivee);

                    // fonction qui permet de mettre Ãƒ  0 la zone mÃƒÂ©moire
                    memset(requete, 0, 256);

                    // Utilisation de la fct et init de struct data
                    struct TrainData data = parseur();
                    char *reponse = afficheTousTrains(data.listeTrains, data.nbTrains, villeDeDepart, villeDArrivee);
                    printf("%s", reponse);
                    printf("%ld\n", strlen(reponse));
                    int tailleAR;
                    tailleAR = strlen(reponse);
                    printf("%d\n", tailleAR);
                    write(socketService, &tailleAR, sizeof(int));
                    // envoi du message dans la socket
                    write(socketService, reponse, tailleAR);
                    // reception du choix

                    // rÃƒÂ©ception du choix
                    int tailleTrajet;
                    read(socketService, &tailleTrajet, sizeof(int));
                    if (tailleTrajet == 1)
                    {
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

                    // fonction qui renvoie le numÃƒÂ©ro du train, son heure de dÃƒÂ©part,
                    // son heure d'arrivÃƒÂ©e et son prix

                    // stockage du rÃƒÂ©sultat dans une variable resultat

                    // envoi de la variable resultat au client
                    free(reponse);

                    // nous avons terminÃƒÂ©, nous attendons un nouveau choix du client
                    // on ferme la socket de service
                    break;
                }
                case 4:
                    continuer = 0;
                    // Ajoutez d'autres cas au besoin

                default:
                    // Code Ã  exÃ©cuter si tailleMessage ne correspond Ã  aucun des cas prÃ©cÃ©dents
                    break;
                }
            }

        default:
            // Comportement processus pÃƒÂ¨re

            // on ferme la socket service
            close(socketService);
            printf("fin de la connexion\n");
            break;
        }
    }
    return 0;
}