#include "structure.h"

Arguments_t* initArguments() {

    Arguments_t* arguments = malloc(sizeof(Arguments_t));

    arguments->mode.present = false;
    arguments->cle.present = false;
    arguments->entree = stdin;
    arguments->sortie = stdout;
    arguments->alphabet = NULL;
    arguments->code_perm = NULL;

    return arguments;
}

void freeArguments(Arguments_t* arguments){

    if(arguments->entree) fclose(arguments->entree);
    if(arguments->sortie) fclose(arguments->sortie);
    if(arguments->alphabet) fclose(arguments->alphabet);

    free(arguments);

}


