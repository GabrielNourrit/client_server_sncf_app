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
#include "fonctionsRequeteClient.h"
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



void viderBuffer()
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

/**
 * La fonction clear le buffer pour pouvoir par la suite refaire un gets sans avoir des caracteres non souhaité
 *
 **/
void clean(const char *buffer, FILE *fp)
{
    char *p = strchr(buffer,'\n');
    if (p != NULL)
        *p = 0;
    else
    {
        int c;
        while ((c = fgetc(fp)) != '\n' && c != EOF);
    }
}


/**
 * Fonction qui crée une requête en demandant au client les infos
 * @parar	void
 * @return	{char*} requete, la nouvelle requete crée
 */
char* creer_requete(char* requete) {
    //char* requete = (char*) malloc((24+30+30+5+5+5+6)*sizeof(char)); //24 -> type, 30 -> ville, 5 -> horaire, 4 -> option, 6 -> delimiteurs
    char* type = (char*) malloc(sizeof(char));
    char* ville_depart = (char*) malloc(30*sizeof(char));
    char* ville_arrivee = (char*) malloc(30*sizeof(char));
    char* heure_depart = (char*) malloc(5*sizeof(char));
    char* heure_arrivee = (char*) malloc(30*sizeof(char));
    int erreur = 0;
    
    printf("Quel type ? (1 -> depart, 2 -> tranche, 3 -> villes) : \n");
    do {
        fflush(stdin);
        fgets(type, 30*sizeof(char), stdin);
        //viderBuffer();
        clean(type, stdin);
        if ((strcmp(type, "1") !=0) && (strcmp(type, "2") !=0) && (strcmp(type, "3") !=0)) {
            erreur = 1;
            printf("Merci de rentrer 1,2 ou 3 (1 -> depart, 2 -> tranche, 3 -> villes) : \n");
        }
        else
            erreur = 0;
    } while(erreur);
    /*trier les trains par départ*/
    if (strcmp(type, "1") == 0) {
        printf("Ville de départ (30 caractères max) : \n");
        do {
            fflush(stdin);
            fgets(ville_depart, 30*sizeof(char), stdin);
            clean(ville_depart, stdin);
            if (strlen(ville_depart) > 30 || !EstVilleValide(ville_depart)) {
                if (!EstVilleValide(ville_depart))
                    printf("Merci de rentrer une ville valide \n");
                if(strlen(ville_depart) > 30)
                    printf("30 caractères maxi \n");
                erreur = 1;
            }
            else
                erreur = 0;
        } while(erreur);
        
        
        printf("Ville de d'arrivée (30 caractères max) : \n");
        do {
            fflush(stdin);
            fgets(ville_arrivee, 30*sizeof(char), stdin);
            clean(ville_arrivee, stdin);
            if (strlen(ville_arrivee) > 30 || !EstVilleValide(ville_arrivee)) {
                if (!EstVilleValide(ville_arrivee))
                    printf("Merci de rentrer une ville valide \n");
                if(strlen(ville_arrivee) > 30)
                    printf("30 caractères maxi \n");
                erreur = 1;
            }
            else
                erreur = 0;
        } while(erreur);
        
        int h,m,test_heure;
        printf("Heure de départ : (Format hh:mm) : \n");
        do {
            fflush(stdin);
            fgets(heure_depart, 30*sizeof(char), stdin);
            clean(heure_depart, stdin);
            /*Test du format hh:mm*/
            test_heure=sscanf(heure_depart, "%d:%d", &h,&m);
            if (h<0 || h>24 || m<0 || m>60 || (test_heure !=2)) {
                printf("Merci de rentrer un horaire au format hh:mm : \n");
                erreur = 1;
            }
            else
                erreur = 0;
        } while(erreur);
        
        /*On crée notre requete avec ce qu'a rentré l'utilisateur*/
        sprintf(requete, "%s?%s?%s?%s?AUCUN?", "trainParHoraireDepart", ville_depart, ville_arrivee, heure_depart);
    }
    /*trier les trains par tranche horaire*/
    if (strcmp(type, "2") == 0) {
        /*Tant que l'utilisateur donne un nom de ville avec plus de 30 caractères*/
        printf("Ville de départ (30 caractères max) : \n");
        do {
            fflush(stdin);
            fgets(ville_depart, 30*sizeof(char), stdin);
            clean(ville_depart, stdin);
            if (strlen(ville_depart) > 30 || !EstVilleValide(ville_depart)) {
                if (!EstVilleValide(ville_depart))
                    printf("Merci de rentrer une ville valide \n");
                if(strlen(ville_depart) > 30)
                    printf("30 caractères maxi \n");
                //scanf("%s", ville_depart);
                erreur = 1;
            }
            else
                erreur = 0;
        } while(erreur);
        
        
        /*Tant que l'utilisateur donne un nom de ville avec plus de 30 caractères*/
        printf("Ville de d'arrivée (30 caractères max) : \n");
        do {
            fflush(stdin);
            fgets(ville_arrivee, 30*sizeof(char), stdin);
            clean(ville_arrivee, stdin);
            if (strlen(ville_arrivee) > 30 || !EstVilleValide(ville_arrivee)) {
                if (!EstVilleValide(ville_arrivee))
                    printf("Merci de rentrer une ville valide \n");
                if(strlen(ville_arrivee) > 30)
                    printf("30 caractères maxi \n");
                erreur = 1;
            }
            else
                erreur = 0;
        } while(erreur);
        
        
        
        int h,m,test_heure;
        printf("Heure min : (Format hh:mm) : \n");
        do {
            fflush(stdin);
            fgets(heure_depart, 30*sizeof(char), stdin);
            clean(heure_depart, stdin);
            /*Test du format hh:mm*/
            test_heure=sscanf(heure_depart, "%d:%d", &h,&m);
            if (h<0 || h>24 || m<0 || m>60 || (test_heure !=2)) {
                printf("Merci de rentrer un horaire au format hh:mm : \n");
                erreur = 1;
            }
            else
                erreur = 0;
        } while(erreur);
        
        
        printf("Heure max : (Format hh:mm) : \n");
        do {
            fflush(stdin);
            fgets(heure_arrivee, 30*sizeof(char), stdin);
            clean(heure_arrivee, stdin);
            /*Test du format hh:mm*/
            test_heure=sscanf(heure_arrivee, "%d:%d", &h,&m);
            /*Tant que l'utilisateur une mauvaise heure (pas de la forme hh:mm, heure < 0 ou heure > 24, minute < 0 ou minute > 60*/
            if (h<0 || h>24 || m<0 || m>60 || (test_heure !=2) || (calculHeurePtrPtr(atoiHeure(heure_depart), atoiHeure(heure_arrivee)) <= 0)) {
                if (calculHeurePtrPtr(atoiHeure(heure_depart), atoiHeure(heure_arrivee)) <= 0)
                    printf("Merci de rentrer une heure d'arrivée supérieure à l'heure de départ : \n");
                else
                    printf("Merci de rentrer un horaire au format hh:mm : \n");
                erreur = 1;
            }
            else
                erreur = 0;
        } while(erreur);
        
        /*On crée notre requete avec ce qu'a rentré l'utilisateur*/
        sprintf(requete, "%s?%s?%s?%s?%s?AUCUN?", "trainsParTrancheHoraires", ville_depart, ville_arrivee, heure_depart, heure_arrivee);
    }
    /*trier les trains par villes*/
    if (strcmp(type, "3") == 0) {
        /*Tant que l'utilisateur donne un nom de ville avec plus de 30 caractères*/
        printf("Ville de départ (30 caractères max) : \n");
        do {
            fflush(stdin);
            fgets(ville_depart, 30*sizeof(char), stdin);
            clean(ville_depart, stdin);
            if (strlen(ville_depart) > 30 || !EstVilleValide(ville_depart)) {
                if (!EstVilleValide(ville_depart))
                    printf("Merci de rentrer une ville valide \n");
                if(strlen(ville_depart) > 30)
                    printf("30 caractères maxi \n");
                //scanf("%s", ville_depart);
                erreur = 1;
            }
            else
                erreur = 0;
        } while(erreur);
        
        
        /*Tant que l'utilisateur donne un nom de ville avec plus de 30 caractères*/
        printf("Ville de d'arrivée (30 caractères max) : \n");
        do {
            fflush(stdin);
            fgets(ville_arrivee, 30*sizeof(char), stdin);
            clean(ville_arrivee, stdin);
            if (strlen(ville_arrivee) > 30 || !EstVilleValide(ville_arrivee)) {
                if (!EstVilleValide(ville_arrivee))
                    printf("Merci de rentrer une ville valide \n");
                if(strlen(ville_arrivee) > 30)
                    printf("30 caractères maxi \n");
                erreur = 1;
            }
            else
                erreur = 0;
        } while(erreur);
        
        /*On crée notre requete avec ce qu'a rentré l'utilisateur*/
        sprintf(requete, "%s?%s?%s?AUCUN?", "trainsParVilles", ville_depart, ville_arrivee);
    }
    
    /*On libère la mémoire*/
    free(type);
    free(ville_depart);
    free(ville_arrivee);
    free(heure_depart);
    free(heure_arrivee);
    
    return requete;
}

