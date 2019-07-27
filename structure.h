/**
* Interface des structures de données
**/ 

#ifndef STRUCTURE_H
#define STRUCTURE_H

typedef enum {

    ENCRYPT, DECRYPT

} Action_s;

typedef struct Mode_s {

    bool present;
    Action_s action;

} Mode_t;

typedef struct Cle_s {

    bool present;
    int cle;

} Cle_t;

typedef struct Arguments_s{
    
    char *code_perm;
    Cle_t cle;
    Mode_t mode;
    FILE *alphabet;
    FILE *entree;
    FILE *sortie;

} Arguments_t;

#endif
