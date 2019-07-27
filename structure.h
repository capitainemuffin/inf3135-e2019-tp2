#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

/**
* Interface des structures de données
**/ 

#ifndef STRUCTURE_H
#define STRUCTURE_H

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

    int nbr_mots;
    char* mots[];

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

/**
 * Méthodes pour un dictionnaire
 */
Dictionnaire_t* initDictionnaire();
Dictionnaire_t* ajouter_mots(Dictionnaire_t* dict);
void freeDictionnaire(Dictionnaire_t* dict);



#endif
