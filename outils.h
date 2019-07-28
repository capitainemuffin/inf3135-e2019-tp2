#include <stdbool.h>
#include <stdio.h>

#ifndef OUTILS_H
#define OUTILS_H

FILE* get_alphabet(char* chemin);
bool isdigits(char* nombre);
int decaler_charactere(int c, int cle, FILE* fichier_alphabet);
#endif


