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
#include <signal.h>
#include <errno.h>
#include "serveur.h"
#include "statistiques.h"

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

#define MAX_CONNEXION_SIM 5
#define TAMPON_TAILLE_MAX 512
int tmpServices;

void lancer_serveur(const int PORT_SOURCE, TableauVoyage *pTableau){
    
    int tmpIdSocketEcoute;
    unsigned int *tmpTailleAdresseClient = (unsigned int*)malloc(sizeof(unsigned int));
    struct sigaction tmpGestionSignal;
    struct sockaddr_in tmpAdresseServeur, tmpAdresseClient;
    
    if(signal(SIGINT,FermetureServeur) == SIG_ERR) erreur("signal");
    
    *tmpTailleAdresseClient = sizeof(struct sockaddr_in);
    
    /*Gestion des signaux à intercepter*/
    /*--On utilise la fonction recupere_mort_fils*/
    tmpGestionSignal.sa_handler = recupere_mort_fils;
    /*--On évite l'interruption de accept par SIGCHLD*/
    tmpGestionSignal.sa_flags = SA_RESTART;
    /*--On précise que notre gestionnaire s'applique aux signaux SICHLD*/
    if(sigaction(SIGCHLD, &tmpGestionSignal, NULL)==-1) erreur("sigaction");
    
    /*Création d'une socket qui va nous servir à écouter*/
    tmpIdSocketEcoute = socket(AF_INET,SOCK_STREAM,0);
    if(tmpIdSocketEcoute == -1) erreur("socket");
    
    /*configuration de l'adresse du serveur*/
    tmpAdresseServeur.sin_family = AF_INET;
    tmpAdresseServeur.sin_port = htons(PORT_SOURCE);
    tmpAdresseServeur.sin_addr.s_addr = htonl(INADDR_ANY);
    /*liaison de l'id de la socket du serveur à l'adresse de la socket du serveur*/
    if(bind(tmpIdSocketEcoute,(struct sockaddr *)&tmpAdresseServeur, sizeof(struct sockaddr_in)) == -1) erreur("bind");
    /*Mise en écoute de la socket serveur*/
    if(listen(tmpIdSocketEcoute,MAX_CONNEXION_SIM) == -1) erreur("listen");
    
    /*tant que l'on reçoit des requêtes de connexions, on crée des services*/
    while(1){
        tmpServices = accept(tmpIdSocketEcoute, (struct sockaddr *)&tmpAdresseClient, tmpTailleAdresseClient);
        if(tmpServices == -1) erreur("accept");
        
        /*Propagation de celle-ci dans les fils*/
        switch(fork()){
            case -1:
                erreur("fork");
            case 0:
                /*Connexion réussie, traitement des demandes client*/
                /*--On ferme la socket d'écoute inutile dans le processus fils*/
                close(tmpIdSocketEcoute);
                /*--Traitements du serveur*/
                traitement_tmpRequete_client(tmpServices,pTableau);
                /*--On tue le service*/
                close(tmpServices);
                exit(0);
            default:
                /*--On ferme le service inutile dans le père*/
                close(tmpServices);
        }
    }
    
    close(tmpIdSocketEcoute);
    
}


/**
 * Fonction qui transforme un {TableauVoyage} en {char*}
 * @param	{TableauVoyage*} pTableau, le tableau à transformer
 * @return	{char*} le tableau transformé
 */
char* transformer_tableau_en_reponse(TableauVoyage *pTableau, int pTailleTableau) {
    TableauVoyage *tmpTableau=NULL;
    char* reponse = (char*) malloc((5+30+30+5+5+7+100)*calculerTailleTableau(pTableau)*sizeof(char));
    /*Si le tableau est vide réponse négative*/
    if (pTableau == NULL) {
        sprintf(reponse, "Aucun train correspondant à votre recherche");
    }
    else {
        /*On concatène tous les éléments du premier voyage du tableau*/
        sprintf(reponse, "Train numéro %d?Ville Départ : %s?Ville Arrivée : %s?Heure Départ : %s?Heure Arrivée : %s?Prix : %.2f€?@", pTableau->v.numero_train, pTableau->v.ville_depart, pTableau->v.ville_arrive, transformer_horaire_en_string(pTableau->v.heure_depart), transformer_horaire_en_string(pTableau->v.heure_arrive), pTableau->v.prix);
        /*Parcours du tableau*/
        while(pTableau->suivant!=NULL)
        {
            /*Passer au suivant */
            pTableau = pTableau->suivant;
            /*On concatène tous les éléments de chaque voyage du tableau à notre reponse*/
            sprintf(reponse, "%sTrain numéro %d?Ville Départ : %s?Ville Arrivée : %s?Heure Départ : %s?Heure Arrivée : %s?Prix : %.2f€?@", reponse, pTableau->v.numero_train, pTableau->v.ville_depart, pTableau->v.ville_arrive, transformer_horaire_en_string(pTableau->v.heure_depart), transformer_horaire_en_string(pTableau->v.heure_arrive), pTableau->v.prix);
        }
    }
    sprintf(reponse, "%s!%d!", reponse, pTailleTableau);
    return reponse;
}


