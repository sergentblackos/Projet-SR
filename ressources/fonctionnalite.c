#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <limits.h>
#include "parser.h"
#include "parser.c"
// Supposez que MAX_STRING_LENGTH soit une valeur suffisamment grande pour stocker la chaÃƒÂ®ne rÃƒÂ©sultante
#define MAX_STRING_LENGTH 2048

char *rechercheTrainVilleDepartArriveeHeure(struct Train *listeTrains, int nbTrains, char *villeDepart, char *villeArrivee, char *heureDepart)
{

    char *result = (char *)malloc(2048 * sizeof(char)); // Allocation d'une mÃƒÂ©moire pour la chaÃƒÂ®ne rÃƒÂ©sultante

    int found = 0;
    double nouveauPrix = 0;
    double reduction = 0.2; // 20% de réduction
    double augmentation = 0.1; // 10% d'augmentation
    for (int i = 0; i < nbTrains; i++)
    {

        if (strcmp(listeTrains[i].villeDepart, villeDepart) == 0 &&
            strcmp(listeTrains[i].villeArrivee, villeArrivee) == 0 &&
            strcmp(listeTrains[i].heureDepart, heureDepart) >= 0)
        {
            printf("%s",listeTrains[i].option);

            // Appliquer la réduction ou l'augmentation en fonction de l'option
            if (strcmp(listeTrains[i].option, "REDUC\n") == 0)
            {
                nouveauPrix = listeTrains[i].prix * (1 - reduction);
            }
            else if (strcmp(listeTrains[i].option, "SUPPL\n") == 0)
            {
                printf("YO");nouveauPrix = listeTrains[i].prix * (1 + augmentation);
            }
            else
            {
                // Option non reconnue, utiliser le prix existant
                nouveauPrix = listeTrains[i].prix;
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
        sprintf(result, " "); // Aucun train ne correspond aux critÃƒÂ¨res. Premier train disponible :\n
    }
    return result;
}

// Manque d'octets

// Changer retour des fonctions
// Function to convert time string to minutes since midnight
int timeToMinutes(const char *timeStr)
{
    int hours, minutes;
    sscanf(timeStr, "%d:%d", &hours, &minutes);
    return hours * 60 + minutes;
}

    // Changer le retour des fonctions
// ...
// Fonction pour afficher tous les trains entre deux villes dans une tranche horaire
char* afficheTrainsDansTrancheHoraire(struct Train *listeTrains, int nbTrains, const char *villeDepart, const char *villeArrivee, const char *heureDepartStart, const char *heureDepartEnd) {
    // Allouer de l'espace pour stocker la chaÃ®ne rÃ©sultante
    char *resultat = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));
        double nouveauPrix = 0;
    double reduction = 0.2; // 20% de réduction
    double augmentation = 0.1; // 10% d'augmentation

    // VÃ©rifier si l'allocation de mÃ©moire a rÃ©ussi
    if (resultat == NULL) {
        perror("Erreur d'allocation de mÃ©moire");
        exit(EXIT_FAILURE);
    }

    // Initialiser la chaÃ®ne rÃ©sultante avec une chaÃ®ne vide
    resultat[0] = '\0';

    int trainsTrouves = 0;

    for (int i = 0; i < nbTrains; i++) {
        if (strcmp(listeTrains[i].villeDepart, villeDepart) == 0 &&
            strcmp(listeTrains[i].villeArrivee, villeArrivee) == 0 &&
            timeToMinutes(listeTrains[i].heureDepart) >= timeToMinutes(heureDepartStart) &&
            timeToMinutes(listeTrains[i].heureDepart) <= timeToMinutes(heureDepartEnd)) {

                // Appliquer la réduction ou l'augmentation en fonction de l'option
            if (strcmp(listeTrains[i].option, "REDUC\n") == 0)
            {
                nouveauPrix = listeTrains[i].prix * (1 - reduction);
            }
            else if (strcmp(listeTrains[i].option, "SUPPL\n") == 0)
            {
                printf("YO");nouveauPrix = listeTrains[i].prix * (1 + augmentation);
            }
            else
            {
                // Option non reconnue, utiliser le prix existant
                nouveauPrix = listeTrains[i].prix;
            }
            
            // ConcatÃ©ner les informations du train Ã  la chaÃ®ne rÃ©sultante
            sprintf(resultat + strlen(resultat), "%d;%s;%s;%.2lf\n",
                    listeTrains[i].numeroTrain,
                    listeTrains[i].heureDepart, listeTrains[i].heureArrivee,
                    listeTrains[i].prix);

            trainsTrouves++;
        }
    }

    // Gestion du cas oÃ¹ aucun train n'est trouvÃ©
    if (trainsTrouves == 0) {
        // Utilisez directement sprintf sans strlen pour ajouter le message
        sprintf(resultat, "Aucun train trouvÃ© dans la tranche horaire spÃ©cifiÃ©e\n");
    }

    // Renvoyer la chaÃ®ne rÃ©sultante
    return resultat;
}