/**
 * Fonction qui crée une requête de demande de filtrage en demandant au client les infos
 * @parar	{char*} pRequetePrecedente, requete precedemment envoyée par le client
 * @return	{char*} requete, la nouvelle requete de filtrage crée
 */
char* creer_requete_filtre(char* pRequetePrecedente) {
    char* tmpRequete;
    tmpRequete = (char*)malloc(strlen(pRequetePrecedente)*sizeof(char));
    int erreur = 0;
    
    sprintf(tmpRequete, "%s", pRequetePrecedente);
    char** requete = diviseur_chaine(tmpRequete, '?');
    
    if (strcmp(requete[0], "trainParHoraireDepart") != 0) {
        char* oui_non = (char*) malloc(3*sizeof(char));
        char* filtre = (char*) malloc(5*sizeof(char));
        
        /*Tant que l'utilisateur donne un nom de ville avec plus de 30 caractères*/
        printf("Voulez-vous filtrer ou trier les résulats ? (oui, non ou tri) \n");
        do {
            fflush(stdin);
            fgets(oui_non, 30*sizeof(char), stdin);
            clean(oui_non, stdin);
            if ((strcmp(oui_non, "oui") != 0) && (strcmp(oui_non, "non") && (strcmp(oui_non, "tri")) != 0)) {
                printf("Merci de dire oui, non ou tri \n");
                erreur = 1;
            }
            else
                erreur = 0;
        } while(erreur);
        
        
        /*Si l'utilisateur veut filtrer*/
        if (strcmp(oui_non, "oui") == 0) {
            //requete = (char*) malloc((strlen(pRequetePrecedente)+5)*sizeof(char));
            
            /*Tant que l'utilisateur donne un nom de ville avec plus de 30 caractères*/
            printf("Par quoi voulez-vous filtrez ? (prix ou durée)\n");
            do {
                fflush(stdin);
                fgets(filtre, 30*sizeof(char), stdin);
                clean(filtre, stdin);
                if ((strcmp(filtre, "prix") != 0) && (strcmp(filtre, "durée") != 0) && (strcmp(filtre, "duree") != 0)) {
                    printf("Merci choisir prix ou durée \n");
                    erreur = 1;
                }
                else
                    erreur = 0;
            } while(erreur);
            
            /*Si l'utilisateur a choisi prix*/
            if (strcmp(filtre, "prix") == 0) {
                /*On remplace l'ancienne option par prix*/
                pRequetePrecedente[strlen(pRequetePrecedente)-6] = '\0';
                sprintf(pRequetePrecedente, "%s%s",pRequetePrecedente, "PRIX?");
            }
            /*Si l'utilisateur a choisi durée*/
            if ((strcmp(filtre, "durée") == 0) || strcmp(filtre, "duree") == 0) {
                /*On remplace l'ancienne option par durée*/
                pRequetePrecedente[strlen(pRequetePrecedente)-6] = '\0';
                sprintf(pRequetePrecedente, "%s%s", pRequetePrecedente, "DUREE?");
            }
        }
        
        /*Si l'utilisateur veut trier*/
        if (strcmp(oui_non, "tri") == 0) {
            printf("Par quoi voulez-vous trier ? (prix ou durée)\n");
            do {
                fflush(stdin);
                fgets(filtre, 30*sizeof(char), stdin);
                clean(filtre, stdin);
                if ((strcmp(filtre, "prix") != 0) && (strcmp(filtre, "durée") != 0) && (strcmp(filtre, "duree") != 0)) {
                    printf("Merci choisir prix ou durée \n");
                    erreur = 1;
                }
                else
                    erreur = 0;
            } while(erreur);
            
            /*Si l'utilisateur a choisi prix*/
            if (strcmp(filtre, "prix") == 0) {
                /*On remplace l'ancienne option par tprix*/
                pRequetePrecedente[strlen(pRequetePrecedente)-6] = '\0';
                sprintf(pRequetePrecedente, "%s%s",pRequetePrecedente, "TPRIX?");
            }
            /*Si l'utilisateur a choisi durée*/
            if ((strcmp(filtre, "durée") == 0) || strcmp(filtre, "duree") == 0) {
                /*On remplace l'ancienne option par tdurée*/
                pRequetePrecedente[strlen(pRequetePrecedente)-6] = '\0';
                sprintf(pRequetePrecedente, "%s%s", pRequetePrecedente, "TDUREE?");
            }
        }
        
        /*On libère la mémoire*/
        free(oui_non);
        free(filtre);
    }
    
    return pRequetePrecedente;
}