void recupere_mort_fils(){
    wait(NULL);
}

void erreur(char *p_fonction){
    perror(p_fonction);
    exit(errno);
}

void FermetureServeur(int sig) {
    close(tmpServices);
    Statistiques();
    sleep(5);
    exit(0);
}

void usage(char *p_nom_programme){
    fprintf(stderr,"%s prend 1 paramètre, c'est le numéro de port que prendra la socket du serveur\n", p_nom_programme);
    exit(-1);
}
/**
 * La fonction envoie la requete resultat au client
 * @param {int} pSocket : socket sur lequelle on lit la requete
 * @param {char*} char* : message a envoyer sous forme de string
 * @return void
 **/
void envoie_requete_resultat(int pSocket, char * pRequete)
{
    fflush(stdout);
    fflush(stdin);
    puts(pRequete);
    char tmpRequete[strlen(pRequete)];
    sprintf(tmpRequete, "%s", pRequete);
    int* tmpTailleBuffer = (int*)malloc(sizeof(int));
    *tmpTailleBuffer = strlen(tmpRequete);
    //On envoie la taille du paquet
    fflush(stdout);
    fflush(stdin);
    if (write(pSocket,tmpTailleBuffer,sizeof(int))==-1) erreur("write");
    //On envoie le paquet
    fflush(stdout);
    fflush(stdin);
    if(write(pSocket,tmpRequete,*tmpTailleBuffer)==-1) erreur("write");
}


/**
 * La fonction traite une requete client
 * @param {int} pSocket : socket sur lequelle on lit la requete
 * @param {TableauVoyage} *pTableau : Tableau contenant la liste des voyages
 * @return void
 */