struct TrainData getTrainsByTimeRange(const char *departureCity, const char *arrivalCity, const char *departureTimeStart, const char *departureTimeEnd) {
    struct TrainData allTrains = parseur();
    struct TrainData filteredTrains;

    // Allocate memory for the filtered trains (assuming the worst case where all trains match the criteria)
    filteredTrains.listeTrains = (struct Train *)malloc(allTrains.nbTrains * sizeof(struct Train));
    if (filteredTrains.listeTrains == NULL) {
        perror("Erreur lors de l'allocation mÃ©moire");
        exit(-1);
    }

    filteredTrains.nbTrains = 0; // Initialize the number of filtered trains

    // Loop through all trains and check if they match the criteria
    for (int i = 0; i < allTrains.nbTrains; ++i) {
        struct Train currentTrain = allTrains.listeTrains[i];

        if (strcmp(currentTrain.villeDepart, departureCity) == 0 &&
            strcmp(currentTrain.villeArrivee, arrivalCity) == 0 &&
            timeToMinutes(currentTrain.heureDepart) >= timeToMinutes(departureTimeStart) &&
            timeToMinutes(currentTrain.heureDepart) <= timeToMinutes(departureTimeEnd)) {
            
            // Add the train to the filtered list
            filteredTrains.listeTrains[filteredTrains.nbTrains++] = currentTrain;
        }
    }

    // Resize the memory to fit the exact number of filtered trains
    filteredTrains.listeTrains = (struct Train *)realloc(filteredTrains.listeTrains, filteredTrains.nbTrains * sizeof(struct Train));

    return filteredTrains;
}





// Fonction pour afficher tous les trains entre deux villes
char *afficheTousTrains(struct Train *listeTrains, int nbTrains, char *villeDepart, char *villeArrivee)
{
    // Allouer de l'espace pour stocker la chaÃƒÂ®ne rÃƒÂ©sultante
    char *resultat = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));
        double nouveauPrix = 0;
    double reduction = 0.2; // 20% de réduction
    double augmentation = 0.1; // 10% d'augmentation

    // VÃƒÂ©rifier si l'allocation de mÃƒÂ©moire a rÃƒÂ©ussi
    if (resultat == NULL)
    {
        perror("Erreur d'allocation de mÃƒÂ©moire");
        exit(EXIT_FAILURE);
    }

    // Initialiser la chaÃƒÂ®ne rÃƒÂ©sultante avec une chaÃƒÂ®ne vide
    resultat[0] = '\0';

    int trainsTrouves = 0;

    for (int i = 0; i < nbTrains; i++)
    {
        if (strcmp(listeTrains[i].villeDepart, villeDepart) == 0 &&
            strcmp(listeTrains[i].villeArrivee, villeArrivee) == 0)
        {

                // Appliquer la réduction ou l'augmentation en fonction de l'option
            if (strcmp(listeTrains[i].option, "REDUC\n") == 0)
            {
                nouveauPrix = listeTrains[i].prix * (1 - reduction);
            }
            else if (strcmp(listeTrains[i].option, "SUPPL\n") == 0)
            {
                printf("YO");nouveauPrix = listeTrains[i].prix * (1 + augmentation);
            }
            else
            {
                // Option non reconnue, utiliser le prix existant
                nouveauPrix = listeTrains[i].prix;
            }
            // ConcatÃƒÂ©ner les informations du train Ãƒ  la chaÃƒÂ®ne rÃƒÂ©sultante
            sprintf(resultat + strlen(resultat), "%d;%s;%s;%.2lf\n",
                    listeTrains[i].numeroTrain,
                    listeTrains[i].heureDepart, listeTrains[i].heureArrivee,
                    listeTrains[i].prix);

            trainsTrouves++;
        }
    }

    // Gestion du cas oÃƒÂ¹ aucun train n'est trouvÃƒÂ©
    if (trainsTrouves == 0)
    {
        // Utilisez directement sprintf sans strlen pour ajouter le message
        sprintf(resultat, "0\n");
    }

    // Renvoyer la chaÃƒÂ®ne rÃƒÂ©sultante
    return resultat;
}

// Structure pour reprÃƒÂ©senter un trajet
struct Trajet
{
    int numeroTrain;
    char heureDepart[6];
    char heureArrivee[6];
    double prix;
    int duree;
};

