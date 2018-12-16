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
#include "fonctionsRecherche.h"

/**
 * La fonction recherche un voyage parmis les voyages contenu dans pTableau
 * @param {char*} pVille_depart :
 * @param {char*} pVille_arrive :
 * @param {char*} pHeure_depart : Delimiteur à appliquer sur la chaine
 * @param {TableauVoyage*} pTableau :
 * @return {voyage} struct voyage du voyage en question ou voyage NULL
 */
TableauVoyage* rechercherTrainParHoraireDepart(char* pVille_depart, char* pVille_arrive, char* pHeure_depart, TableauVoyage *pTableau)
{
    const int tmpTailleTableau = calculerTailleTableau(pTableau);
    TableauVoyage *tmpTableau=NULL;
    int* tmpHeure_depart = atoiHeure(pHeure_depart);
    
    for (int i = 0; i<tmpTailleTableau; pTableau = pTableau->suivant, i++) {
        /* Si train trouve a la bonne heure, meme heure meme minute */
        if(strcasecmp(pTableau->v.ville_depart, pVille_depart) == 0 &&
           strcasecmp(pTableau->v.ville_arrive, pVille_arrive) == 0 &&
           pTableau->v.heure_depart.heure == tmpHeure_depart[0] &&
           pTableau->v.heure_depart.minute == tmpHeure_depart[1])
        {
            printf("1 Train trouvé\n");
            //free(*(tmpHeure_depart));
            tmpTableau = ajouter_voyage(tmpTableau, &pTableau->v);
            return tmpTableau;
        }
        /* Si train trouve le plus proche, apres l'heure indique, Si (heure depart demandé == heure depart train) et (minute depart train > minute depart demande)  */
        else if(
                (strcasecmp(pTableau->v.ville_depart, pVille_depart) == 0 &&
                 strcasecmp(pTableau->v.ville_arrive, pVille_arrive) == 0) &&
                pTableau->v.heure_depart.heure == tmpHeure_depart[0] &&
                pTableau->v.heure_depart.minute > tmpHeure_depart[1]
                )
        {
            printf("\n1 Train plus proche trouvé\n");
            //free(tmpHeure_depart);
            return tmpTableau = ajouter_voyage(tmpTableau, &pTableau->v);
        }
        /* Si train trouve le plus proche, apres l'heure indique, Si (heure depart train > heure depart demandé)*/
        else if(
                (strcasecmp(pTableau->v.ville_depart, pVille_depart) == 0 &&
                strcasecmp(pTableau->v.ville_arrive, pVille_arrive) == 0) &&
                (pTableau->v.heure_depart.heure > tmpHeure_depart[0])
                )
        {
            printf("\n1 Train plus proche trouvé\n");
            //free(tmpHeure_depart);
            return tmpTableau = ajouter_voyage(tmpTableau, &pTableau->v);
        }
    }
    printf("\nAucun Train trouvé\n");
    //free(*(tmpTableauHoraireDepart));
    return NULL;
}


/**
 * La fonction recherche tout voyage d'un point a un autre parmis les voyages contenu dans pTableau
 * @param {char*} pVille_depart :
 * @param {char*} pVille_arrive :
 * @param {TableauVoyage*} pTableau :
 * @return {TableauVoyage*} tableau contenant les voyages en question
 */
TableauVoyage* rechercherTrainsParDestionation(char* pVille_depart, char* pVille_arrive, TableauVoyage *pTableau)
{
    const int tmpTailleTableau = calculerTailleTableau(pTableau);
    TableauVoyage *tmpTableau=NULL;
    // int tmpCompteur = 0;
    
    for (int i = 0; i<tmpTailleTableau; pTableau = pTableau->suivant, i++)
    {
        /* Si train avec meme depart et meme dest*/
        if(strcasecmp(pTableau->v.ville_depart, pVille_depart) == 0 &&
           strcasecmp(pTableau->v.ville_arrive, pVille_arrive) == 0)
        {
            // printf("Train trouvé\n");
            tmpTableau = ajouter_voyage(tmpTableau, &pTableau->v);
            // tmpCompteur++;
        }
    }
    
    if(tmpTableau == NULL)
    {
        // printf("Aucun Train trouvé\n");
        return NULL;
    }
    else
    {
        // printf("%d Train(s) trouvé(s)\n", tmpCompteur);
        return tmpTableau;
    }
}

/**
 * La fonction recherche tout voyage d'un point a un autre parmis les voyages contenu dans pTableau
 * @param {char*} pVille_depart :
 * @param {char*} pVille_arrive :
 * @param {char*} pHeure_depart_min :
 * @param {char*} pHeure_depart_max :
 * @param {TableauVoyage*} pTableau :
 * @return {TableauVoyage*} tableau contenant les voyages en question
 */
TableauVoyage* rechercherTrainsParTrancheHoraires(char* pVille_depart, char* pVille_arrive, char* pHeure_depart_min, char* pHeure_depart_max, TableauVoyage *pTableau)
{
    const int tmpTailleTableau = calculerTailleTableau(pTableau);
    TableauVoyage *tmpTableau=NULL;
    // int tmpCompteur = 0;
    
    /* On transforme nos char** en int* pour les comparer avec nos int dans la struct */
    int* tmpHeure_min = atoiHeure(pHeure_depart_min);
    int* tmpHeure_max = atoiHeure(pHeure_depart_max);
    
    /* Calcul ecart (en minutes) entre pHeure_depart_min et pHeure_depart_max */
    int tmpEcartMax = calculHeurePtrPtr(tmpHeure_min, tmpHeure_max);
    // printf("tmpEcartMax = %d\n", tmpEcartMax);

    for (int i = 0; i<tmpTailleTableau; pTableau = pTableau->suivant, i++)
    {
        /* Calcul de l'ecart entre la valeur max et l'heure de la struct parcouru  */
        int tmpEcartValeurEnCours = calculHeureIntPtr(pTableau->v.heure_depart.heure, pTableau->v.heure_depart.minute, tmpHeure_max);
        // printf("heure de depart du train = %d heure d'arriver du train = %d, heure max %d:%d \n", pTableau->v.heure_depart.heure, pTableau->v.heure_depart.minute, tmpHeure_max[0], tmpHeure_max[1]);
        // printf("tmpEcart entre l heure du train et le heure maximum = %d\n", tmpEcartValeurEnCours);

        // tmpEcartValeurEnCours = tmpEcartMax - tmpEcartValeurEnCours;

        /* Si ville depart et ville arrive coincide */
        if (strcasecmp(pTableau->v.ville_depart, pVille_depart) == 0 &&
            strcasecmp(pTableau->v.ville_arrive, pVille_arrive) == 0 &&
            tmpEcartValeurEnCours <= tmpEcartMax &&
            tmpEcartValeurEnCours > 0)
        {
            // printf("Train trouvé\n");
            tmpTableau = ajouter_voyage(tmpTableau, &pTableau->v);
            // tmpCompteur++;
        }
    }

    if(tmpTableau == NULL)
    {
        // printf("Aucun Train trouvé\n");
        return NULL;
    }
    else
    {
        // printf("%d Train(s) trouvé(s)\n", tmpCompteur);
        return tmpTableau;
    }
}

