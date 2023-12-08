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
#include <regex.h>
#include "parser.c"

char *rechercheTrainVilleDepartArriveeHeure(struct Train *listeTrains, int nbTrains, char *villeDepart, char *villeArrivee, char *heureDepart)
{
    // Allocation dynamique de mémoire pour stocker la chaîne résultante
    char *result = (char *)malloc(2048 * sizeof(char));

    // Variable pour indiquer si un train satisfaisant les critères a été trouvé
    int found = 0;

    // Variable pour stocker le nouveau prix du train en fonction des options
    double nouveauPrix = 0;

    // Taux de réduction de 20%
    double reduction = 0.2;

    // Taux d'augmentation de 10%
    double augmentation = 0.1;

    // Boucle parcourant tous les trains dans la liste
    int i = 0;
    while (i < nbTrains && !found)
    {
        // Condition vérifiant si les critères de ville de départ, ville d'arrivée et heure de départ sont satisfaits pour le train actuel
        if (strcmp(listeTrains[i].villeDepart, villeDepart) == 0 &&
            strcmp(listeTrains[i].villeArrivee, villeArrivee) == 0 &&
            strcmp(listeTrains[i].heureDepart, heureDepart) >= 0)
        {
            // Affichage de l'option du train (peut Ãªtre commenté si non nécessaire)
            printf("%s", listeTrains[i].option);

            // Bloc conditionnel appliquant la réduction, l'augmentation ou le prix existant en fonction de l'option du train
            if (strcmp(listeTrains[i].option, "REDUC\n") == 0)
            {
                nouveauPrix = listeTrains[i].prix * (1 - reduction);
            }
            else if (strcmp(listeTrains[i].option, "SUPPL\n") == 0)
            {
                nouveauPrix = listeTrains[i].prix * (1 + augmentation);
            }
            else
            {
                // Option non reconnue, utiliser le prix existant
                nouveauPrix = listeTrains[i].prix;
            }

            // Construction de la chaîne résultante en utilisant sprintf pour formater les détails du train
            sprintf(result, "%d;%s;%s;%f\n",
                    listeTrains[i].numeroTrain,
                    listeTrains[i].heureDepart, listeTrains[i].heureArrivee,
                    nouveauPrix);

            // Marquer que le train satisfaisant les critères a été trouvé
            found = 1;
        }
        i++;
    }

    // Si aucun train n'est trouvé, la chaîne "*" est stockée dans result
    if (!found)
    {
        sprintf(result, "*");
    }

    // Retourne la chaîne résultante
    return result;
}

// Convertit une chaîne de caractères représentant une heure au format "hh:mm" en minutes depuis minuit.
int heureMinutes(const char *heureStr)
{
    int heures, minutes;
    sscanf(heureStr, "%d:%d", &heures, &minutes);
    return heures * 60 + minutes;
}

// Fonction pour afficher tous les trains entre deux villes dans une tranche horaire
char *afficheTrainsDansTrancheHoraire(struct Train *listeTrains, int nbTrains, const char *villeDepart, const char *villeArrivee, const char *heureDepartStart, const char *heureDepartEnd)
{
    // Allouer de l'espace pour stocker la chaîne résultante
    char *resultat = (char *)malloc(2048 * sizeof(char));

    // Variable pour stocker le nouveau prix du train en fonction des options
    double nouveauPrix = 0;

    // Taux de réduction de 20%
    double reduction = 0.2;

    // Taux d'augmentation de 10%
    double augmentation = 0.1;

    // Vérifier si l'allocation de mémoire a réussi
    if (resultat == NULL)
    {
        perror("Erreur d'allocation de mémoire");
        exit(EXIT_FAILURE);
    }

    // Initialiser la chaîne résultante avec une chaîne vide
    resultat[0] = '\0';

    // Variable pour compter le nombre de trains trouvés
    int trainsTrouves = 0;

    // Parcourir tous les trains dans la liste
    for (int i = 0; i < nbTrains; i++)
    {
        // Vérifier si le train correspond aux critères de ville de départ, ville d'arrivée et tranche horaire
        if (strcmp(listeTrains[i].villeDepart, villeDepart) == 0 &&
            strcmp(listeTrains[i].villeArrivee, villeArrivee) == 0 &&
            heureMinutes(listeTrains[i].heureDepart) >= heureMinutes(heureDepartStart) &&
            heureMinutes(listeTrains[i].heureDepart) <= heureMinutes(heureDepartEnd))
        {
            // Appliquer la réduction ou l'augmentation en fonction de l'option
            if (strcmp(listeTrains[i].option, "REDUC\n") == 0)
            {
                nouveauPrix = listeTrains[i].prix * (1 - reduction);
            }
            else if (strcmp(listeTrains[i].option, "SUPPL\n") == 0)
            {
                nouveauPrix = listeTrains[i].prix * (1 + augmentation);
            }
            else
            {
                // Option non reconnue, utiliser le prix existant
                nouveauPrix = listeTrains[i].prix;
            }

            // Concaténer les informations du train Ã  la chaîne résultante
            sprintf(resultat + strlen(resultat), "%d;%s;%s;%.2lf\n",
                    listeTrains[i].numeroTrain,
                    listeTrains[i].heureDepart, listeTrains[i].heureArrivee,
                    nouveauPrix);

            // Incrémenter le nombre de trains trouvés
            trainsTrouves++;
        }
    }

    // Gestion du cas oÃ¹ aucun train n'est trouvé
    if (trainsTrouves == 0)
    {
        // Utilisez directement sprintf sans strlen pour ajouter le message
        sprintf(resultat, "*");
    }

    // Renvoyer la chaîne résultante
    return resultat;
}

