#ifndef STATISTIQUES_H
#define STATISTIQUES_H

#include "types.h"
#include "voyage.h"

typedef struct sChaine{
    char *Chaine;
    int nombre;
    struct sChaine* suivant;
}ChaineStats;
/**
 * La fonction qui stock les requetes sur le serveur pour les statistiques
 * @param {char*} pRequete :
 * @return {void}
 */
void ecrire(char*pRequete);
/**
 * La fonction qui affiche une Chaine de statistique
 * @param {char*} 
 * @return {void}
 */
void afficherChaine(ChaineStats *c, int total);
/**
 * La fonction qui regarde si une chaine est contenu dans une autre et incremente son nombre
 */
int ExisteDansEtIncremente(ChaineStats *TabChaine, ChaineStats *Chaine);
/**
 * Fonction qui lis une Case d'une chaine en lui donnant son indice
 */
ChaineStats* LireChaine(ChaineStats *pChaine,int pIndex);
/**
 * Fonction qui Supprime une case d'une chaine en lui donnant son indice
 */
ChaineStats* SupprimerChaine(ChaineStats *pChaine,int pIndex);
/**
 * Fonction qui cree une chaine
 */
ChaineStats *CreerChaine(char*Chaine,int n);
/**
 * Fonction qui ajoute une chaine à un tableau de chaine
 */
ChaineStats *AjouterChaine(ChaineStats *Tabc,ChaineStats *Chaine);
/**
 * Fonction qui renvoy l'index de l'index correspondant au nombre maximum d'apparition
 */
int MaximumChaine(ChaineStats *TabChaine);
/**
 * Fonction qui recupere les villes de départs dans un un Tableau parsé
 */
ChaineStats* villeDToChaine(TableauVoyage *t);
/**
 * Fonction qui recupere les villes d'arrivées dans un Tableau parsé
 */
ChaineStats* villeAToChaine(TableauVoyage *t);
/**
 * Fonction qui transforme un entier en double
 */
double itof(int a);
/**
 * Fonction qui renvoy la taille d'une chaine statistique
 */
int TailleChaine(ChaineStats *Chaine);

/**
 * Fonction qui parse le fichier de statistique
 */
TableauVoyage* Lire_stats(TableauVoyage *pTableau_voyage);

/**
 * Fonction qui Trie une chaine statistique par selection en utilisant MaximumChaine
 */
ChaineStats *Trier(ChaineStats*pChaineNonTrie);
/**
 * Fonction qui transforme une durée en chaine de caractere
 */
char* AtoDuree(horaire pHeure);
/**
 * Fonction qui Transforme une horaire en chaine statistiques
 */
ChaineStats* HeureToChaine(TableauVoyage *t);
/**
 * Fonction qui Traite les heures de départs en appelant les autres fonctions 
 */
void TraiterHdepart(TableauVoyage*TabVoyage);
/**
 * Fonction qui Traite les villes de départs en appelant les autres fonctions 
 */
void TraiterVilleDepart(TableauVoyage*TabVoyage);
/**
 * Fonction qui Traite les villes d'arrivés en appelant les autres fonctions 
 */
void TraiterVilleArrive(TableauVoyage*TabVoyage);
/**
 * Fonction "main" du module statistique
 */
void Statistiques();





#endif
