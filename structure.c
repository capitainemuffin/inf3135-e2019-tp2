#include "structure.h"

Arguments_t *initArguments() {

    Arguments_t *arguments = malloc(sizeof(Arguments_t));

    arguments->mode.present = false;
    arguments->cle.present = false;
    arguments->entree = stdin;
    arguments->sortie = stdout;
    arguments->alphabet = NULL;
    arguments->code_perm = NULL;
    arguments->dictionnaires = initDictionnaires();

    return arguments;
}

void valider_arguments(Arguments_t *arguments) {

    if (arguments->code_perm == NULL) {

        fprintf(stderr,
                "Usage: %s <-c CODEpermanent> <-d | -e> <-k valeur> [-i fichier.in] [-o fichier.out] [-a chemin]\n",
                arguments->programme);
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

void freeArguments(Arguments_t *arguments) {

    if (arguments->entree) fclose(arguments->entree);
    if (arguments->sortie) fclose(arguments->sortie);
    if (arguments->alphabet) fclose(arguments->alphabet);
    if (arguments->dictionnaires) freeDictionnaires(arguments->dictionnaires);
    free(arguments);

}

Dictionnaire_t *initDictionnaire(const char *nom_fichier) {

    FILE *fichier;
    Dictionnaire_t *dict = NULL;

    //taille minimale est 4
    if (strlen(nom_fichier) < 4) return NULL;
    if ((fichier = fopen(nom_fichier, "r")) != NULL) {

        dict = malloc(sizeof(Dictionnaire_t));
        dict->nbr_mots = 0;

        //doit terminer par .fr, .en. .de
        if (strcmp(&nom_fichier[strlen(nom_fichier - 3)], ".fr") == 0) {
            dict->langue = FRANCAIS;
        } else if (strcmp(&nom_fichier[strlen(nom_fichier - 3)], ".en") == 0) {
            dict->langue = ANGLAIS;
        } else if (strcmp(&nom_fichier[strlen(nom_fichier - 3)], ".de") == 0) {
            dict->langue = ALLEMAND;
        } else {
            return NULL;
        }

        int lettre;
        while ((lettre = fgetc(fichier)) != EOF) {
            if (lettre == ' ' || lettre == '\n') dict->nbr_mots++;
        }

        dict->mots = (char **) malloc(dict->nbr_mots * sizeof(char *));
        fseek(fichier, 0, SEEK_SET);
        char tmp[45];

        while (EOF != fscanf(fichier, "%[^\n]\n", tmp)) {

            char *token = strtok(tmp, " ");
            int i = 0;
            while (token != NULL) {

                dict->mots[i] = (char *) malloc(45 * sizeof(char));
                dict->mots[i][0] = '\0';
                strncpy(dict->mots[i], token, strlen(token));
                i++;
                token = strtok(NULL, " ");
            }

        }

        fclose(fichier);

    }

    return dict;

}

void freeDictionnaire(Dictionnaire_t *dict) {

    for (unsigned long i = 0; i < dict->nbr_mots; i++) {
        if (dict->mots[i]) free(dict->mots[i]);
    }

    if (dict->mots) free(dict->mots);
    free(dict);
}

Dictionnaires_t *initDictionnaires() {

    Dictionnaires_t *dict = malloc(sizeof(Dictionnaires_t));
    dict->nbr_dictionnaires = 0;
    dict->dictionnaires = malloc(sizeof(Dictionnaire_t *) * 10);
    dict->capacite = 10;
    dict->francais = false;
    dict->anglais = false;
    dict->allemand = false;

    return dict;
}

Dictionnaires_t *ajouter_dictionnaire(Dictionnaires_t *dicts, Dictionnaire_t *dico1) {

    if (dicts->capacite == dicts->nbr_dictionnaires) {

        if (realloc(dicts->dictionnaires, dicts->capacite * 2)) {

            dicts->capacite *= 2;

        } else {

            return NULL;

        }
    }

    dicts->dictionnaires[dicts->nbr_dictionnaires] = dico1;
    dicts->nbr_dictionnaires++;
    return dicts;

}


void freeDictionnaires(Dictionnaires_t *dicts) {
    for (int i = 0; i < dicts->nbr_dictionnaires; i++) {
        if (dicts->dictionnaires[i]) {
            freeDictionnaire(dicts->dictionnaires[i]);
            free(dicts->dictionnaires[i]);
        }
    }

    if (dicts->dictionnaires) free(dicts->dictionnaires);
    free(dicts);
}
