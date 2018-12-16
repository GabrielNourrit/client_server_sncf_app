#ifndef OUTIL_H
#define OUTIL_H

#include "types.h"

/**
 * La fonction calcul un prix en fonction d'un code promo
 * @param {double} pPrix :
 * @param {CodePromo} pCode_promo :
 * @return {int} retourn le prix apres REDUC ou SUPPL
 */
double calculPrixCodePromo(double pPrix, CodePromo pCode_promo);

/**
 * La méthode découpe la chaine de caractere à partir d'un délimiteur passé en parametre
 * @param {char*} pChaine : Chaine de caractere à traiter
 * @param {char} pDelimiteur : Delimiteur à appliquer sur la chaine
 * @return {char**} tableau de chaine de caracteres separé par le delimiteur et se finissant par (char*)0
 */
char** diviseur_chaine(char* pChaine, const char pDelimiteur);

/**
 * La fonction transforme un char* "heure:minute" en int* {heure, minute}
 * @param {char*} pHeure :
 * @return {int*} pointeur de int contenant les valeur dans pHeure
 */
int* atoiHeure(char* pHeure);

/**
 * La fonction calcul l'ecart entre 2 horaires differents
 * @param {int*} pHeureMin :
 * @param {int*} pHeureMax :
 * @return {int} retourn la difference convertie en minutes
 */
int calculHeurePtrPtr(int* pHoraireMin, int* pHoraireMax);

/**
 * La fonction calcul l'ecart entre 2 horaires differents
 * @param {int} pHeureMinHeure :
 * @param {int} pHeureMinMinute :
 * @param {int*} pHeureMax :
 * @return {int} retourn la difference convertie en minutes
 */
int calculHeureIntPtr(int pHoraireMinHeure, int pHoraireMinMinute, int* pHoraireMax);


/**
 * La fonction transforme un char* "heure:minute" en int* {heure, minute}
 * @param {char*} pHeure :
 * @return {int*} pointeur de int contenant les valeur dans pHeure
 */
int* atoiHeure(char* pHeure);

/**
 * La fonction calcul l'ecart entre 2 horaires differents
 * @param {int*} pHeureMin :
 * @param {int*} pHeureMax :
 * @return {int} retourn la difference convertie en minutes
 */
int calculHeurePtrPtr(int* pHoraireMin, int* pHoraireMax);


/**
 * La fonction calcul l'ecart entre 2 horaires differents
 * @param {int} pHeureMinHeure :
 * @param {int} pHeureMinMinute :
 * @param {int*} pHeureMax :
 * @return {int} retourn la difference convertie en minutes
 */
int calculHeureIntPtr(int pHoraireMinHeure, int pHoraireMinMinute, int* pHoraireMax);

/**
 * Fonction qui transforme un {horaire} en {char*}
 * @param	{horaire} pNum, le numero à transformer
 * @return	{char*} l'horaire transformé
 */
char* transformer_horaire_en_string(horaire pHoraire);

void printc(char * pChaine,char* COULEUR);

void printfc(char * pChaine,char* COULEUR, char* FOND_COULEUR);


#endif
