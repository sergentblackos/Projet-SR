#ifndef FONCTIONNALITE_H
#define FONCTIONNALITE_H

#include "parser.h"

char* rechercheTrainVilleDepartArriveeHeure(struct Train *listeTrains, int nbTrains, char* villeDepart, char* villeArrivee, char* heureDepart);
char* afficherTrainsPlageHoraires(struct Train *listeTrains, int nbTrains, char* villeDepart, char* villeArrivee, char* plageDebut, char* plageFin);
char* afficheTousTrains(struct Train *listeTrains, int nbTrains, char* villeDepart, char* villeArrivee);
char* meilleurTrajet1(char* resultat);
char* meilleurTrajetDuree(char* resultat);

#endif /* FONCTIONNALITE_H */