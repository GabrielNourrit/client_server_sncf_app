#ifndef SERVEUR_H
#define SERVEUR_H

#include "types.h"
#include "fonctionsRecherche.h"
#include "voyage.h"
#include "triModule.h"


/**
 * La fonction traite une requete client
 * @param {int} pSocket : socket sur lequelle on lit la requete
 * @param {TableauVoyage} *pTableau : Tableau contenant la liste des voyages
 * @return void
 */
void traitement_tmpRequete_client(int pSocket, TableauVoyage *pTableau);

/**
 * La fonction creer le serveur
 * @param {const int} PORT_SOURCE : port sur lequel ecoute le serveur
 * @return void
 */
void lancer_serveur(const int PORT_SOURCE, TableauVoyage *pTableau);

/**
 * Fonction qui transforme un {TableauVoyage} en {char*}
 * @param	{TableauVoyage*} pTableau, le tableau à transformer
 * @return	{char*} le tableau transformé
 */
char* transformer_tableau_en_reponse(TableauVoyage *pTableau, int pTailleTableau);

void recupere_mort_fils();

void erreur(char *p_fonction);

void usage(char *p_nom_programme);
/**
 * La fonction envoie la requete resultat au client
 * @param {int} pSocket : socket sur lequelle on lit la requete
 * @param {char*} char* : message a envoyer sous forme de string
 * @return void
 **/
void envoie_requete_resultat(int pSocket, char * pRequete);

/**
 * La fonction gere la fermeture du serveur grace a Controle + C
 * @param {int} sig : numero du signal
 **/
void FermetureServeur(int sig);
#endif
