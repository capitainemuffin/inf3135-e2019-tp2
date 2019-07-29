#include "outils.h"
#include <string.h>
#include <ctype.h>

FILE *get_alphabet(char *chemin) {

    char chemin_complet[100];
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

unsigned long get_taille_fichier(FILE* fichier){

    fseek(fichier, -1, SEEK_END);
    if (fgetc(fichier) == '\n') fseek(fichier, -1, SEEK_CUR);
    fseek(fichier, -1, SEEK_CUR);

    return ftell(fichier);
}

bool isdigits(char *nombre) {

    int i = 0;
    if (nombre[0] == '-') i++;

    while (i < strlen(nombre)) {
        if (!isdigit(nombre[i])) return false;
        i++;
    }
    return true;
}

int decaler_charactere(int old, int cle, FILE *fichier_alphabet) {

    unsigned long taille = get_taille_fichier(fichier_alphabet);
    int alphabet;
    bool trouve = false;
    int new;

    fseek(fichier_alphabet, 0, SEEK_SET);
    while ((alphabet = fgetc(fichier_alphabet)) != EOF && alphabet != '\n') {

        if (alphabet == old) {

            trouve = true;
            fseek(fichier_alphabet, -1, SEEK_CUR);
            break;

        }

    }

    if (trouve) {
        int i = 0;
        if (cle < 0) {

            while (i > cle) {

                if (ftell(fichier_alphabet) == 0) {

                    fseek(fichier_alphabet, -1, SEEK_END);

                    if (fgetc(fichier_alphabet) == '\n') {

                        fseek(fichier_alphabet, -2, SEEK_CUR);

                    } else {

                        fseek(fichier_alphabet, -1, SEEK_CUR);

                    }

                } else {

                    fseek(fichier_alphabet, -1, SEEK_CUR);

                }

                i--;
            }

        } else {

            while (i < cle) {

                if (ftell(fichier_alphabet) == taille) {

                    fseek(fichier_alphabet, 0, SEEK_SET);

                } else {

                    fseek(fichier_alphabet, 1, SEEK_CUR);
                }

                i++;
            }

        }

        new = fgetc(fichier_alphabet);

    } else {

        new = old;

    }

    return new;
}






