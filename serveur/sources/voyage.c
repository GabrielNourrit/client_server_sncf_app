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
#include "voyage.h"

/**
 * La fonction qui lit un voyage en fonction de son index dans un tableau de voyage
 * @param {TableauVoyage} pTableau :
 * @param {int} pIndex :
 * @return {voyage*} Le voyage correspondant à l'index demandé en parametre dans le tableau
 */
voyage* LireVoyage(TableauVoyage *pTableau,int pIndex){
    int tmpIndice;
    if(pIndex==0){
        /*si il n'y a qu'une case on la retourne*/
        return &(pTableau->v);
    }
    /*on place notre pointeur sur la case*/
    for(tmpIndice=0;tmpIndice<pIndex;tmpIndice++,pTableau=pTableau->suivant) ;
    /*on retourne la case qui nous interesse*/
    return &(pTableau->v);
    
}


/**
 * La fonction qui supprime un voyage en fonction de son index dans un tableau de voyage
 * @param {TableauVoyage} pTableau :
 * @param {int} pIndex :
 * @return {TableauVoyage*} Le Tableau de voyage sans le voyage correspondant à l'index passé en parametre
 */
TableauVoyage* SupprimerVoyage(TableauVoyage *pTableau,int pIndex){
    TableauVoyage *tmpIterateur=pTableau,*tmpTableau=NULL;
    int i;
    /*si il y a une case on la supprime et on retourne null*/
    if(pIndex==0){;
        if(calculerTailleTableau(pTableau)==1){
            free(pTableau);
            return NULL;
        }
        /*sinon on envoie la suite*/
        tmpIterateur=tmpIterateur->suivant;
        free(pTableau);
        return tmpIterateur;
    }
    /*on place notre pointeur sur la case à pop*/
    for(i=0;i<pIndex;i++,tmpIterateur=tmpIterateur->suivant)
        tmpTableau=tmpIterateur;
    /*on lie la case d'avant à celle d'après*/
    tmpTableau->suivant=tmpIterateur->suivant;
    /*on supprime la case retiré*/
    free(tmpIterateur);
    /*on retourne le debut de la liste chainee*/
    return pTableau;
}


/**
 * La fonction calcule la taille du Tableau passee en parametre
 **/
int calculerTailleTableau(TableauVoyage * pTableau) {
    int tmpTaille = 0;
    TableauVoyage * tmpIterateur = pTableau;
    
    while (tmpIterateur != NULL) {
        tmpTaille ++;
        tmpIterateur = tmpIterateur-> suivant;
    }
    
    return tmpTaille;
}




/**
 * La méthode lis le fichier, et remplis le tableau passé en parametre
 * @param {TableauVoyage} pTableau_voyage : tableau à remplir
 * @return {int} renvoie -1 en cas d'erreur, 1 si reussi
 */
TableauVoyage* remplir_tableau_voyage(TableauVoyage *pTableau_voyage){
    FILE * tmp_fichier;
    char * tmpLigne = NULL;
    size_t tmpLongueur = 0;
    ssize_t tmpTailleLigneCourante;
    int tmpCompteur = 0;
    char** tmpLigneCourante;
    char ** tmpTableauHoraireDepart;
    char ** tmpTableauHoraireArrive;
    voyage* tmpVoyage;
    horaire* tmpHoraireDepart;
    horaire* tmpHoraireArrivee;
    enum CodePromo tmpCodePromo;
    
    /*On ouvre le fichier*/
    tmp_fichier = fopen("Trains.txt", "r");
    if (tmp_fichier == NULL){
        fprintf(stderr,"ERREUR OUVERTURE IMPOSSIBLE DU FICHIER");
        exit(EXIT_FAILURE);
    }
    
    /*On parcours toutes les lignes du fichier*/
    while ((tmpTailleLigneCourante = getline(&tmpLigne, &tmpLongueur, tmp_fichier)) != -1) {
        /*On decoupe la chaine de caractere en tableau*/
        tmpLigneCourante=diviseur_chaine(tmpLigne,';');
        /*Si on a une ligne correcte non null*/
        if(*tmpLigneCourante){
            
            /*On separe l'heure et les minutes pour l'heure de depart et l'heure d'arrivé car on recupere une chaine du type hh:MM*/
            tmpTableauHoraireDepart = diviseur_chaine(tmpLigneCourante[3],':');
            tmpTableauHoraireArrive = diviseur_chaine(tmpLigneCourante[4],':');
            /*On crée le type horaire pour l'horaire d'arrivé et de départ*/
            
            tmpHoraireDepart = creer_horaire(atoi(tmpTableauHoraireDepart[0]),atoi(tmpTableauHoraireDepart[1]));
            tmpHoraireArrivee = creer_horaire(atoi(tmpTableauHoraireArrive[0]),atoi(tmpTableauHoraireArrive[1]));
            
            int tmpNumeroTrain = atoi(tmpLigneCourante[0]);
            
            /*On crée le code promo associé*/
            if(strstr(tmpLigneCourante[6],"REDUC")!= NULL)
            {
                tmpCodePromo=REDUC;
            }
            else if(strstr(tmpLigneCourante[6],"SUPPL")!= NULL)
            {
                tmpCodePromo=SUPPL;
            }
            else
            {
                tmpCodePromo=DEFAULT;
            }
            /*On crée un voyage et on l'ajoute au tableau*/
            tmpVoyage=creer_voyage(tmpNumeroTrain,tmpLigneCourante[1], tmpLigneCourante[2], *tmpHoraireDepart,*tmpHoraireArrivee,atof(tmpLigneCourante[5]),tmpCodePromo);
            
            /* On ajoute le voyage au tableau */
            pTableau_voyage = ajouter_voyage(pTableau_voyage,tmpVoyage);
            free(*(tmpLigneCourante));
            tmpCompteur++;
            /*printf("%d // %s---%s---%s---%s---%s---%s\n\n", tmpCompteur+1,tmpLigneCourante[0],tmpLigneCourante[1],tmpLigneCourante[2],tmpLigneCourante[3],tmpLigneCourante[4],tmpLigneCourante[5]);*/
        }
    }
    return pTableau_voyage;
}

