#ifndef FONCTIONS_RECHERCHE_H
#define FONCTIONS_RECHERCHE_H

#include "types.h"
#include "outil.h"
#include "voyage.h"

/**
 * La fonction recherche un voyage parmis les voyages contenu dans pTableau
 * @param {char*} pVille_depart :
 * @param {char*} pVille_arrive :
 * @param {char*} pHeure_depart : Delimiteur à appliquer sur la chaine
 * @param {TableauVoyage*} pTableau :
 * @return {voyage} struct voyage du voyage en question ou voyage NULL
 */
TableauVoyage* rechercherTrainParHoraireDepart(char* pVille_depart, char* pVille_arrive, char* pHeure_depart, TableauVoyage *pTableau);

/**
 * La fonction recherche tout voyage d'un point a un autre parmis les voyages contenu dans pTableau
 * @param {char*} pVille_depart :
 * @param {char*} pVille_arrive :
 * @param {TableauVoyage*} pTableau :
 * @return {TableauVoyage*} tableau contenant les voyages en question
 */
TableauVoyage* rechercherTrainsParDestionation(char* pVille_depart, char* pVille_arrive, TableauVoyage *pTableau);

/**
 * La fonction recherche tout voyage d'un point a un autre parmis les voyages contenu dans pTableau
 * @param {char*} pVille_depart :
 * @param {char*} pVille_arrive :
 * @param {char*} pHeure_depart_min :
 * @param {char*} pHeure_depart_max :
 * @param {TableauVoyage*} pTableau :
 * @return {TableauVoyage*} tableau contenant les voyages en question
 */
TableauVoyage* rechercherTrainsParTrancheHoraires(char* pVille_depart, char* pVille_arrive, char* pHeure_depart_min, char* pHeure_depart_max, TableauVoyage *pTableau);


#endif
