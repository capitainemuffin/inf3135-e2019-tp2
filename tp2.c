#include <ctype.h>
#include "structure.h"
#include "outils.h"

void traitement_arguments(int argc, char **argv, Arguments_t *arguments) {

    arguments->programme = argv[0];

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

                    if (i + 1 < argc) {

                        if((arguments->dictionnaires = initDictionnaires(argv[i + 1])) == NULL){
                            freeArguments(arguments);
                            exit(12);
                        }

                    } else {

                        freeArguments(arguments);
                        exit(11);
                    }
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
}

int main(int argc, char **argv) {

    Arguments_t *arguments = initArguments();
    arguments->alphabet = fopen("alphabet.txt", "r");

    traitement_arguments(argc, argv, arguments);
    valider_arguments(arguments);

    // si c'est l'option encrypter ou decrypter

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
