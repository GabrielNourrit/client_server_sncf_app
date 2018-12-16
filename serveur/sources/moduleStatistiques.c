#include <stdlib.h>
#include <stdio.h>
#include "statistiques.h"
#include "types.h"
#include "outil.h"
#include "voyage.h"
#include <unistd.h>
#include <string.h>
#include <math.h>
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


void ecrire(char*pRequete){
FILE* fichier = NULL;


    fichier = fopen("stats.txt", "a");

    
    if (fichier != NULL)

    {
        fputs(pRequete, fichier);
        fputs("\n",fichier);


    }

}


ChaineStats *CreerChaine(char*Chaine,int n){
   ChaineStats * new=(ChaineStats *)malloc(sizeof(ChaineStats));
   new->Chaine=(char*)malloc(strlen(Chaine));
   strcpy(new->Chaine,Chaine);
   new->nombre=n;
   new->suivant=NULL;
   return new;
}

ChaineStats *AjouterChaine(ChaineStats *Tabc,ChaineStats *Chaine){
    ChaineStats *it=Tabc,*new=NULL;
    if(Tabc==NULL)
        return Chaine;
    new=CreerChaine(Chaine->Chaine,Chaine->nombre);
    
    while(it->suivant!=NULL){
        
        it=it->suivant;
    };
    
    it->suivant=new;
    return Tabc;
}


ChaineStats *IncrementerChaine(ChaineStats *TabChaine){
    ChaineStats *c=NULL,*TabRes=NULL;
    
    while(TabChaine!=NULL){
        c=LireChaine(TabChaine,0);
        if(ExisteDansEtIncremente(TabRes,c)){
            TabChaine=SupprimerChaine(TabChaine,0);
        }else
            TabRes=AjouterChaine(TabRes,c);
    }
    return TabRes;
}

int MaximumChaine(ChaineStats *TabChaine){
    //ChaineStats * tmpMaximum =NULL;
    int Mi=0,i=0;
    int max=-1;
    while (TabChaine!=NULL){
        if(TabChaine->nombre>max){
            max=TabChaine->nombre;
            Mi=i;
            //tmpMaximum=LireChaine(TabChaine,0);
        }
        TabChaine=TabChaine->suivant;
        i++;
    }

    return Mi;
}

int ExisteDansEtIncremente(ChaineStats *TabChaine, ChaineStats *Chaine){
    while(TabChaine!=NULL){
        if(strcasecmp(TabChaine->Chaine,Chaine->Chaine)==0){
            TabChaine->nombre++;
            return 1;
        }
        TabChaine=TabChaine->suivant;
    }
    return 0;
}

ChaineStats* LireChaine(ChaineStats *pChaine,int pIndex){
    int tmpIndice;
    
    if(pIndex==0){
        /*si il n'y a qu'une case on la retourne*/
        return CreerChaine(pChaine->Chaine,pChaine->nombre);
    }
    /*on place notre pointeur sur la case*/
    for(tmpIndice=0;tmpIndice<pIndex;tmpIndice++,pChaine=pChaine->suivant) ;
    /*on retourne la case qui nous interesse*/
    return CreerChaine(pChaine->Chaine,pChaine->nombre);
    
}

ChaineStats* SupprimerChaine(ChaineStats *pChaine,int pIndex){
    ChaineStats *tmpIterateur=pChaine,*tmpChaine=NULL;
    int i;
    /*si il y a une case on la supprime et on retourne null*/
    if(pIndex==0){;
        if(pChaine->suivant==NULL){
            free(pChaine);
            return NULL;
        }
        /*sinon on envoie la suite*/
        tmpIterateur=tmpIterateur->suivant;
        free(pChaine);
        return tmpIterateur;
    }
    /*on place notre pointeur sur la case à pop*/
    for(i=0;i<pIndex;i++,tmpIterateur=tmpIterateur->suivant)
        tmpChaine=tmpIterateur;
    /*on lie la case d'avant à celle d'après*/
    tmpChaine->suivant=tmpIterateur->suivant;
    /*on supprime la case retiré*/
    free(tmpIterateur);
    /*on retourne le debut de la liste chainee*/
    return pChaine;
}

ChaineStats* villeDToChaine(TableauVoyage *t){
    //CreerChaine
    ChaineStats *c=NULL,*tabRes=NULL;
    int i=0, taille=calculerTailleTableau(t);
    for(i=0;i<taille;i++){
        c=CreerChaine(t->v.ville_depart,0);
        tabRes=AjouterChaine(tabRes,c);
        t=t->suivant;
    }
    return tabRes;
}

ChaineStats* villeAToChaine(TableauVoyage *t){
    //CreerChaine
    ChaineStats *c=NULL,*tabRes=NULL;
    int i=0, taille=calculerTailleTableau(t);
    for(i=0;i<taille;i++){
        c=CreerChaine(t->v.ville_arrive,0);
        tabRes=AjouterChaine(tabRes,c);
        t=t->suivant;
    }
    return tabRes;
}
double itof(int a){
    double b=a*(double)1.00;
    return b;
}

void afficherChaine(ChaineStats *c, int total){
    if(c==NULL)
        return;
    while(c->suivant!=NULL){
        printf("\n* %s a ete selectione %d fois sur %d soit %0.2f pourcent\n",c->Chaine,c->nombre,total,itof(c->nombre)/itof(total)*100.00);
        c=c->suivant;
    };
        printf(" %s apparut %d fois\n",c->Chaine,c->nombre);
}
int TailleChaine(ChaineStats *Chaine){
    int count=0;
    while(Chaine!=NULL){
        count++;
        Chaine=Chaine->suivant;
    }
    return count;
}



