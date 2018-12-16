#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <string.h>
#include "triModule.h"

/**
 * La fonction calcul du voyage de duree minimum d'un tableau de voyage
 * @param {TableauVoyage*} pTableau :
 * @return {int} l'index de la plus petite durée
 */
int CalculerDureeMinimum(TableauVoyage *pTableau){
    int tmpMinimum=24*60, tmpMaxindex=0, tmpDuree;
    TableauVoyage *tmpIterateur=pTableau;
    int const tmpTaille=calculerTailleTableau(pTableau);
    
    /*On parcourt notre tableau*/
    for(int i=0;i<tmpTaille;i++,tmpIterateur=tmpIterateur->suivant){
        /*Calcul de la durée*/
        tmpDuree=EcartHoraires(tmpIterateur->v.heure_depart, tmpIterateur->v.heure_arrive);
        // printf("%d\n",tmpDuree);
        if(tmpDuree<tmpMinimum){
            /*Si le minimum est plus grand que la duree courrante alors on le remplace*/
            tmpMinimum=tmpDuree;
            tmpMaxindex=i;
        }
    }
    /*On retourne l'index de la plus petite durée*/
    return tmpMaxindex;
}


/**
 * La fonction calcul du voyage de cout minimum d'un tableau de voyage
 * @param {TableauVoyage*} pTableau :
 * @return {int} l'index du plus petit cout
 */
int CalculerCoutMinimum(TableauVoyage *pTableau){
    double tmpMinimum=MAX_PRIX+1.0;
    int tmpMaxindex=0;
    int const tmpTaille=calculerTailleTableau(pTableau);
    TableauVoyage *tmpIterateur=pTableau;
    
    /*on fais un trie par selection comme dans le cas de la duree mais pour un cout*/
    
    for(int i=0;i<tmpTaille;i++,tmpIterateur=tmpIterateur->suivant){
        if(tmpIterateur->v.prix<tmpMinimum){
            tmpMinimum=tmpIterateur->v.prix;
            tmpMaxindex=i;
        }
    }
    return tmpMaxindex;
}


/**
 * La fonction qui trie un tableau de voyage en fonction de leur cout (trie par selection)
 * @param {TableauVoyage} pTableau :
 * @return {TableauVoyage*} Le Tableau de voyage trié par ordre croissant en fonction des cout des voyages qu'il contient
 */
TableauVoyage *TrierVoyageParCout(TableauVoyage *pTableau){
    TableauVoyage *tmpTableau=NULL;
    voyage *tmpVoyage=NULL;
    int tmpIndex=0;
    int const tmpTaille=calculerTailleTableau(pTableau);
    
    /*on trie tout les éléments*/
    for(int i=0;i<tmpTaille;i++){
        /*on cherche le plus petit prix*/
        tmpIndex=CalculerCoutMinimum(pTableau);
        /*on y associe son voyage*/
        tmpVoyage=LireVoyage(pTableau,tmpIndex);
        /*on l'ajoute au tableau temporaire qui sera notre résultat*/
        tmpTableau=ajouter_voyage(tmpTableau,tmpVoyage);
        /*puis on le supprime du tableau initial avant de recommencer*/
        pTableau=SupprimerVoyage(pTableau,tmpIndex);
    }
    
    return tmpTableau;
}


/**
 * La fonction qui trie un tableau de voyage en fonction de leur duree (trie par selection)
 * @param {TableauVoyage} pTableau :
 * @return {TableauVoyage*} Le Tableau de voyage trié par ordre croissant en fonction des duree des voyages qu'il contient
 */
TableauVoyage *TrierDuree(TableauVoyage *pTableau){
    TableauVoyage *tmpTableau=NULL;
    voyage *tmpVoyage=NULL;
    int tmpIndex=0;
    int const tmpTaille=calculerTailleTableau(pTableau);
    
    /*on trie tout les éléments cf:TrierCout*/
    for(int i=0;i<tmpTaille;i++){
        tmpIndex=CalculerDureeMinimum(pTableau);
        tmpVoyage=LireVoyage(pTableau,tmpIndex);
        tmpTableau=ajouter_voyage(tmpTableau,tmpVoyage);
        pTableau=SupprimerVoyage(pTableau,tmpIndex);
    }
    return tmpTableau;
}

/**
 * La fonction qui trie un tableau de voyage dans un tableau de voyage en fonction de sa duree ou de son prix
 * @param {TableauVoyage} pTableau :
 * @param {char *} pFiltre :
 * @return {TableauVoyage*} Un Tableau de voyage trié en fonction du filtre passé en parametre
 */
TableauVoyage* TrierTrajets(char* pFiltre, TableauVoyage* pListeVoyage)
{
    /*traitement du filtre et aiguillage vers fonction associé*/
    if(!strcmp(pFiltre,"PRIX"))
        pListeVoyage=TrierVoyageParCout(pListeVoyage);
    else if(!strcmp(pFiltre,"DUREE"))
        pListeVoyage=TrierDuree(pListeVoyage);
    else{
        fprintf(stderr,"ERREUR MAUVAIS FORMAT FILTRE\n");
        return NULL;
    }
    return pListeVoyage;
}


/**
 * La fonction qui retourne une liste contenant le voyage le plus court ou le moins chere
 * @param {TableauVoyage} pTableau :
 * @param {char *} pFiltre :
 * @return {TableauVoyage*} Un Tableau de voyage trié en fonction du filtre passé en parametre
 */
TableauVoyage* filterTrajets(char* pFiltre, TableauVoyage* pListeVoyage)
{
    TableauVoyage *tmpTableauVoyage = NULL;
    /*traitement du filtre et aiguillage vers fonction associé*/
    if(!strcmp(pFiltre,"PRIX"))
    {
        tmpTableauVoyage = ajouter_voyage(tmpTableauVoyage, LireVoyage(pListeVoyage, CalculerCoutMinimum(pListeVoyage)));
    }
    else if(!strcmp(pFiltre,"DUREE"))
    {
        tmpTableauVoyage = ajouter_voyage(tmpTableauVoyage, LireVoyage(pListeVoyage, CalculerDureeMinimum(pListeVoyage)));
    }
    else if(!strcmp(pFiltre,"TPRIX"))
    {
        tmpTableauVoyage = TrierTrajets("PRIX",pListeVoyage);
    }
    else if(!strcmp(pFiltre,"TDUREE"))
    {
        tmpTableauVoyage = TrierTrajets("DUREE",pListeVoyage);
    }
    else{
        fprintf(stderr,"ERREUR MAUVAIS FORMAT FILTRE\n");
    }
    return tmpTableauVoyage;
}

/**
 * Fonction qui calcul l'ecart en minutes entre deux horraires
 * @param	{horaire} 2 horaires
 * @return	{int} l'écart en minutes
 */
int EcartHoraires(horaire pHeure1, horaire pHeure2){
    int tmpMinutes1=(pHeure1.heure*60) + (pHeure1.minute);
    int tmpMinutes2=(pHeure2.heure*60) + (pHeure2.minute);
    
    if(tmpMinutes1<tmpMinutes2)
        return tmpMinutes2-tmpMinutes1;
    else
        return tmpMinutes1-tmpMinutes2;
}

