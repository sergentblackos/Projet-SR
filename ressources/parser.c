#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include "parser.h"
#define nombreMaxTrains 100

// Fonction pour parser les donnÃ©es Ã  partir d'un fichier
struct TrainData parseur(const char *nomFichier) {
    // Ouverture du fichier en mode lecture
    FILE *fichier = fopen(nomFichier, "r");
    
    // VÃ©rification de l'ouverture rÃ©ussie du fichier
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    // Allocation dynamique de mÃ©moire pour stocker les informations sur les trains
    struct Train *matriceTrains = (struct Train *)malloc(nombreMaxTrains * sizeof(struct Train));

    // VÃ©rification de l'allocation rÃ©ussie
    if (matriceTrains == NULL) {
        perror("Erreur lors de l'allocation mÃ©moire");
        exit(EXIT_FAILURE);
    }

    // Lecture du fichier ligne par ligne et extraction des informations sur les trains
    char ligne[256];
    int i = 0;

    while (i < nombreMaxTrains && fgets(ligne, sizeof(ligne), fichier) != NULL) {
        sscanf(ligne, "%d;%49[^;];%49[^;];%49[^;];%49[^;];%lf;%49[^;];",
               &matriceTrains[i].numeroTrain, matriceTrains[i].villeDepart,
               matriceTrains[i].villeArrivee, matriceTrains[i].heureDepart,
               matriceTrains[i].heureArrivee, &matriceTrains[i].prix,
               matriceTrains[i].option);
        i++;
    }

    // Fermeture du fichier aprÃ¨s lecture
    fclose(fichier);

    // CrÃ©ation d'une structure TrainData pour stocker les donnÃ©es des trains
    struct TrainData trainData;
    trainData.listeTrains = matriceTrains;
    trainData.nbTrains = i;

    // Retourne la structure TrainData
    return trainData;
}