/**
 * Fonction qui verifie si un caractere est une lettre de l'alphabet
 * @parar	{char} pLettre
 * @return	{int} retourne 1(vrai) ou 0(faux) celon si pLettre est une lettre
 */
int EstLettre(char pLettre){
    return ((pLettre>='A' && pLettre<='Z') || (pLettre>='a' && pLettre<='z'));
}
/**
 * Fonction qui verifie que le nom d'une ville est valide
 * On veut que çà contiennent des lettres ou des tiret/espaces si ces derniers sont entourés de lettres
 * La taille minimal est de 3 lettres
 * @parar	{char*} pChaine à verifier
 * @return	{int} retourne 1(vrai) ou 0(faux) celon si pChaine est correcte ou non.
 */
int EstVilleValide(char* pChaine){
    const int n = strlen(pChaine);
    int i,j,k;
    /*il y a forcemment 3 caracteres*/
    /*Si la premiere lettre ou la derniere n'est pas une lettre c'est pas bon*/
    if((!EstLettre(pChaine[0]) || !EstLettre(pChaine[n-1]))|| n<3)
        return 0;
    
    /*+ Grande chaine*/
    for(i=0,j=1,k=2;k<n;j++,i++,k++){
        /*tiret au milieu non entouré*/
        if((!EstLettre(pChaine[i]) || !EstLettre(pChaine[k])) && (pChaine[j]=='-' || pChaine[j]==' '))
            return 0;
    }
    return 1;
}