/**
 * La méthode lis le fichier, et remplis le tableau passé en parametre
 * @param {TableauVoyage} pTableau_voyage : tableau à remplir
 * @return {int} renvoie -1 en cas d'erreur, 1 si reussi
 */
TableauVoyage* Lire_stats(TableauVoyage *pTableau_voyage){
    FILE * tmp_fichier;
    char * tmpLigne = NULL;
    size_t tmpLongueur = 0;
    ssize_t tmpTailleLigneCourante;
    int tmpCompteur = 0;
    char** tmpLigneCourante;
    voyage* tmpVoyage;
    horaire h;
    h.heure=0;
    h.minute=0;
    
    /*On ouvre le fichier*/
    tmp_fichier = fopen("stats.txt", "r");
    if (tmp_fichier == NULL){
        fprintf(stderr,"ERREUR OUVERTURE IMPOSSIBLE DU FICHIER\n");
        exit(EXIT_FAILURE);
    }
    
    /*On parcours toutes les lignes du fichier*/
    while ((tmpTailleLigneCourante = getline(&tmpLigne, &tmpLongueur, tmp_fichier)) != -1) {
        tmpLigneCourante=diviseur_chaine(tmpLigne,'?');
        
        if(*tmpLigneCourante){
            
            sscanf(tmpLigneCourante[3],"%d:%d",&h.heure,&h.minute);
            
            /*On crée un voyage et on l'ajoute au tableau*/
            tmpVoyage=creer_voyage(0,tmpLigneCourante[1], tmpLigneCourante[2], h,h,0.0,DEFAULT);
            
            /* On ajoute le voyage au tableau */
            pTableau_voyage = ajouter_voyage(pTableau_voyage,tmpVoyage);
            free(*(tmpLigneCourante));
            tmpCompteur++;
            }
    }
    
    return pTableau_voyage;
}

ChaineStats *Trier(ChaineStats*pChaineNonTrie){
    ChaineStats *tmpTabRes=NULL;
    int tmpIndex;
    while (pChaineNonTrie!=NULL){
        tmpIndex=MaximumChaine(pChaineNonTrie);
        tmpTabRes=AjouterChaine(tmpTabRes,LireChaine(pChaineNonTrie,tmpIndex));
        pChaineNonTrie=SupprimerChaine(pChaineNonTrie,tmpIndex);
                                                                               }
    return tmpTabRes;
    }
char* AtoDuree(horaire pHeure){
    char *Heure=malloc(5);
    sprintf(Heure,"%02d:%02d",pHeure.heure,pHeure.minute);
    return Heure;
}

ChaineStats* HeureToChaine(TableauVoyage *t){
    //CreerChaine
    ChaineStats *c=NULL,*tabRes=NULL;
    int i=0, taille=calculerTailleTableau(t);
    for(i=0;i<taille;i++){
        c=CreerChaine(AtoDuree(t->v.heure_depart),0);
        tabRes=AjouterChaine(tabRes,c);
        t=t->suivant;
    }
    return tabRes;
    
}

void TraiterHdepart(TableauVoyage*TabVoyage){
    ChaineStats *tmpDuree=NULL;
    int total;
    tmpDuree=HeureToChaine(TabVoyage);
    total=TailleChaine(tmpDuree);
    tmpDuree=IncrementerChaine(tmpDuree);
    tmpDuree=Trier(tmpDuree);
    printf("* L' ");printc("horaire ",JAUNE);printf("de départ");
    afficherChaine(tmpDuree,total);
}


void TraiterVilleDepart(TableauVoyage*TabVoyage){
    ChaineStats *tmpVille=NULL;
    int total;
    tmpVille=villeDToChaine(TabVoyage);
    total=TailleChaine(tmpVille);
    tmpVille=IncrementerChaine(tmpVille);
    tmpVille=Trier(tmpVille);
    printf("* La ");printc("ville ",ROUGE);printf("de départ");
    afficherChaine(tmpVille,total);
    /*TODO:: Penser à implementer min/maj dans incrementer 
<<<<<<<<<<<<<Et aussi le comptage du total des villes*/
}
void TraiterVilleArrive(TableauVoyage*TabVoyage){
    ChaineStats *tmpVille=NULL;
    int total=0;
    tmpVille=villeAToChaine(TabVoyage);
    total=TailleChaine(tmpVille);
    tmpVille=IncrementerChaine(tmpVille);
    tmpVille=Trier(tmpVille);
    printf("* La ");printc("ville ",BLEU);printf("d'arrivée");
    afficherChaine(tmpVille,total);
    
}


void Statistiques(){   
    TableauVoyage* t=NULL;
    t=Lire_stats(t);
puts("");
puts("");
    puts("********************************");
    puts("******VILLES DEPART*************");
    puts("********************************");
    TraiterVilleDepart(t);
puts("*");
    puts("********************************");
    puts("");

puts("");
puts("");
    puts("********************************");
    puts("******VILLES ARRIVE*************");
    puts("********************************");
    TraiterVilleArrive(t);
puts("*");
    puts("********************************");

    puts("");
puts("");
puts("");
    puts("********************************");
    puts("******HEURES DEPART*************");
    puts("********************************");
    TraiterHdepart(t);
puts("*");
    puts("********************************");


puts("");
puts("Veuillez Patientez 5 secondes ...");
}