/**
 * Fonction qui crée et initialise un voyage
 * @param	{int} pNumero_train, numéro du train
 * @param	{char*} pVoyageille_depart, ville de départ
 * @param	{char*} pVoyageille_arrive, ville d'arrivee
 * @param	{horaire} pHeure_depart, horaire de départ
 * @param	{horaire} pHeure_arrive, horaire d'arrivee
 * @param	{double} pPrix, prix du trajet
 * @param	{enum CodePromo} pCode_promo, code promo (REDUC, SUPPL, DEFAULT)
 * @return	Le voyage crée et initialisé
 */
voyage* creer_voyage(int pNumero_train, char* pVoyageille_depart, char* pVoyageille_arrive, horaire pHeure_depart, horaire pHeure_arrive, double pPrix, CodePromo pCode_promo) {
    voyage *tmpVoyage = (voyage*)malloc(sizeof(voyage));
    /*Remplissage des données du voyage*/
    tmpVoyage->numero_train = pNumero_train;
    tmpVoyage->ville_depart = pVoyageille_depart;
    tmpVoyage->ville_arrive = pVoyageille_arrive;
    tmpVoyage->heure_depart.heure = pHeure_depart.heure;
    tmpVoyage->heure_depart.minute = pHeure_depart.minute;
    tmpVoyage->heure_arrive.heure = pHeure_arrive.heure;
    tmpVoyage->heure_arrive.minute = pHeure_arrive.minute;
    tmpVoyage->heure_arrive = pHeure_arrive;
    
    /*On calcul le cout pour chaque voyage ajoute*/
    tmpVoyage->prix = calculPrixCodePromo(pPrix, pCode_promo);
    
    tmpVoyage->code_promo = pCode_promo;
    return tmpVoyage;
}

/**
 * Fonction qui crée et initialise un horaire
 * @param	{int} pHeure, heure de l'horaire
 * @param	{int} pMinute, minute de l'horaire
 * @return	L'horaire crée
 */
horaire* creer_horaire(int pHeure, int pMinute){
    horaire *h=(horaire *)malloc(sizeof(horaire));
    /*Verification format heure*/
    if(pHeure<0 || pHeure>24 || pMinute<0 || pMinute>60){
        fprintf(stderr,"ERREUR FORMAT HORAIRE\n");
        exit(-1);
    }
    /*Remplissage des données*/
    h->heure=pHeure;
    h->minute=pMinute;
    return h;
}


/**
 * Fonction qui ajoute un voyage à la fin de le tableau des voyages
 * @param	{TableauVoyage} *pTab_v, le tableau des voyages où l'on ajoute
 * @param	{voyage} pVoyage, le voyage à ajouter
 * @return	le tableau des voyages avec le voyage ajouté en fin
 */
TableauVoyage* ajouter_voyage(TableauVoyage *pTab_v, voyage* pVoyage){
    TableauVoyage *tmpNouveauVoyage = (TableauVoyage*)malloc(sizeof(TableauVoyage));
    /*Sauvegarde de notre table*/
    TableauVoyage *tmpSauvegarde=pTab_v;
    /*On ajoute le voyage à le tableau*/
    tmpNouveauVoyage->v = *pVoyage;
    tmpNouveauVoyage->suivant = NULL;
    /*Si le tableau est vide*/
    if(pTab_v==NULL) {
        /*On retourne le tableau avec le voyage ajouter*/
        return tmpNouveauVoyage;
    }
    /*Sinon on parcourt le tableau*/
    else {
        /*Parcours du tableau*/
        while(pTab_v->suivant!=NULL)
            pTab_v=pTab_v->suivant;
        /*On ajoute notre voyage à fin de le tableau*/
        pTab_v->suivant = tmpNouveauVoyage;
    }
    return tmpSauvegarde;
}


/**
 * La fct affiche le tableau
 **/
void afficher_table(TableauVoyage * pTableau)
{
    const int tmpTailleTableau = calculerTailleTableau(pTableau);
    printf("\nAfficher Tableau:\n\n");
    if(pTableau==NULL)
    {
        printf("Tableau Vide\n\n");
    }
    else{
        for (int i = 0; i < tmpTailleTableau; i++, pTableau = pTableau->suivant) {
            printf("=> %d, %s, %s, %d, %d, %d, %d, %f \n\n",pTableau->v.numero_train, pTableau->v.ville_depart, pTableau->v.ville_arrive, pTableau->v.heure_depart.heure, pTableau->v.heure_depart.minute, pTableau->v.heure_arrive.heure, pTableau->v.heure_arrive.minute, pTableau->v.prix);
        }
    }
}

/**
 * La fct affiche un voyage
 **/
void afficher_voyage(voyage *tmpVoyage)
{
    if(tmpVoyage!=NULL)
    {
        printf("Affichage Voyage retourné par rechercherTrainParDepart() \n => %d, %s, %s, %d, %d, %d, %d, %f \n\n",tmpVoyage->numero_train, tmpVoyage->ville_depart, tmpVoyage->ville_arrive, tmpVoyage->heure_depart.heure, tmpVoyage->heure_depart.minute, tmpVoyage->heure_arrive.heure, tmpVoyage->heure_arrive.minute, tmpVoyage->prix);
    }
}