// Fonction pour afficher tous les trains entre deux villes
char *afficheTousTrains(struct Train *listeTrains, int nbTrains, char *villeDepart, char *villeArrivee)
{
    // Allouer de l'espace pour stocker la chaîne résultante
    char *resultat = (char *)malloc(2048 * sizeof(char));

    // Variable pour stocker le nouveau prix du train en fonction des options
    double nouveauPrix = 0;

    // Taux de réduction de 20%
    double reduction = 0.2;

    // Taux d'augmentation de 10%
    double augmentation = 0.1;

    // Vérifier si l'allocation de mémoire a réussi
    if (resultat == NULL)
    {
        perror("Erreur d'allocation de mémoire");
        exit(EXIT_FAILURE);
    }

    // Initialiser la chaîne résultante avec une chaîne vide
    resultat[0] = '\0';

    // Variable pour compter le nombre de trains trouvés
    int trainsTrouves = 0;

    // Parcourir tous les trains dans la liste
    for (int i = 0; i < nbTrains; i++)
    {
        // Vérifier si le train correspond aux villes de départ et d'arrivée spécifiées
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
                nouveauPrix = listeTrains[i].prix * (1 + augmentation);
            }
            else
            {
                // Option non reconnue, utiliser le prix existant
                nouveauPrix = listeTrains[i].prix;
            }

            // Concaténer les informations du train Ã  la chaîne résultante
            sprintf(resultat + strlen(resultat), "%d;%s;%s;%.2lf\n",
                    listeTrains[i].numeroTrain,
                    listeTrains[i].heureDepart, listeTrains[i].heureArrivee,
                    nouveauPrix);

            // Incrémenter le compteur de trains trouvés
            trainsTrouves++;
        }
    }

    // Gestion du cas oÃ¹ aucun train n'est trouvé
    if (trainsTrouves == 0)
    {
        // Utilisez directement sprintf sans strlen pour ajouter le message
        sprintf(resultat, "*");
    }

    // Renvoyer la chaîne résultante
    return resultat;
}

// Structure pour representer un trajet
struct Trajet
{
    int numeroTrain;
    char heureDepart[6];
    char heureArrivee[6];
    double prix;
    int duree;
};

