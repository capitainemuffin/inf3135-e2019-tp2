#include <ctype.h>
#include "structure.h"
#include "outils.h"

void bruteforce(Arguments_t* arguments){

}

void traitement_arguments(int argc, char **argv, Arguments_t *arguments) {

    arguments->programme = argv[0];

    for (int i = 1; i < argc; i++) {

#ifdef DEBUG
        printf("%s\n",argv[i]);
#endif

        if (argv[i][0] != '-' && argv[i][2] != '\0') {
            freeArguments(arguments);
            exit(3);
        }

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

                if (i + 1 < argc && isdigits(argv[i + 1])) {

                    arguments->cle.present = true;
                    arguments->cle.cle = atoi(argv[i + 1]);
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
                } else {
                    freeArguments(arguments);
                    exit(8);
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

                    if ((arguments->dictionnaires = initDictionnaires(argv[i + 1])) == NULL) {
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

    }
}

int main(int argc, char **argv) {

    Arguments_t *arguments = initArguments();
    arguments->alphabet = fopen("alphabet.txt", "r");
    traitement_arguments(argc, argv, arguments);
    valider_arguments(arguments);

    if(arguments->mode.action == DECRYPT || arguments->mode.action == ENCRYPT){

            if (arguments->mode.action == DECRYPT) {

                arguments->cle.cle = -(arguments->cle.cle);

            }

            int old;
            while ((old = fgetc(arguments->entree)) != EOF) {

                int new = decaler_charactere(old, arguments->cle.cle,arguments->alphabet);
                fprintf(arguments->sortie, "%c", new);

            }

    } else {

        bruteforce(arguments);

    }

    freeArguments(arguments);
    return 0;
}
