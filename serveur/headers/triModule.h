#ifndef TRI_MODULE_H
#define TRI_MODULE_H

#include "outil.h"
#include "voyage.h"

/**
 * La fonction calcul du voyage de cout minimum d'un tableau de voyage
 * @param {TableauVoyage*} pTableau :
 * @return {int} l'index du plus petit cout
 */
int CalculerCoutMinimum(TableauVoyage *pTableau);
/**
 * La fonction calcul du voyage de duree minimum d'un tableau de voyage
 * @param {TableauVoyage*} pTableau :
 * @return {int} l'index de la plus petite durée
 */
int CalculerDureeMinimum(TableauVoyage *pTableau);
/**
 * La fonction qui trie un tableau de voyage en fonction de leur cout (trie par selection)
 * @param {TableauVoyage} pTableau :
 * @return {TableauVoyage*} Le Tableau de voyage trié par ordre croissant en fonction des cout des voyages qu'il contient
 */
TableauVoyage *TrierVoyageParCout(TableauVoyage *pTableau);
/**
 * La fonction qui trie un tableau de voyage en fonction de leur duree (trie par selection)
 * @param {TableauVoyage} pTableau :
 * @return {TableauVoyage*} Le Tableau de voyage trié par ordre croissant en fonction des duree des voyages qu'il contient
 */
TableauVoyage *TrierDuree(TableauVoyage *pTableau);
/**
 * La fonction qui trie un tableau de voyage dans un tableau de voyage en fonction de sa duree ou de son prix
 * @param {TableauVoyage} pTableau :
 * @param {char *} pFiltre :
 * @return {TableauVoyage*} Un Tableau de voyage trié en fonction du filtre passé en parametre
 */
TableauVoyage* filterTrajets(char* pFiltre, TableauVoyage* pListeVoyage);
/**
 * La fonction qui trie un tableau de voyage dans un tableau de voyage en fonction de sa duree ou de son prix
 * @param {TableauVoyage} pTableau :
 * @param {char *} pFiltre :
 * @return {TableauVoyage*} Un Tableau de voyage trié en fonction du filtre passé en parametre
 */
TableauVoyage* TrierTrajets(char* pFiltre, TableauVoyage* pListeVoyage);
/**
 * Fonction qui calcul l'ecart en minutes entre deux horraires
 * @param	{horaire} 2 horaires
 * @return	{int} l'écart en minutes
 */
int EcartHoraires(horaire pHeure1, horaire pHeure2);

#endif
