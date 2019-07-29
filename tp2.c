#include <ctype.h>
#include "structure.h"
#include "outils.h"

void bruteforce(Arguments_t *arguments) {

    int nbr_cle = (int) get_taille_alphabet(arguments->alphabet) + 1;
    int score_max = 0;
    char *message_a_decoder = malloc(sizeof(char) * 3);
    message_a_decoder[0] = '\0';

    int old;
    int taille_message = 0;
    while ((old = fgetc(arguments->entree)) != EOF && (arguments->entree != stdin || old != '\n')) {
        taille_message++;
        char *tmp = malloc(sizeof(char) * 2);
        tmp[1] = '\0';
        tmp[0] = (char) old;
        message_a_decoder = realloc(message_a_decoder,sizeof(char) * taille_message + 2);
        strcat(message_a_decoder, tmp);
        free(tmp);
    }

    char *message_trouve = malloc(sizeof(char) * taille_message + 2);
    message_trouve[0] = '\0';

    for (int cle = 0; cle < nbr_cle; cle++) {

        int score = 0;
        char *tmp = malloc(sizeof(char) * taille_message + 2);
        tmp[0] = '\0';
        for (int i = 0; i < taille_message; i++) {
            char *tmp2 = malloc(sizeof(char) * 2);
            tmp2[1] = '\0';
            tmp2[0] = (char) decaler_charactere(message_a_decoder[i], cle, arguments->alphabet);
            strcat(tmp, tmp2);
            free(tmp2);
        }

        char *message_decoder = malloc(sizeof(char) * taille_message + 2);
        message_decoder[0] = '\0';
        strcpy(message_decoder, tmp);

        char *token = strtok(tmp, " \n\t");
        while (token != NULL) {
            for (int i = 0; i < arguments->dictionnaires->nbr_dictionnaires; i++) {
                // pour chaque dictionnaire
                for (unsigned long j = 0; j < arguments->dictionnaires->dictionnaires[i]->nbr_mots; j++) {
                    //pour chaque mot du dictionnaire
                    if (arguments->dictionnaires->dictionnaires[i]->mots[j]) {
                        //pcque seg fault
                        if (strcmp(arguments->dictionnaires->dictionnaires[i]->mots[j], token) == 0) score++;
                    }
                }
            }
            token = strtok(NULL, " \n\t");
        }

        if (score_max < score) {
            score_max = score;
            strcpy(message_trouve, message_decoder);

#ifdef DEBUG
        printf("Score max : %d\n", score_max);
        printf("Meilleur message pour le moment : %s\n", message_trouve);
#endif
        }
        free(message_decoder);
        free(tmp);
    }

    if (score_max == 0) {
        freeArguments(arguments);
        exit(50);
    }

    fprintf(arguments->sortie, "%s", message_trouve);
    free(message_a_decoder);
    free(message_trouve);

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
                    if(arguments->alphabet) fclose(arguments->alphabet);
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
                    exit(9);
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

    if (arguments->mode.action == DECRYPT || arguments->mode.action == ENCRYPT) {

        if (arguments->mode.action == DECRYPT) {

            arguments->cle.cle = -(arguments->cle.cle);

        }

        int old;
        while ((old = fgetc(arguments->entree)) != EOF) {

            int new = decaler_charactere(old, arguments->cle.cle, arguments->alphabet);
            fprintf(arguments->sortie, "%c", new);

        }

    } else {

        bruteforce(arguments);

    }

    freeArguments(arguments);
    return 0;
}