void traitement_tmpRequete_client(int pSocket, TableauVoyage *pTableau){
    char** tmpContenuRequeteRecu;
    char* tmpRequeteAEnvoyer;
    int tmpFin = 0;
    
    /* allocation memoire pour tampon */
    int *taille_tampon=(int*)malloc(sizeof(int));
    
    /* allocation memoire pour requete */
    char* tmpRequete=(char*)malloc(sizeof(char)*(*taille_tampon));

    
    while (!tmpFin) {
        
        if(read(pSocket,taille_tampon,sizeof(int))==-1) erreur("read");
        tmpRequete=(char*)malloc(sizeof(char)*(*taille_tampon));
        fflush(stdout);
        fflush(stdin);
        if(read(pSocket,tmpRequete,*taille_tampon)==-1) erreur("read");
        printfc("\nRequete recu par le serveur: \n", BLANC, FOND_ROUGE);

        
        if(strstr(tmpRequete, "Fin connexion") != NULL)
        {
            printfc("Fin connexion", BLANC, FOND_ROUGE);
        }
        else
        {
            puts(tmpRequete);
        }


    	if(strcmp(tmpRequete, "Fin connexion?")!=0) ecrire(tmpRequete);
    
        /* Transformation de la requete string */
    tmpContenuRequeteRecu = diviseur_chaine(tmpRequete, '?');
    
    /*
     Si l'ID de la commande est egale a trainParHoraireDepart */
    if(strstr(tmpContenuRequeteRecu[0],"trainParHoraireDepart") != NULL)
    {

        /* Recherche de la requete dans la Base de donnee */
        TableauVoyage *tmpResultatTrainParHoraireDepart = rechercherTrainParHoraireDepart(tmpContenuRequeteRecu[1], tmpContenuRequeteRecu[2], tmpContenuRequeteRecu[3], pTableau);
        
        afficher_table(tmpResultatTrainParHoraireDepart);

        /* Transformation du tableau de voyage resultant de la recherche en une string  */
        tmpRequeteAEnvoyer = transformer_tableau_en_reponse(tmpResultatTrainParHoraireDepart, calculerTailleTableau(tmpResultatTrainParHoraireDepart));
        
        printc("Requete a renvoyer au client:\n\n", BLEU);
        
        envoie_requete_resultat(pSocket, tmpRequeteAEnvoyer);

        puts("");
        printc("La reponse a été envoyé au client ..\n\n", BLEU);
        
    }
    /* Si l'ID de la commande est egale a tecrirerainsParTrancheHoraires */
    else if(strstr(tmpContenuRequeteRecu[0],"trainsParTrancheHoraires") != NULL)
    {
        /* Recherche de la requete dans la Base de donnee */
        TableauVoyage *tmpResultatTrainsTrainsParTrancheHoraires = rechercherTrainsParTrancheHoraires(tmpContenuRequeteRecu[1], tmpContenuRequeteRecu[2], tmpContenuRequeteRecu[3], tmpContenuRequeteRecu[4], pTableau);
        
        /* Application de filtre si parametre optiennel specifie */
        if(strstr(tmpContenuRequeteRecu[5],"AUCUN") == NULL )
        {
            tmpResultatTrainsTrainsParTrancheHoraires = filterTrajets(tmpContenuRequeteRecu[5], tmpResultatTrainsTrainsParTrancheHoraires);
        }
        
        printf("\n%d Train(s) trouvé(s)\n", calculerTailleTableau(tmpResultatTrainsTrainsParTrancheHoraires));
        
        afficher_table(tmpResultatTrainsTrainsParTrancheHoraires);
        
        // printf("avant de transformer la reponse\n");
                /* Transformation du tableau de voyage resultant de la recherche en une string  */
        tmpRequeteAEnvoyer = transformer_tableau_en_reponse(tmpResultatTrainsTrainsParTrancheHoraires, calculerTailleTableau(tmpResultatTrainsTrainsParTrancheHoraires));
        // printf("avant d'envoyer\n");
        
        printc("Requete a renvoyer au client:\n\n", BLEU);
        
        envoie_requete_resultat(pSocket, tmpRequeteAEnvoyer);
        
        puts("");
        printc("La reponse a été envoyé au client ..\n\n", BLEU);

        
    }
    /* Si l'ID de la commande est egale a trainsParVilles */
    else if(strstr(tmpContenuRequeteRecu[0],"trainsParVilles") != NULL)
    {
        /* Recherche de la requete dans la Base de donnee */
        TableauVoyage *tmpResultatTrainsParDestionation = rechercherTrainsParDestionation(tmpContenuRequeteRecu[1], tmpContenuRequeteRecu[2], pTableau);
        
        /* Application de filtre si parametre optiennel specifie */
        if(strstr(tmpContenuRequeteRecu[3],"AUCUN") == NULL )
        {
            tmpResultatTrainsParDestionation = filterTrajets(tmpContenuRequeteRecu[3], tmpResultatTrainsParDestionation);
        }
        
        printf("\n%d Train(s) trouvé(s)\n", calculerTailleTableau(tmpResultatTrainsParDestionation));
        
        afficher_table(tmpResultatTrainsParDestionation);
        // printf("avant de transformer la reponse\n");
        /* Transformation du tableau de voyage resultant de la recherche en une string  */
        tmpRequeteAEnvoyer = transformer_tableau_en_reponse(tmpResultatTrainsParDestionation, calculerTailleTableau(tmpResultatTrainsParDestionation));
        // printf("avant d'envoyer\n");
        
        printc("Requete a renvoyer au client:\n\n", BLEU);
        
        envoie_requete_resultat(pSocket, tmpRequeteAEnvoyer);
        
        puts("");
        printc("La reponse a été envoyé au client ..\n\n", BLEU);

        
    }
    else if(strstr(tmpContenuRequeteRecu[0],"Fin connexion") != NULL)
    {
        tmpFin = 1;
    }
        free(tmpRequete);
    }
    
    free(taille_tampon);

}

/*
char*supprimerChar(char*pChaine,char*pChaineSup){
    char*tmpChaine=strstr(pChaine,pChaineSup);
    int N=strlen(tmpChaine);

}*/