/**
 * La fonction transforme un char* "heure:minute" en int* {heure, minute}
 * @param {char*} pHeure :
 * @return {int*} pointeur de int contenant les valeur dans pHeure
 */
int* atoiHeure(char* pHeure)
{
    char* tmpHeure = (char*) malloc(strlen(pHeure)*sizeof(char));
    sprintf(tmpHeure, "%s", pHeure);
    char** tmpTableauHoraire;
    int* tmpTableauResult;
    tmpTableauResult = (int*)malloc(2*sizeof(int));
    
    tmpTableauHoraire = diviseur_chaine(tmpHeure,':');
    
    tmpTableauResult[0] = atoi(tmpTableauHoraire[0]);
    tmpTableauResult[1] = atoi(tmpTableauHoraire[1]);
    
    free(tmpHeure);
    return tmpTableauResult;
}



/**
 * La méthode découpe la chaine de caractere à partir d'un délimiteur passé en parametre
 * @param {char*} pChaine : Chaine de caractere à traiter
 * @param {char} pDelimiteur : Delimiteur à appliquer sur la chaine
 * @return {char**} tableau de chaine de caracteres separé par le delimiteur et se finissant par (char*)0
 */
char** diviseur_chaine(char* pChaine, const char pDelimiteur)
{
    char** tmpResultat = 0;
    size_t tmpCompteur = 0;
    char* tmpChaine = pChaine;
    char* tmpLigneCourante = 0;
    char tmpDelimiteur[2];
    size_t tmpId;
    char* token;
    tmpDelimiteur[0] = pDelimiteur;
    tmpDelimiteur[1] = 0;
    
    /*On compte le nombre de chaine séparé par un délimiteur*/
    while (*tmpChaine)
    {
        if (pDelimiteur == *tmpChaine)
        {
            tmpCompteur++;
            tmpLigneCourante = tmpChaine;
        }
        tmpChaine++;
    }
    
    /*Si le pointeur se trouve dans la chaine il reste un mot à prendre en compte*/
    tmpCompteur += tmpLigneCourante < (pChaine + strlen(pChaine) - 1);
    /*Puis on ajoute un pour le mot 0*/
    tmpCompteur++;
    /*On alloue la mémoire au tableau resultat celon le nombre de mots*/
    tmpResultat = malloc(sizeof(char*) * tmpCompteur);
    
    tmpId  = 0;
    token = strtok(pChaine, tmpDelimiteur);
    
    /*tant qu'on a des mots on remplit notre tableau*/
    while (token)
    {
        *(tmpResultat + tmpId++) = strdup(token);
        token = strtok(0, tmpDelimiteur);
    }
    /*on ajoute le mot 0*/
    *(tmpResultat + tmpId) = 0;
    
    return tmpResultat;
}