char *meilleurTrajet1(char *resultat)
{
    struct Trajet meilleur;
    meilleur.prix = __DBL_MAX__; // Comme en Java DOUBLE_MAXVALUE pour trouver le minimum

    // Utilisation d'une copie de la chaÃƒÂ®ne pour ÃƒÂ©viter la modification de l'original
    char *copieResultat = strdup(resultat);
    if (copieResultat == NULL)
    {
        perror("Erreur d'allocation de mÃƒÂ©moire");
        exit(EXIT_FAILURE);
    }

    // Alloue de l'espace pour stocker la chaÃƒÂ®ne rÃƒÂ©sultante
    char *resultatFinal = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));
    if (resultatFinal == NULL)
    {
        perror("Erreur d'allocation de mÃƒÂ©moire");
        exit(EXIT_FAILURE);
    }

    // Initialiser la chaÃƒÂ®ne rÃƒÂ©sultante avec une chaÃƒÂ®ne vide
    resultatFinal[0] = '\0';

    // Definit le separateur en tant que \n
    char *token = strtok(copieResultat, "\n");
    // Tq la chaine n'est pas null
    while (token != NULL)
    {
        struct Trajet trajet;
        // Le sscanf definit comment la chaine est faites
        sscanf(token, "%d;%5[^;];%5[^;];%lf", &trajet.numeroTrain, trajet.heureDepart, trajet.heureArrivee, &trajet.prix);

        // Comparaison des prix pour trouver le meilleur trajet
        if (trajet.prix < meilleur.prix)
        {
            meilleur = trajet;
        }

        // ConcatÃƒÂ©ner les informations du trajet Ãƒ  la chaÃƒÂ®ne rÃƒÂ©sultante
        sprintf(resultatFinal, "%d;%s;%s;%.2lf\n",
                meilleur.numeroTrain,
                meilleur.heureDepart, meilleur.heureArrivee,
                meilleur.prix);

        token = strtok(NULL, "\n");
    }
    free(copieResultat);
    return resultatFinal;
}

int calculerDuree(const char *heureDepart, const char *heureArrivee)
{
    int departMinutes = timeToMinutes(heureDepart);
    int arriveeMinutes = timeToMinutes(heureArrivee);

    return arriveeMinutes - departMinutes;
}

char *meilleurTrajetDuree(char *resultat) {
    struct Trajet meilleur;
    meilleur.duree = INT_MAX; // Initialiser avec une valeur maximale pour trouver la durÃƒÂ©e minimale

    // Utilisation d'une copie de la chaÃƒÂ®ne pour ÃƒÂ©viter la modification de l'original
    char *copieResultat = strdup(resultat);
    if (copieResultat == NULL) {
        perror("Erreur d'allocation de mÃƒÂ©moire");
        exit(EXIT_FAILURE);
    }

    // DÃƒÂ©finir le sÃƒÂ©parateur en tant que \n
    char *token = strtok(copieResultat, "\n");

    // Tant que la chaÃƒÂ®ne n'est pas nulle
    while (token != NULL) {
        struct Trajet trajet;
        // Le sscanf dÃƒÂ©finit comment la chaÃƒÂ®ne est faite
        sscanf(token, "%d;%5[^;];%5[^;];%lf", &trajet.numeroTrain, trajet.heureDepart, trajet.heureArrivee, &trajet.prix);

        // Calcul de la durÃƒÂ©e du trajet
        trajet.duree = calculerDuree(trajet.heureDepart, trajet.heureArrivee);

        // Comparaison des durÃƒÂ©es pour trouver le meilleur trajet
        if (trajet.duree < meilleur.duree) {
            meilleur = trajet;
        }

        token = strtok(NULL, "\n");
    }

    free(copieResultat);

    // CrÃƒÂ©er une chaÃƒÂ®ne pour le rÃƒÂ©sultat
    char *resultatFinal = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));
    if (resultatFinal == NULL) {
        perror("Erreur d'allocation de mÃƒÂ©moire");
        exit(EXIT_FAILURE);
    }

    // Formatter le rÃƒÂ©sultat en chaÃƒÂ®ne de caractÃƒÂ¨res
    sprintf(resultatFinal, "%d;%s;%s;%.2lf\n",
            meilleur.numeroTrain, meilleur.heureDepart, meilleur.heureArrivee, meilleur.prix);

    return resultatFinal;
}


int main(){

   // Get user input for departure city, arrival city, and departure time range
    
    // Get filtered trains based on user input

    // Obtenez les informations de tous les trains
    struct TrainData allTrains = parseur();

    // Appelez la nouvelle fonction pour obtenir les trains dans la tranche horaire spÃ©cifiÃ©e
    char *result = rechercheTrainVilleDepartArriveeHeure(allTrains.listeTrains, allTrains.nbTrains, "Grenoble", "Valence", "16:55");

    // Affichez le rÃ©sultat
    printf("RÃ©sultat de la recherche :\n%s", result);

    // LibÃ©rez la mÃ©moire allouÃ©e
    free(allTrains.listeTrains);
    free(result);

    return 0;


    
} 