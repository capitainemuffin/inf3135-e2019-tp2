#include <dirent.h>
#include "structure.h"

Arguments_t *initArguments() {

    Arguments_t *arguments = malloc(sizeof(Arguments_t));

    arguments->mode.present = false;
    arguments->cle.present = false;
    arguments->entree = stdin;
    arguments->sortie = stdout;
    arguments->alphabet = NULL;
    arguments->code_perm = NULL;
    arguments->dictionnaires = NULL;
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

    if (arguments->mode.action != BRUTEFORCE && arguments->dictionnaires != NULL) {
        freeArguments(arguments);
        exit(9);
    }

    if (arguments->mode.action == BRUTEFORCE && arguments->dictionnaires == NULL) {
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

Dictionnaire_t *initDictionnaire(const char *repertoire, const char *nom_fichier) {

    FILE *fichier;
    Dictionnaire_t *dict = NULL;

    //taille minimale est 4
    if (strlen(nom_fichier) < 4) return NULL;
    if ((fichier = fopen(repertoire, "r")) == NULL) return NULL;

    dict = malloc(sizeof(Dictionnaire_t));
    dict->nbr_mots = 0;

    //doit terminer par .fr, .en. .de
    if (strcmp(nom_fichier + strlen(nom_fichier) - 3, ".fr") == 0) {
        dict->langue = FRANCAIS;
    } else if (strcmp(nom_fichier + strlen(nom_fichier) - 3, ".en") == 0) {
        dict->langue = ANGLAIS;
    } else if (strcmp(nom_fichier + strlen(nom_fichier) - 3, ".de") == 0) {
        dict->langue = ALLEMAND;
    } else {
        return NULL;
    }

    int lettre;
    while ((lettre = fgetc(fichier)) != EOF) {
        if (lettre == ' ' || lettre == '\n' ) dict->nbr_mots++;
    }

    dict->mots = (char **) malloc(dict->nbr_mots * sizeof(char *));
    fseek(fichier, 0, SEEK_SET);
    char tmp[500];

    int i = 0;
    while (EOF != fscanf(fichier, "%[^\n]\n", tmp)) {

        char *token = strtok(tmp, " \n\t");
        while (token != NULL) {
            dict->mots[i] = (char *) malloc(45 * sizeof(char));
            dict->mots[i][0] = '\0';
            strcpy(dict->mots[i], token);
            i++;
            token = strtok(NULL, " \n\t");
        }

    }

    fclose(fichier);
    return dict;

}

void freeDictionnaire(Dictionnaire_t *dict) {

    for (unsigned long i = 0; i < dict->nbr_mots; i++) {
        if (dict->mots[i]) free(dict->mots[i]);
    }

    if (dict->mots) free(dict->mots);
    free(dict);
}

Dictionnaires_t *initDictionnaires(char *chemin) {

    Dictionnaires_t *dicts = malloc(sizeof(Dictionnaires_t));
    dicts->nbr_dictionnaires = 0;
    dicts->dictionnaires = malloc(sizeof(Dictionnaire_t *) * 10);
    dicts->capacite = 10;
    dicts->francais = false;
    dicts->anglais = false;
    dicts->allemand = false;

    DIR *repertoire;
    struct dirent *dir;
    repertoire = opendir(chemin);

    if (repertoire == NULL) return NULL;

    int nbr_fichiers = 0;
    while ((dir = readdir(repertoire)) != NULL) {

        if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {

            nbr_fichiers++;
            char chemin_complet[300];
            chemin_complet[0] = '\0';
            strcpy(chemin_complet, chemin);
            if (chemin[strlen(chemin) - 1] != '/') strcat(chemin_complet, "/");
            strcat(chemin_complet, dir->d_name);

            Dictionnaire_t *dictionnaire;
            if ((dictionnaire = initDictionnaire(chemin_complet, dir->d_name)) == NULL) return NULL;
            if ((dicts = ajouter_dictionnaire(dicts, dictionnaire)) == NULL) return NULL;

        }
    }

    if (nbr_fichiers == 0) return NULL;
    closedir(repertoire);
    return dicts;
}

Dictionnaires_t *ajouter_dictionnaire(Dictionnaires_t *dicts, Dictionnaire_t *dico1) {

    if (dicts->capacite == dicts->nbr_dictionnaires) {

        if (realloc(dicts->dictionnaires, dicts->capacite * 2)) {

            dicts->capacite *= 2;

        } else {

            return NULL;

        }
    }

    switch (dico1->langue) {
        case FRANCAIS :
            dicts->francais = true;
            break;
        case ANGLAIS :
            dicts->anglais = true;
            break;
        case ALLEMAND :
            dicts->allemand = true;
            break;
    }

    dicts->dictionnaires[dicts->nbr_dictionnaires] = dico1;
    dicts->nbr_dictionnaires++;
    return dicts;

}

void freeDictionnaires(Dictionnaires_t *dicts) {

    if (dicts) {
        for (int i = 0; i < dicts->nbr_dictionnaires; i++) {
            if (dicts->dictionnaires[i]) {
                freeDictionnaire(dicts->dictionnaires[i]);
            }
        }

        if (dicts->dictionnaires) free(dicts->dictionnaires);
        free(dicts);
    }

}