/**
 * La fonction calcul l'ecart entre 2 horaires differents
 * @param {int*} pHeureMin :
 * @param {int*} pHeureMax :
 * @return {int} retourn la difference convertie en minutes
 */
int calculHeurePtrPtr(int* pHoraireMin, int* pHoraireMax)
{
    return 60*(pHoraireMax[0]-pHoraireMin[0]) + (pHoraireMax[1]-pHoraireMin[1]);
}


/**
 * La fonction ecrit des données pour le serveur
 * @param {int} pSocket : Socket d'ecoute du serveur
 * @return void
 */
void envoie_requete_serveur(int pSocket){
    //Ecrire un while pour le buffer pour qu'il transmete tout le message
    //char* tmpMessage = "Je me présente je m'appel Heny, je voudrais bien réussir ma vie... SALUT JIMMY";
    char* tmpMessage = (char*) malloc(100*sizeof(char));
    char* tmpMessagePrec;
    int* tmpTailleBuffer = (int*)malloc(sizeof(int));
    int fin = 0;
    char* oui_non = (char*) malloc(4*sizeof(char));
    int *taille_tampon=(int*)malloc(sizeof(int));
    char* tmpReponse;
    while(fin == 0) {
        tmpMessage = creer_requete(tmpMessage);
        *tmpTailleBuffer = strlen(tmpMessage);
        //On envoie la taille du paquet
        write(pSocket,tmpTailleBuffer,sizeof(int));
        //On envoie le paquet
        write(pSocket,tmpMessage,*tmpTailleBuffer);
        
        /*read réponse du serveur */
        if(read(pSocket,taille_tampon,sizeof(int))==-1) erreur("read");
        tmpReponse=(char*)malloc(sizeof(char)*(*taille_tampon));
        if(read(pSocket,tmpReponse,*taille_tampon)==-1) erreur("read");
        // puts(tmpReponse);
        
        char* tmpReponseAAfficher = tmpReponse;
        afficher_requete_resultat(tmpReponseAAfficher);
        // ###############################################################################################################
        //Requete filtre
        tmpMessagePrec = (char*) malloc(strlen(tmpMessage)*sizeof(char));
        //Sauvegarde de la requete envoyé
        sprintf(tmpMessagePrec, "%s", tmpMessage);
        
        
        if (strstr(tmpReponse,"Aucun train correspondant à votre recherche") == NULL) {
            tmpMessage = creer_requete_filtre(tmpMessage);
            //Si la requete a été modifié l'utilsateur veut filtrer
            if (strcmp(tmpMessage,tmpMessagePrec) != 0) {
                *tmpTailleBuffer = strlen(tmpMessage);
                //On envoie la taille du paquet
                // printf("je renvoie une requete");
                if(write(pSocket,tmpTailleBuffer,sizeof(int))==-1) erreur("write");;
                //On envoie le paquet
                if(write(pSocket,tmpMessage,*tmpTailleBuffer)==-1) erreur("write");
                
                /*read réponse du serveur */
                if(read(pSocket,taille_tampon,sizeof(int))==-1) erreur("read");
                tmpReponse=(char*)malloc(sizeof(char)*(*taille_tampon));
                if(read(pSocket,tmpReponse,*taille_tampon)==-1) erreur("read");
                afficher_requete_resultat(tmpReponse);
                // puts(tmpReponse);
            }
        }
        else
            puts("Aucun train correspondant à votre recherche\n");
        printf("Voulez-vous quitter ? (oui ou non) \n");
        scanf("%s", oui_non);
        /*Tant que l'utilisateur ne répond pas oui ou non on lui redemande*/
        while ((strcmp(oui_non, "oui") != 0) && (strcmp(oui_non, "non") != 0) ) {
            printf("Merci de dire oui ou non \n");
            scanf("%s", oui_non);
        }
        if (strcmp(oui_non, "oui") == 0) {
            fin = 1;
            //On envoie la taille du paquet
            sprintf(tmpMessage, "%s","Fin connexion?");
            *tmpTailleBuffer = strlen(tmpMessage);	
            if(write(pSocket,tmpTailleBuffer,sizeof(int))==-1) erreur("write");
            //On envoie le paquet
            if(write(pSocket,tmpMessage,*tmpTailleBuffer)==-1) erreur("write");
        }
        viderBuffer();		
    }
    
    free(tmpMessage);
    free(tmpMessagePrec);
    free(oui_non);
    free(tmpTailleBuffer);
    free(taille_tampon);
    free(tmpReponse);				
}

