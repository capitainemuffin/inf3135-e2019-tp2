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

    //ajouter option -b (bruteforce) et -l (repertoire des dictionnaires)
    // code de retour 9 (les options obligatoires ne sont pas (toutes) présentes ou mal utilisées;)
    for (int i = 1; i < argc; i++) {

        #ifdef DEBUG
        printf('%s\n', argv[i]);
        #endif

        if(argv[i][0] == '-' && argv[i][2] == '\0'){

            switch(argv[i][1]){

                case 'c': {

                    if (i + 1 < argc && strlen(argv[i + 1]) == 12) {
                        arguments->code_perm = argv[i + 1];
                        i++;                    
                    } else { exit(2); } 
                    break;

                }

                case 'i' : {

                    if (i + 1 < argc) arguments->entree = fopen(argv[i + 1], "r");
                    if (i + 1 >= argc || arguments->entree == NULL) exit(5);
                    i++;
                    break;
                    
                }

                case 'o' : {

                    if (i + 1 < argc) arguments->sortie = fopen(argv[i + 1], "w");
                    if (i + 1 >= argc || arguments->sortie == NULL) exit(6);
                    i++;
                    break;

                }

                case 'd' : {

                    if(arguments->mode.present) exit(11);
                    arguments->mode.present = true;
                    arguments->mode.action = DECRYPT;
                    break;

                }

                case 'e' : {

                    if(arguments->mode.present) exit(11);
                    arguments->mode.present = true;
                    arguments->mode.action = ENCRYPT;
                    break;

                }

                case 'k' : {

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
                    
                    break;                

                }

                case 'a' : {

                    if (i + 1 < argc) {

                        char *chemin;
                        if (argv[i + 1][strlen(argv[i + 1]) - 1] != '/') {

                            chemin = malloc(strlen(argv[i + 1]) + strlen("/alphabet.txt") + 1);
                            strcpy(chemin, argv[i + 1]);
                            strcat(chemin, "/alphabet.txt");
                            arguments->alphabet = fopen(chemin, "r");

                        } else {

                            chemin = malloc(strlen(argv[i + 1]) + strlen("alphabet.txt") + 1);
                            strcpy(chemin, argv[i + 1]);
                            strcat(chemin, "alphabet.txt");
                            arguments->alphabet = fopen(chemin, "r");
                            
                        }

                        free(chemin);
                        i++;
                    }

                    break;

                }

                default : exit(3);

            }

        } else { exit(3); }

    } 

    /**
    * Pas de -C ou aucun argument -> code 1 
    **/
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

    while ((old = fgetc(arguments.entree)) != EOF) {

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

    fclose(arguments.entree);
    fclose(arguments.sortie);
    fclose(arguments.alphabet);

    return 0;
}
