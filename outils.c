#include "outils.h"
#include <string.h>

FILE* get_alphabet(char* chemin){

    char chemin_complet [100];
    chemin_complet[0] = '\0';
    strcpy(chemin_complet, chemin);

    if (chemin[strlen(chemin) - 1] != '/') {
        strcat(chemin_complet, "/alphabet.txt");
        return fopen(chemin_complet, "r");

    } else {
        strcat(chemin_complet, "alphabet.txt");
        return fopen(chemin_complet, "r");
    }

}
