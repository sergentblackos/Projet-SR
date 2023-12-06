#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "parser.h"
#include "parser.c"




char *rechercheTrainVilleDepartArriveeHeure(struct Train *listeTrains, int nbTrains, char *villeDepart, char *villeArrivee, char *heureDepart)
{

    char *result = (char *)malloc(2048 * sizeof(char)); // Allocation d'une mémoire pour la chaîne résultante
    double nouveauPrix = 0;
    int found = 0;
    for (int i = 0; i < nbTrains; i++)
    {
        if (strcmp(listeTrains[i].villeDepart, villeDepart) == 0 &&
            strcmp(listeTrains[i].villeArrivee, villeArrivee) == 0 &&
            strcmp(listeTrains[i].heureDepart, heureDepart) >= 0)
        {
            if ((strcmp(listeTrains[i].option, "REDUC")))
            {
                double reduction = listeTrains[i].prix * 0.2;         // Calcul de la réduction (20% du prix)
                nouveauPrix = listeTrains[i].prix - reduction; // Calcul du nouveau prix
            }
            sprintf(result, "%d;%s;%s;%f\n",
                    listeTrains[i].numeroTrain,
                    listeTrains[i].heureDepart, listeTrains[i].heureArrivee,
                    nouveauPrix);
            found = 1;
            break;
        }
    }
    if (!found)
    {
        sprintf(result, "Aucun train ne correspond aux critères. Premier train disponible :\n");
    }
    return result;
}

// Manque d'octets



// Changer retour des fonctions
// Function to convert time string to minutes since midnight
int timeToMinutes(const char *timeStr) {
    int hours, minutes;
    sscanf(timeStr, "%d:%d", &hours, &minutes);
    return hours * 60 + minutes;
}

void afficherTrainsPlageHoraire(struct Train *listeTrains, int nbTrains, char *villeDepart, char *villeArrivee, char *plageDebut, char *plageFin)
{
    int trainsTrouvees = 0;

    // Convert time strings to minutes since midnight
    int debutMinutes = timeToMinutes(plageDebut);
    int finMinutes = timeToMinutes(plageFin);

    for (int i = 0; i < nbTrains; i++) {
        // Convert train departure and arrival times to minutes since midnight
        int departMinutes = timeToMinutes(listeTrains[i].heureDepart);
        int arriveeMinutes = timeToMinutes(listeTrains[i].heureArrivee);

        if (strcmp(listeTrains[i].villeDepart, villeDepart) == 0 &&
            strcmp(listeTrains[i].villeArrivee, villeArrivee) == 0 &&
            departMinutes >= debutMinutes &&
            arriveeMinutes <= finMinutes) {
            // Afficher les informations du train
            printf("Train trouvé : De %s à %s, Départ à %s, Arrivée à %s, Prix : %.2lf, Option : %s\n",
                   listeTrains[i].villeDepart, listeTrains[i].villeArrivee,
                   listeTrains[i].heureDepart, listeTrains[i].heureArrivee,
                   listeTrains[i].prix, listeTrains[i].option);

            trainsTrouvees++;
        }
    }

    if (trainsTrouvees == 0) {
        printf("Aucun train trouvé dans la tranche horaire spécifiée.\n");
    }
}


// le stocker ds un tableau
void afficherTousTrainsVilleDepartArrivee(struct Train *listeTrains, int nbTrains, char *villeDepart, char *villeArrivee)
{
    int trainsTrouves = 0;

    for (int i = 0; i < nbTrains; i++)
    {
        if (strcmp(listeTrains[i].villeDepart, villeDepart) == 0 &&
            strcmp(listeTrains[i].villeArrivee, villeArrivee) == 0)
        {
            // Afficher les informations du train
            printf("Train trouvé : De %s à %s, Départ à %s, Arrivée à %s, Prix : %.2lf, Option : %s\n",
                   listeTrains[i].villeDepart, listeTrains[i].villeArrivee,
                   listeTrains[i].heureDepart, listeTrains[i].heureArrivee,
                   listeTrains[i].prix, listeTrains[i].option);

            trainsTrouves++;
        }
    }

    // Gestion du cas où aucun train n'est trouvé
    if (trainsTrouves == 0)
    {
        printf("Aucun train trouvé entre les villes spécifiées.\n");
    }
}

void trajetMeilleurPrix(struct Train *listeTrains, int nbTrains)
{
    double minPrix = listeTrains[0].prix;
    int index = 0;
    for (int i = 1; i < nbTrains; i++)
    {
        if (listeTrains[i].prix < minPrix)
        {
            minPrix = listeTrains[i].prix;
            index = i;
        }
    }
    printf("Train avec le meilleur prix : De %s à %s, Départ à %s, Arrivée à %s, Prix : %.2lf, Option : %s\n",
           listeTrains[index].villeDepart, listeTrains[index].villeArrivee,
           listeTrains[index].heureDepart, listeTrains[index].heureArrivee,
           listeTrains[index].prix, listeTrains[index].option);
}

int main()
{
    // Appel de la fonction parseur pour obtenir les données des trains
    struct TrainData trainData = parseur();

    // Exemple d'utilisation des fonctionnalités mentionnées
    char villeDepart[] = "Valence";
    char villeArrivee[] = "Grenoble";
    char heureDepart[] = "16:00";
    char heureFin[] = "18:00";
    char plageDebut[] = "06:00";
    char plageFin[] = "07:00";


    //printf("%s", rechercheTrainVilleDepartArriveeHeure(trainData.listeTrains, trainData.nbTrains, villeDepart, villeArrivee, heureDepart));
    afficherTrainsPlageHoraire(trainData.listeTrains, trainData.nbTrains, "Grenoble", "Valence", heureDepart, heureFin);
    //afficherTousTrainsVilleDepartArrivee(trainData.listeTrains, trainData.nbTrains, "Valence", "Grenoble");
    // trajetMeilleurPrix(trainData.listeTrains, trainData.nbTrains);
    // printf("%s", listeTousTrainsVilleDepartArrivee(trainData.listeTrains, trainData.nbTrains, villeDepart, villeArrivee));

    // Assure-toi de libérer la mémoire allouée pour les trains
    free(trainData.listeTrains);

    return 0;
}
