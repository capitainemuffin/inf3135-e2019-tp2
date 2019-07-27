#include "structure.h"

Arguments_t* initArguments() {

    Arguments_t* arguments = malloc(sizeof(Arguments_t));

    arguments->mode.present = false;
    arguments->cle.present = false;
    arguments->entree = stdin;
    arguments->sortie = stdout;
    arguments->alphabet = NULL;
    arguments->code_perm = NULL;
    arguments->dictionnaires = initDictionnaires();

    return arguments;
}

void freeArguments(Arguments_t* arguments){

    if(arguments->entree) fclose(arguments->entree);
    if(arguments->sortie) fclose(arguments->sortie);
    if(arguments->alphabet) fclose(arguments->alphabet);

    free(arguments);

}

Dictionnaire_t* initDictionnaire(char* fichier){

}

Dictionnaire_t* ajouter_mots(Dictionnaire_t* dict){

    return dict;
}

void freeDictionnaire(Dictionnaire_t* dict){

    for(int i = 0 ; i < dict->nbr_mots; i++) {
        free (dict->mots[i]);
    }
    free(dict);
}

Dictionnaires_t* initDictionnaires(){
    Dictionnaires_t* dict = malloc(sizeof(Dictionnaires_t));
    dict->nbr_dictionnaires = 0;
    dict->dictionnaires = malloc(sizeof(Dictionnaire_t*) * 10);
    dict->capacite = 10;

    return dict;
}

Dictionnaires_t* ajouter_dictionnaire(Dictionnaires_t* dicts, Dictionnaire_t* dico1){

    if(dicts->capacite == dicts->nbr_dictionnaires){

        if(realloc(dicts->dictionnaires, dicts->capacite *2)){

            dicts->capacite*=2;

        } else {

            return NULL;

        }
    }

    dicts->dictionnaires[dicts->nbr_dictionnaires] = dico1;
    dicts->nbr_dictionnaires++;
    return dicts;

}


void freeDictionnaires(Dictionnaires_t* dicts){
    for(int i = 0 ; i < dicts->nbr_dictionnaires; i++){
        free(dicts->dictionnaires[i]);
    }
    free(dicts);
}