/*
 * La fonction parse et affiche la requete en parametre
 * @param {char*} pReponse : Requete a afficher sous forme de string
 * @return void
 */
void afficher_requete_resultat(char* pReponse)
{

    char** tmpReponseDecomposeParam;
    char** tmpReponseDecomposeLigne;
    char** tmpReponseDecomposeInfo;

    
    tmpReponseDecomposeParam = diviseur_chaine(pReponse,'!');
    int tmpTailleTableau = atoi(tmpReponseDecomposeParam[1]);
    tmpReponseDecomposeLigne = diviseur_chaine(tmpReponseDecomposeParam[0],'@');
    
    puts("");

    for (int i =0; i< tmpTailleTableau; i++)
    {
        // puts(tmpReponseDecomposeLigne[i]);
        tmpReponseDecomposeInfo = diviseur_chaine(tmpReponseDecomposeLigne[i],'?');
        
        printfc(tmpReponseDecomposeInfo[0] , ROUGE, FOND_JAUNE);
        puts("");
        printfc(tmpReponseDecomposeInfo[1] , BLEU, FOND_VERT);
        puts("");
        printfc(tmpReponseDecomposeInfo[2] , MAGENTA, FOND_VERT);
        puts("");
        printfc(tmpReponseDecomposeInfo[3] , BLEU, FOND_VERT);
        puts("");
        printfc(tmpReponseDecomposeInfo[4] , MAGENTA, FOND_VERT);
        puts("");
        printfc(tmpReponseDecomposeInfo[5] , BLANC, FOND_ROUGE);
        puts("");

        puts("");
        
    }
}