// Fonction pour trouver le meilleur trajet Ã  partir d'une liste de trajets donnée
char *meilleurTrajet1(char *resultat)
{
    // Structure pour stocker les détails du meilleur trajet
    struct Trajet meilleur;
    meilleur.prix = __DBL_MAX__; // Utilisation de DBL_MAX comme valeur maximale en double (comme en Java DOUBLE_MAXVALUE pour trouver le minimum)

    // Utilisation d'une copie de la chaîne pour éviter la modification de l'original
    char *copieResultat = strdup(resultat);
    if (copieResultat == NULL)
    {
        perror("Erreur d'allocation de mémoire");
        exit(EXIT_FAILURE);
    }

    // Alloue de l'espace pour stocker la chaîne résultante
    char *resultatFinal = (char *)malloc(2048 * sizeof(char));
    if (resultatFinal == NULL)
    {
        perror("Erreur d'allocation de mémoire");
        exit(EXIT_FAILURE);
    }

    // Initialiser la chaîne résultante avec une chaîne vide
    resultatFinal[0] = '\0';

    // Définit le séparateur en tant que '\n'
    char *token = strtok(copieResultat, "\n");

    // Tant que la chaîne n'est pas nulle
    while (token != NULL)
    {
        // Structure pour stocker les détails du trajet actuel
        struct Trajet trajet;

        // Le sscanf définit comment la chaîne est formée
        sscanf(token, "%d;%5[^;];%5[^;];%lf", &trajet.numeroTrain, trajet.heureDepart, trajet.heureArrivee, &trajet.prix);

        // Comparaison des prix pour trouver le meilleur trajet
        if (trajet.prix < meilleur.prix)
        {
            meilleur = trajet;
        }

        // Concaténer les informations du meilleur trajet Ã  la chaîne résultante
        sprintf(resultatFinal, "%d;%s;%s;%.2lf\n",
                meilleur.numeroTrain,
                meilleur.heureDepart, meilleur.heureArrivee,
                meilleur.prix);

        token = strtok(NULL, "\n");
    }

    // Libération de la mémoire allouée pour la copie de la chaîne
    free(copieResultat);

    // Renvoie la chaîne résultante contenant le meilleur trajet
    return resultatFinal;
}

// Fonction pour calculer la durée en minutes entre deux heures de départ et d'arrivée
int calculerDuree(const char *heureDepart, const char *heureArrivee)
{
    // Convertir les heures de départ et d'arrivée en minutes depuis minuit
    int departMinutes = heureMinutes(heureDepart);
    int arriveeMinutes = heureMinutes(heureArrivee);

    // Calculer la différence de temps en minutes
    return arriveeMinutes - departMinutes;
}

char *meilleurTrajetDuree(char *resultat)
{
    struct Trajet meilleur;
    meilleur.duree = INT_MAX; // Initialiser avec une valeur maximale pour trouver la durée minimale

    // Utilisation d'une copie de la chaine pour éviter la modification de l'original
    char *copieResultat = strdup(resultat);
    if (copieResultat == NULL)
    {
        perror("Erreur d'allocation de mémoire");
        exit(EXIT_FAILURE);
    }

    // Définit le séprateur en tant que \n
    char *token = strtok(copieResultat, "\n");

    // Tant que la chaine n'est pas nulle
    while (token != NULL)
    {
        struct Trajet trajet;
        // Le sscanf définit comment la chaine est faite
        sscanf(token, "%d;%5[^;];%5[^;];%lf", &trajet.numeroTrain, trajet.heureDepart, trajet.heureArrivee, &trajet.prix);

        // Calcul de la durée du trajet
        trajet.duree = calculerDuree(trajet.heureDepart, trajet.heureArrivee);

        // Comparaison des durée pour trouver le meilleur trajet
        if (trajet.duree < meilleur.duree)
        {
            meilleur = trajet;
        }

        token = strtok(NULL, "\n");
    }

    free(copieResultat);

    // créer une chaine pour le résultat
    char *resultatFinal = (char *)malloc(2048 * sizeof(char));
    if (resultatFinal == NULL)
    {
        perror("Erreur d'allocation de mémoire");
        exit(EXIT_FAILURE);
    }

    // Formatter le résultat en chaine de caractères
    sprintf(resultatFinal, "%d;%s;%s;%.2lf\n",
            meilleur.numeroTrain, meilleur.heureDepart, meilleur.heureArrivee, meilleur.prix);

    return resultatFinal;
}

/*
    void verifierHeure(char chaine[6]) {
        regex_t regex;
        int reti;

        // Compile le motif regex
        reti = regcomp(&regex, "^(2[0-3]|[01]?[0-9]):[0-5][0-9]$", REG_EXTENDED);
        if (reti) {
            fprintf(stderr, "Impossible de compiler le motif regex\n");
            exit(EXIT_FAILURE);
        }

        // Vérifie si la chaîne correspond au motif regex
        reti = regexec(&regex, chaine, 0, NULL, 0);
        if (!reti) {
            printf("La chaîne \"%s\" correspond au motif regex.\n", chaine);
        } else if (reti == REG_NOMATCH) {
            printf("La chaîne \"%s\" ne correspond pas au motif regex.\n", chaine);
        } else {
            char msgbuf[100];
            regerror(reti, &regex, msgbuf, sizeof(msgbuf));
            fprintf(stderr, "Erreur lors de l'exécution de regexec: %s\n", msgbuf);
            exit(EXIT_FAILURE);
        }

        // Libère la mémoire utilisée par la structure regex
        regfree(&regex);
    }

*/