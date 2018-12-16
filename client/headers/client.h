#ifndef CLIENT_H
#define CLIENT_H

#include "fonctionsRequeteClient.h"


/**
 * La fonction renvoie l'erreur passé en parametre
 * @param {char*} *pFonction : Erreur à renvoyer
 * return void
 */
void erreur(char *pFonction);

/**
 * La fonction ecrit un message d'erreur en cas de parametre incorrect
 * @param {char*} *pNomProgramme : Nom du programme
 * return void
 */
void usage(char *pNomProgramme);

void FermetureClient(int sig);

void requete_client(const char* NOM_SERV, const int PORT_SOURCE);

void printc(char * pChaine,char* COULEUR);

void printfc(char * pChaine,char* COULEUR, char* FOND_COULEUR);

#endif
