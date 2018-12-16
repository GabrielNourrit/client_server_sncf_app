#ifndef VOYAGE_H
#define VOYAGE_H

#include "types.h"
#include "outil.h"

/**
 * La fonction qui lit un voyage en fonction de son index dans un tableau de voyage
 * @param {TableauVoyage} pTableau :
 * @param {int} pIndex :
 * @return {voyage*} Le voyage correspondant à l'index demandé en parametre dans le tableau
 */
voyage* LireVoyage(TableauVoyage *pTableau,int pIndex);
/**
 * La fonction qui supprime un voyage en fonction de son index dans un tableau de voyage
 * @param {TableauVoyage} pTableau :
 * @param {int} pIndex :
 * @return {TableauVoyage*} Le Tableau de voyage sans le voyage correspondant à l'index passé en parametre
 */
TableauVoyage* SupprimerVoyage(TableauVoyage *pTableau,int pIndex);

/**
 * La fonction calcule la taille du Tableau passee en parametre
 **/
int calculerTailleTableau(TableauVoyage * pTableau);


/**
 * La méthode lis le fichier, et remplis le tableau passé en parametre
 * @param {TableauVoyage} pTableau_voyage : tableau à remplir
 * @return {int} renvoie -1 en cas d'erreur, 1 si reussi
 */
TableauVoyage* remplir_tableau_voyage(TableauVoyage *pTableau_voyage);


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
voyage * creer_voyage(int pNumero_train, char* pVoyageille_depart, char* pVoyageille_arrive, horaire pHeure_depart, horaire pHeure_arrive, double pPrix, CodePromo pCode_promo);

/**
 * Fonction qui crée et initialise un horaire
 * @param	{int} pHeure, heure de l'horaire
 * @param	{int} pMinute, minute de l'horaire
 * @return	L'horaire crée
 */
horaire *creer_horaire(int pHeure, int pMinute);

/**
 * Fonction qui ajoute un voyage à la fin de le tableau des voyages
 * @param	{TableauVoyage} *pTab_v, le tableau des voyages où l'on ajoute
 * @param	{voyage} pVoyage, le voyage à ajouter
 * @return	le tableau des voyages avec le voyage ajouté en fin
 */
TableauVoyage* ajouter_voyage(TableauVoyage *pTab_v, voyage* pVoyage);

/**
 * La fct affiche le tableau
 **/
void afficher_table(TableauVoyage * pTableau);

/**
 * La fct affiche un voyage
 **/
void afficher_voyage(voyage *tmpVoyage);

#endif
