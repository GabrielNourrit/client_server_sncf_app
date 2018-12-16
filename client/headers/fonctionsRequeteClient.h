#ifndef FONCTIONS_REQUETE_CLIENT_H
#define FONCTIONS_REQUETE_CLIENT_H


/**
 * Fonction qui crée une requête de demande de filtrage en demandant au client les infos
 * @parar	{char*} pRequetePrecedente, requete precedemment envoyée par le client
 * @return	{char*} requete, la nouvelle requete de filtrage crée
 */
char* creer_requete_filtre(char* pRequetePrecedente);

/**
 * Fonction qui crée une requête en demandant au client les infos
 * @parar	void
 * @return	{char*} requete, la nouvelle requete crée
 */
char* creer_requete(char* requete);

/**
 * La fonction ecrit des données pour le serveur
 * @param {int} pSocket : Socket d'ecoute du serveur
 * @return void
 */
void envoie_requete_serveur(int pSocket);

/**
 * La méthode découpe la chaine de caractere à partir d'un délimiteur passé en parametre
 * @param {char*} pChaine : Chaine de caractere à traiter
 * @param {char} pDelimiteur : Delimiteur à appliquer sur la chaine
 * @return {char**} tableau de chaine de caracteres separé par le delimiteur et se finissant par (char*)0
 */
char** diviseur_chaine(char* pChaine, const char pDelimiteur);

/**
 * Fonction qui verifie si un caractere est une lettre de l'alphabet
 * @parar	{char} pLettre
 * @return	{int} retourne 1(vrai) ou 0(faux) celon si pLettre est une lettre
 */
int EstLettre(char pLettre);
/**
 * Fonction qui verifie que le nom d'une ville est valide
 * On veut que çà contiennent des lettres ou des tiret/espaces si ces derniers sont entourés de lettres
 * La taille minimal est de 3 lettres
 * @parar	{char*} pChaine à verifier
 * @return	{int} retourne 1(vrai) ou 0(faux) celon si pChaine est correcte ou non.
 */
int EstVilleValide(char* pChaine);

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

/*
 * La fonction parse et affiche la requete en parametre
 * @param {char*} pReponse : Requete a afficher sous forme de string
 * @return void
 */
void afficher_requete_resultat(char* pReponse);

#endif
