#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

typedef enum {

    ENCRYPT, DECRYPT

} Action;

typedef struct {

    bool present;
    Action action;

} Mode;

typedef struct {

    bool present;
    int cle;

} Cle;

typedef struct {
    
    char *code_perm;
    Cle cle;
    Mode mode;
    FILE *alphabet;
    FILE *entree;
    FILE *sortie;

} Arguments;

void validation_args(int argc, char *argv[], Arguments *arguments) {

    for (int i = 1; i < argc; i++) {

        if (strcmp(argv[i], "-c") == 0) {

            if (i + 1 < argc) {

                arguments->code_perm = argv[i + 1];

            }

            if (i + 1 >= argc || strlen(arguments->code_perm) != 12) exit(2);
            
            i++;

        } else if (strcmp(argv[i], "-i") == 0) {

            if (i + 1 < argc) {

                arguments->entree = fopen(argv[i + 1], "r");

            }

            if (i + 1 >= argc || arguments->entree == NULL) exit(5);
            

            i++;

        } else if (strcmp(argv[i], "-o") == 0) {

            if (i + 1 < argc) {

                arguments->sortie = fopen(argv[i + 1], "w");

            }

            if (i + 1 >= argc || arguments->sortie == NULL) exit(6);

            i++;

        } else if (strcmp(argv[i], "-d") == 0) {

            arguments->mode.present = true;
            arguments->mode.action = DECRYPT;

        } else if (strcmp(argv[i], "-e") == 0) {

            arguments->mode.present = true;
            arguments->mode.action = ENCRYPT;

        } else if (strcmp(argv[i], "-k") == 0) {

            if (i + 1 < argc) {

                arguments->cle.present = true;
                char *ptr = argv[i + 1];
                bool negatif = false;

                for (int j = 0; j < strlen(argv[i + 1]); j++) {

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

            }

        } else if (strcmp(argv[i], "-a") == 0) {

            if (i + 1 < argc) {

                if (argv[i + 1][strlen(argv[i + 1]) - 1] != '/') {

                    arguments->alphabet = fopen(argv[i + 1], "r");

                } else {

                    char *chemin = malloc(strlen(argv[i + 1]) + strlen("alphabet.txt") + 1);
                    strcpy(chemin, argv[i + 1]);
                    strcat(chemin, "alphabet.txt");
                    arguments->alphabet = fopen(chemin, "r");
                    free(chemin);

                }

                i++;
            }

        } else {

            exit(3);

        }

    }

    if (arguments->code_perm == NULL) {

        fprintf(stderr, "Usage: %s <-c CODEpermanent> <-d | -e> <-k valeur> [-i fichier.in] [-o fichier.out] [-a chemin]\n",
               argv[0]);
        exit(1);

    }

    if (!arguments->mode.present) exit(4);
    if (!arguments->cle.present) exit(7);
    if (arguments->alphabet == NULL) exit(8);
    
}

int main(int argc, char **argv) {

    Arguments arguments;
    arguments.mode.present = false;
    arguments.cle.present = false;
    arguments.entree = stdin;
    arguments.sortie = stdout;
    arguments.alphabet = fopen("alphabet.txt", "r");
    arguments.code_perm = NULL;

    validation_args(argc, argv, &arguments);
    fseek(arguments.alphabet, -1, SEEK_END);

    if (fgetc(arguments.alphabet) == '\n') {

        fseek(arguments.alphabet, -2, SEEK_END);

    } else {

        fseek(arguments.alphabet, -1, SEEK_END);

    }

    int taille = ftell(arguments.alphabet);

    if (arguments.mode.action == DECRYPT) {

        arguments.cle.cle = -(arguments.cle.cle);

    }

    char old;
    char new;

    while ((old = fgetc(arguments.entree)) != EOF && old != '\n') {

        char alphabet;
        bool trouve = false;
        fseek(arguments.alphabet, 0, SEEK_SET);

        while ((alphabet = fgetc(arguments.alphabet)) != EOF && alphabet != '\n') {

            if (alphabet == old) {

                trouve = true;
                fseek(arguments.alphabet, -1, SEEK_CUR);
                break;

            }

        }

        if (trouve) {
            int i = 0;
            if (arguments.cle.cle < 0) {

                while (i > arguments.cle.cle) {

                    if (ftell(arguments.alphabet) == 0) {

                        fseek(arguments.alphabet, -1, SEEK_END);

                        if (fgetc(arguments.alphabet) == '\n') {

                            fseek(arguments.alphabet, -2, SEEK_CUR);

                        } else {

                            fseek(arguments.alphabet, -1, SEEK_CUR);

                        }

                    } else {

                        fseek(arguments.alphabet, -1, SEEK_CUR);

                    }

                    i--;
                }

            } else {

                while (i < arguments.cle.cle) {

                    if (ftell(arguments.alphabet) == taille) {

                        fseek(arguments.alphabet, 0, SEEK_SET);

                    } else {

                        fseek(arguments.alphabet, 1, SEEK_CUR);
                    }

                    i++;
                }

            }

            new = fgetc(arguments.alphabet);

        } else {

            new = old;

        }

        fprintf(arguments.sortie, "%c", new);

    }

    fprintf(arguments.sortie, "\n");
    fclose(arguments.entree);
    fclose(arguments.sortie);
    fclose(arguments.alphabet);

    return 0;
}
