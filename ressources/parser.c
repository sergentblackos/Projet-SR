    #include <stdio.h>
    #include <stdlib.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netdb.h>
    #include <string.h>
    #include <unistd.h>
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
            perror("Erreur lors de l'allocation mémoire");
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


    /* int main() {
        // Appel de la fonction parseur pour obtenir les données des trains
        struct TrainData trainData = parseur();

        // Affichage des données des trains extraites
        printf("Nombre de trains : %d\n", trainData.nbTrains);
        for (int i = 0; i < trainData.nbTrains; ++i) {
            printf("Train %d : De %s à %s, Départ à %s, Arrivée à %s, Prix : %.2lf, Option : %s\n",
                trainData.listeTrains[i].numeroTrain, trainData.listeTrains[i].villeDepart,
                trainData.listeTrains[i].villeArrivee, trainData.listeTrains[i].heureDepart,
                trainData.listeTrains[i].heureArrivee, trainData.listeTrains[i].prix,
                trainData.listeTrains[i].option);
        }

        // Libération de la mémoire allouée pour les trains
        free(trainData.listeTrains);

        return 0;
    } */