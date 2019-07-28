#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/**
* Interface des structures de données
**/ 

#ifndef STRUCTURE_H
#define STRUCTURE_H

typedef enum Langue_s{

    FRANCAIS, ANGLAIS, ALLEMAND

} Langue_t;

typedef enum Action_s{

    ENCRYPT, DECRYPT, BRUTEFORCE

} Action_t;

typedef struct Mode_s {

    bool present;
    Action_t action;

} Mode_t;

typedef struct Cle_s {

    bool present;
    int cle;

} Cle_t;

typedef struct Dictionnaire_s {

    Langue_t langue;
    unsigned long nbr_mots;
    char** mots;

} Dictionnaire_t ;

typedef struct Dictionnaires_s {

    int nbr_dictionnaires;
    int capacite;
    Dictionnaire_t** dictionnaires;

} Dictionnaires_t;

typedef struct Arguments_s{

    char *code_perm;
    Cle_t cle;
    Mode_t mode;
    FILE *alphabet;
    FILE *entree;
    FILE *sortie;
    Dictionnaires_t* dictionnaires;

} Arguments_t;

/**
 * Méthodes pour les Arguments
 */
Arguments_t* initArguments();
void freeArguments(Arguments_t* arguments);

/**
 * Méthode pour une liste de dictionnaires
 */
Dictionnaires_t* initDictionnaires();
void freeDictionnaires(Dictionnaires_t* dicts);
Dictionnaires_t* ajouter_dictionnaire(Dictionnaires_t* dicts, Dictionnaire_t* dico1);

/**
 * Méthodes pour un dictionnaire
 */
Dictionnaire_t* initDictionnaire(const char* nom_fichier);
Dictionnaire_t* ajouter_mots(Dictionnaire_t* dict);
void freeDictionnaire(Dictionnaire_t* dict);



#endif
