#include "outils.h"
#include <string.h>
#include <ctype.h>

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

bool isdigits(char* nombre){

    int i = 0;
    if(nombre[0] == '-') i++;

    while(i < strlen(nombre)){
        if (!isdigit(nombre[i])) return false;
        i++;
    }
    return true;
}






