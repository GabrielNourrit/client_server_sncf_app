#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "serveur.h"

int main(int argc,char*argv[]){
    
    /*Si il n'y a pas qu'un et unique argument on appel la fonction usage*/
    if(argc !=2) usage(argv[0]);
    const int PORT_SOURCE = atoi(argv[1]);
    
    TableauVoyage *tmpTableau=NULL;
    
    /*Parsage et instanciation de la base de données*/
    tmpTableau=remplir_tableau_voyage(tmpTableau);
    
    /* Lancer Serveur*/
    lancer_serveur(PORT_SOURCE, tmpTableau);
    
    exit(0);
}
