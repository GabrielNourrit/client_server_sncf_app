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
#include "outil.h"


#define couleur(param) printf("\033[%sm",param)
#define NOIR "30"
#define ROUGE "31"
#define VERT "32"
#define JAUNE "33"
#define BLEU "34"
#define MAGENTA "35"
#define CYAN "36"
#define BLANC "37"
#define FOND_NOIR "40"
#define FOND_ROUGE "41"
#define FOND_VERT "42"
#define FOND_JAUNE "43"
#define FOND_BLEU "44"
#define FOND_MAGENTA "45"
#define FOND_CYAN "46"
#define FOND_BLANC "47"
#define DEFAUT "0"

/**
 * La fonction calcul un prix en fonction d'un code promo
 * @param {double} pPrix :
 * @param {CodePromo} pCode_promo :
 * @return {int} retourn le prix apres REDUC ou SUPPL
 */
double calculPrixCodePromo(double pPrix, CodePromo pCode_promo)
{
    if(pCode_promo == REDUC)
    {
        pPrix = pPrix*(double)(0.80);
        return pPrix;
    }
    else if(pCode_promo == SUPPL)
    {
        pPrix = pPrix*(double)(1.10);
        return pPrix;
        
    }
    else
    {
        return pPrix;
    }
}



/**
 * La méthode découpe la chaine de caractere à partir d'un délimiteur passé en parametre
 * @param {char*} pChaine : Chaine de caractere à traiter
 * @param {char} pDelimiteur : Delimiteur à appliquer sur la chaine
 * @return {char**} tableau de chaine de caracteres separé par le delimiteur et se finissant par (char*)0
 */
char** diviseur_chaine(char* pChaine, const char pDelimiteur)
{
    char** tmpResultat = 0;
    size_t tmpCompteur = 0;
    char* tmpChaine = pChaine;
    char* tmpLigneCourante = 0;
    char tmpDelimiteur[2];
    size_t tmpId;
    char* token;
    tmpDelimiteur[0] = pDelimiteur;
    tmpDelimiteur[1] = 0;
    
    /*On compte le nombre de chaine séparé par un délimiteur*/
    while (*tmpChaine)
    {
        if (pDelimiteur == *tmpChaine)
        {
            tmpCompteur++;
            tmpLigneCourante = tmpChaine;
        }
        tmpChaine++;
    }
    
    /*Si le pointeur se trouve dans la chaine il reste un mot à prendre en compte*/
    tmpCompteur += tmpLigneCourante < (pChaine + strlen(pChaine) - 1);
    /*Puis on ajoute un pour le mot 0*/
    tmpCompteur++;
    /*On alloue la mémoire au tableau resultat celon le nombre de mots*/
    tmpResultat = malloc(sizeof(char*) * tmpCompteur);
    
    tmpId  = 0;
    token = strtok(pChaine, tmpDelimiteur);
    
    /*tant qu'on a des mots on remplit notre tableau*/
    while (token)
    {
        *(tmpResultat + tmpId++) = strdup(token);
        token = strtok(0, tmpDelimiteur);
    }
    /*on ajoute le mot 0*/
    *(tmpResultat + tmpId) = 0;
    
    return tmpResultat;
}

/**
 * Fonction qui transforme un {horaire} en {char*}
 * @param	{horaire} pNum, le numero à transformer
 * @return	{char*} l'horaire transformé
 */
char* transformer_horaire_en_string(horaire pHoraire) {
    char* chaine_horaire = (char*) malloc(7*sizeof(char));
    /*Concat de l'heure sous forme hh:mm*/
    if (pHoraire.heure < 10) {
        if (pHoraire.minute < 10)
            sprintf(chaine_horaire, "0%d:0%d", pHoraire.heure, pHoraire.minute);
        else
            sprintf(chaine_horaire, "0%d:%d", pHoraire.heure, pHoraire.minute);
    }
    else {
        if (pHoraire.minute < 10)
            sprintf(chaine_horaire, "%d:0%d", pHoraire.heure, pHoraire.minute);
        else
            sprintf(chaine_horaire, "%d:%d", pHoraire.heure, pHoraire.minute);
    }
    
    return chaine_horaire;
}


/**
 * La fonction transforme un char* "heure:minute" en int* {heure, minute}
 * @param {char*} pHeure :
 * @return {int*} pointeur de int contenant les valeur dans pHeure
 */
int* atoiHeure(char* pHeure)
{
    char** tmpTableauHoraire;
    int* tmpTableauResult;
    tmpTableauResult = (int*)malloc(2*sizeof(int));
    
    tmpTableauHoraire = diviseur_chaine(pHeure,':');
    
    tmpTableauResult[0] = atoi(tmpTableauHoraire[0]);
    tmpTableauResult[1] = atoi(tmpTableauHoraire[1]);
    
    return tmpTableauResult;
}

/**
 * La fonction calcul l'ecart entre 2 horaires differents
 * @param {int*} pHeureMin :
 * @param {int*} pHeureMax :
 * @return {int} retourn la difference convertie en minutes
 */
int calculHeurePtrPtr(int* pHoraireMin, int* pHoraireMax)
{
    return 60*(pHoraireMax[0]-pHoraireMin[0]) + (pHoraireMax[1]-pHoraireMin[1]);
}


/**
 * La fonction calcul l'ecart entre 2 horaires differents
 * @param {int} pHeureMinHeure :
 * @param {int} pHeureMinMinute :
 * @param {int*} pHeureMax :
 * @return {int} retourn la difference convertie en minutes
 */
int calculHeureIntPtr(int pHoraireMinHeure, int pHoraireMinMinute, int* pHoraireMax)
{
    return 60*(pHoraireMax[0]-pHoraireMinHeure) + (pHoraireMax[1]-pHoraireMinMinute);
}


void printc(char * pChaine,char* COULEUR){
    couleur(COULEUR);
    printf("%s",pChaine);
    couleur(DEFAUT);
}

void printfc(char * pChaine,char* COULEUR, char* FOND_COULEUR){
    couleur(COULEUR);
    couleur(FOND_COULEUR);
    printf("%s",pChaine);
    couleur(DEFAUT);
}


