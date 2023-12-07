#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
// parser.c
#include "parser.h"



struct TrainData parseur() {
    FILE *fichier = fopen("train.txt", "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(-1);
    }

    const int nombreMaxTrains = 100; // Ajustez la taille selon vos besoins
    struct Train *matriceTrains = (struct Train *)malloc(nombreMaxTrains * sizeof(struct Train));

    if (matriceTrains == NULL) {
        perror("Erreur lors de l'allocation mÃƒÂ©moire");
        exit(-1);
    }

    char ligne[256];
    int i = 0;

    while (fgets(ligne, sizeof(ligne), fichier) != NULL && i < nombreMaxTrains) {
        sscanf(ligne, "%d;%49[^;];%49[^;];%49[^;];%49[^;];%lf;%49[^;];",
               &matriceTrains[i].numeroTrain, matriceTrains[i].villeDepart,
               matriceTrains[i].villeArrivee, matriceTrains[i].heureDepart,
               matriceTrains[i].heureArrivee, &matriceTrains[i].prix,
               matriceTrains[i].option);
        i++;
    }

    fclose(fichier);

    struct TrainData trainData;
    trainData.listeTrains = matriceTrains;
    trainData.nbTrains = i;
    return trainData;
}

