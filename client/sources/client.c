#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include "client.h"

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



int tmpIdSocketEcriture;

int main(int argc, char*argv[]){
    /*récupération sur la ligne de cmd*/
    if(argc!=3) usage(argv[0]);
    const char* NOM_SERV = argv[1];
    const int PORT_SOURCE = atoi(argv[2]);

    /*on regarde si les choses passé en argument ont un minimum de cohérence*/
    if((NOM_SERV[0]<'A' && NOM_SERV[0]>'Z') || (NOM_SERV[0]<'a' && NOM_SERV[0]>'z')|| PORT_SOURCE==0) usage(argv[0]);
    
    requete_client(NOM_SERV, PORT_SOURCE);
    
    puts("Au revoir");
    
    exit(0);
}

void requete_client(const char* NOM_SERV, const int PORT_SOURCE) {
    struct sockaddr_in tmpAdresseServeur;
    struct hostent *tmpIpServeur;
    
    /* Gestionnaire de signal Controle + C => SIGINT */
    if(signal(SIGINT,FermetureClient) == SIG_ERR) erreur("signal");
    
    /*Création de la socket qui va nous permettre d'écrire*/
    tmpIdSocketEcriture=socket(AF_INET, SOCK_STREAM, 0);
    if(tmpIdSocketEcriture==-1) erreur("socket");
    
    /*configuration pour la liaison adresse du serveur et socket client*/
    tmpAdresseServeur.sin_family = AF_INET;
    tmpAdresseServeur.sin_port = htons(PORT_SOURCE);
    tmpIpServeur=gethostbyname(NOM_SERV);
    memcpy(&tmpAdresseServeur.sin_addr.s_addr,tmpIpServeur->h_addr,tmpIpServeur->h_length);
    
    /*connexion au serveur*/
    if(connect(tmpIdSocketEcriture, (struct sockaddr *)&tmpAdresseServeur,sizeof(struct sockaddr_in))==-1) erreur("connect");
    /*nous sommes connecté, traitement*/
    envoie_requete_serveur(tmpIdSocketEcriture);
    /*on ferme la socket desormais inutile*/
    close(tmpIdSocketEcriture);
    
}

/**
 * La fonction renvoie l'erreur passé en parametre
 * @param {char*} *pFonction : Erreur à renvoyer
 * return void
 */
void erreur(char *pFonction){
    perror(pFonction);
    exit(errno);
}

/**
 * La fonction gere la fermeture du client avec Controle C
 * @param {int} sig : numéro du signal envoyé
 */
void FermetureClient(int sig) {
    char * tmpMessage = "Fin connexion?";
    //On envoie la taille du paquet
    unsigned long *tmpTailleBuffer = (unsigned long *) malloc(sizeof(unsigned long));
    *tmpTailleBuffer = strlen(tmpMessage);
    if(write(tmpIdSocketEcriture,tmpTailleBuffer,sizeof(int))==-1) erreur("write");
    //On envoie le paquet
    if(write(tmpIdSocketEcriture, tmpMessage, *tmpTailleBuffer)==-1) erreur("write");
    close(tmpIdSocketEcriture);
    free(tmpTailleBuffer);
    exit(0);
}


/**
 * La fonction ecrit un message d'erreur en cas de parametre incorrect
 * @param {char*} *pNomProgramme : Nom du programme
 * return void
 */
void usage(char *pNomProgramme){
    fprintf(stderr,"%s prend 2 paramètres, <nomserveur> <numeroport>\n le nom du serveur doit commencer par a-z ou A-Z.\n le numero de port doit etre entier\n", pNomProgramme);
    exit(-1);
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



