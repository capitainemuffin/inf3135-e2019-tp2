#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "structure.h"
#include "outils.h"

void validation_args(int argc, char *argv[], Arguments_t *arguments) {

    for (int i = 1; i < argc; i++) {

#ifdef DEBUG
        printf("%s\n",argv[i]);
#endif

        if (argv[i][0] == '-' && argv[i][2] == '\0') {
            switch (argv[i][1]) {

                case 'c': {

                    if (i + 1 < argc && strlen(argv[i + 1]) == 12) {
                        arguments->code_perm = argv[i + 1];
                    } else {
                        freeArguments(arguments);
                        exit(2);
                    }
                    i++;
                    break;
                }
                case 'i' : {

                    if (i + 1 < argc) arguments->entree = fopen(argv[i + 1], "r");
                    if (i + 1 >= argc || arguments->entree == NULL) {
                        freeArguments(arguments);
                        exit(5);
                    }
                    i++;
                    break;
                }
                case 'o' : {

                    if (i + 1 < argc) arguments->sortie = fopen(argv[i + 1], "w");
                    if (i + 1 >= argc || arguments->sortie == NULL) {
                        freeArguments(arguments);
                        exit(6);
                    }
                    i++;
                    break;
                }
                case 'd' : {

                    if (arguments->mode.present) {
                        freeArguments(arguments);
                        exit(9);
                    }
                    arguments->mode.present = true;
                    arguments->mode.action = DECRYPT;
                    break;
                }
                case 'e' : {

                    if (arguments->mode.present) {
                        freeArguments(arguments);
                        exit(9);
                    }
                    arguments->mode.present = true;
                    arguments->mode.action = ENCRYPT;
                    break;
                }
                case 'k' : {

                    if (i + 1 < argc) {

                        arguments->cle.present = true;
                        char *ptr = argv[i + 1];
                        bool negatif = false;

                        for (unsigned long j = 0; j < strlen(argv[i + 1]); j++) {

                            if (ptr[j] == '-') {

                                if (negatif) {

                                    arguments->cle.present = false;
                                    break;

                                } else {

                                    negatif = true;

                                }

                            } else if (!isdigit(ptr[j])) {

                                arguments->cle.present = false;
                                break;

                            }

                        }

                        if (arguments->cle.present) {

                            arguments->cle.cle = atoi(argv[i + 1]);

                        }
                        i++;
                    } else {
                        freeArguments(arguments);
                        exit(7);
                    }
                    break;
                }
                case 'a' : {

                    if (i + 1 < argc) {

                        arguments->alphabet = get_alphabet(argv[i + 1]);
                        i++;
                    }

                    break;
                }
                case 'b' : {

                    if (arguments->mode.present) {
                        freeArguments(arguments);
                        exit(9);
                    }

                    arguments->mode.present = true;
                    arguments->mode.action = BRUTEFORCE;
                    break;
                }
                case 'l' : {


                    // pour tous les fichiers du répertoire
                    // tester si le dictionnaire s'est bien ouvert
                    // tester si l'ajout s'est fait
                    i++;
                    break;
                }
                default : {
                    freeArguments(arguments);
                    exit(3);
                }
            }

        } else {
            freeArguments(arguments);
            exit(3);
        }

    }

    if (arguments->code_perm == NULL) {

        fprintf(stderr,
                "Usage: %s <-c CODEpermanent> <-d | -e> <-k valeur> [-i fichier.in] [-o fichier.out] [-a chemin]\n",
                argv[0]);
        freeArguments(arguments);
        exit(1);

    }

    if (!arguments->mode.present) {
        freeArguments(arguments);
        exit(4);
    }


    if (arguments->mode.action != BRUTEFORCE && !arguments->cle.present) {
        freeArguments(arguments);
        exit(7);

    }

    if (arguments->alphabet == NULL) {
        freeArguments(arguments);
        exit(8);
    }

    if (arguments->mode.action != BRUTEFORCE && arguments->dictionnaires->nbr_dictionnaires > 0) {
        freeArguments(arguments);
        exit(9);
    }

    if (arguments->mode.action == BRUTEFORCE && arguments->dictionnaires->nbr_dictionnaires == 0) {
        freeArguments(arguments);
        exit(9);
    }

    if (arguments->mode.action == BRUTEFORCE && arguments->cle.present) {
        freeArguments(arguments);
        exit(9);
    }

}

int main(int argc, char **argv) {

    Arguments_t *arguments = initArguments();
    arguments->alphabet = fopen("alphabet.txt", "r");

    validation_args(argc, argv, arguments);
    fseek(arguments->alphabet, -1, SEEK_END);

    if (fgetc(arguments->alphabet) == '\n') {

        fseek(arguments->alphabet, -2, SEEK_END);

    } else {

        fseek(arguments->alphabet, -1, SEEK_END);

    }

    long taille = ftell(arguments->alphabet);

    if (arguments->mode.action == DECRYPT) {

        arguments->cle.cle = -(arguments->cle.cle);

    }

    int old;
    int new;

    while ((old = fgetc(arguments->entree)) != EOF) {

        int alphabet;
        bool trouve = false;
        fseek(arguments->alphabet, 0, SEEK_SET);

        while ((alphabet = fgetc(arguments->alphabet)) != EOF && alphabet != '\n') {

            if (alphabet == old) {

                trouve = true;
                fseek(arguments->alphabet, -1, SEEK_CUR);
                break;

            }

        }

        if (trouve) {
            int i = 0;
            if (arguments->cle.cle < 0) {

                while (i > arguments->cle.cle) {

                    if (ftell(arguments->alphabet) == 0) {

                        fseek(arguments->alphabet, -1, SEEK_END);

                        if (fgetc(arguments->alphabet) == '\n') {

                            fseek(arguments->alphabet, -2, SEEK_CUR);

                        } else {

                            fseek(arguments->alphabet, -1, SEEK_CUR);

                        }

                    } else {

                        fseek(arguments->alphabet, -1, SEEK_CUR);

                    }

                    i--;
                }

            } else {

                while (i < arguments->cle.cle) {

                    if (ftell(arguments->alphabet) == taille) {

                        fseek(arguments->alphabet, 0, SEEK_SET);

                    } else {

                        fseek(arguments->alphabet, 1, SEEK_CUR);
                    }

                    i++;
                }

            }

            new = fgetc(arguments->alphabet);

        } else {

            new = old;

        }

        fprintf(arguments->sortie, "%c", new);

    }

    freeArguments(arguments);
    return 0;
}
