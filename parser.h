#ifndef PARSER_H
#define PARSER_H

struct Train {
    int numeroTrain;
    char villeDepart[50];
    char villeArrivee[50];
    char heureDepart[50];
    char heureArrivee[50];
    double prix;
    char option[50];
};

struct TrainData {
    struct Train *listeTrains;
    int nbTrains;
};

struct TrainData parseur();

#endif /* PARSER_H */
