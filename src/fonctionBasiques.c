
#include "fonctionBasiques.h"

Individu* initIndividus()
{
    Individu* listeI= NULL;
    return listeI;

}



void ajoutIndividu(Individu** listeI,Sexe s,Espece* espece, Temps naissance)
{
    Individu* nouveau;
    nouveau=(Individu*)malloc(sizeof(Individu));
    nouveau->suivant=*listeI;
    *listeI=nouveau;

    espece->population++;

    nouveau->sexe=s;
    nouveau->derniereReproduction=naissance;

    nouveau->direction=Random(0,360);// angle degree
    nouveau->naissance = naissance;
    nouveau->fatigue=0;
    nouveau->sante=100;
    nouveau->stockEau=100;
    nouveau->stockNourriture=100;
    nouveau->espece=espece;
    nouveau->vitesse=0;
    nouveau->id=espece->population;
    nouveau->repos=0;



}



Individu* rechercheParPos(Individu* listeI, Vec pos)
{
    if(listeI==NULL)
    {
        return NULL;
    }
    else
    {
        Individu* courant=listeI;
        do
        {
            if((pos.x==courant->position.x)&&(pos.y==courant->position.y))
            {
            return courant;
            }
            else
            {
                courant=courant->suivant;
            }
        }while(courant !=NULL);
        return NULL;
    }

}

void deplacementAleatoire(Individu* ind)
{
    switch (rand()%50)
    {
        case 0:
            ind->direction += 20;
            break;
        case 1:
            ind->direction -= 20;
            break;
        default:
            break;
    }
}


void regulerVitesse(Individu* ind)
{
    if (ind->vitesse < ind->espece->vitesseMoyenne) // vitesse trop faible
    {
        ind->vitesse += ind->espece->acceleration;
    }
    if (ind->vitesse > ind->espece->vitesseMoyenne) // vitesse trop rapide
    {
        ind->vitesse -= ind->espece->acceleration;
    }
}



bool rayonAction(Vec pos1, Vec pos2)
{
    return((((pos1.x-pos2.x)*(pos1.x-pos2.x))+((pos1.y-pos2.y)*(pos1.y-pos2.y)))<=64);//arbitraire a modifier

}



float distance(Vec pos1, Vec pos2)
{
    return sqrt( ((pos1.x-pos2.x)*(pos1.x-pos2.x)) + ((pos1.y-pos2.y)*(pos1.y-pos2.y)) );
}




float Random (int Min, int Max)
{
    return ((rand() % (Max - Min + 1)) + Min);
}



void supprimerIndividus (Espece *espece)
{
    while(espece->premierIndividu != NULL)
    {
        mortIndividu(espece->premierIndividu);
    }
}

void mortIndividu(Individu* ind)
{
    Individu* p=ind->espece->premierIndividu; //@fixme : erreur de segmentation
    if(ind==p)
    {
        ind->espece->premierIndividu=ind->suivant;
    }
    else
    {
        while(p->suivant!=ind)
        {
            p=p->suivant;
        }
        p->suivant=ind->suivant;
    }
    ind->espece->population--;
    free(ind);

}


void supprimerEspeceEtIndividu (Espece **listeEspece)
{
    Espece *espece, *p;
    Individu *individu, *pi;

    espece=*listeEspece;
    while(espece != NULL)
    {
        p=espece;
        espece=espece->suivante;

        individu=p->premierIndividu;
        while(individu != NULL)
        {
            pi=individu;
            individu=individu->suivant;

            free(pi);
        }
        free(p);
    }
    *listeEspece=NULL;
}


char** initTableauChaineCaractrere ()
{
    char** tab;

    tab=(char**)malloc(sizeof(char*));
    tab[0]=(char*)malloc(TAILLE_MAX*sizeof(char));

    return tab;
}



char** augmenterTailleTabChaineCaractere(int taille, char** tab, int nombre_case_en_plus)
{
    char** tab2;
    int i;
    int nombre_case_nouvtab = 0;

    if(nombre_case_en_plus != 1)
    {
        if(taille == 1)
        {
            nombre_case_nouvtab=nombre_case_en_plus+1;
        }
        else
        {
            nombre_case_nouvtab = nombre_case_en_plus*(((taille-1)/nombre_case_en_plus)+1) + 1;
        }

    }
    else
    {
        nombre_case_nouvtab = taille;
    }

    tab2=(char**)malloc(nombre_case_nouvtab*sizeof(char*));
    for(i=0;i<nombre_case_nouvtab;i++)
    {
        tab2[i]=(char*)malloc(TAILLE_MAX*sizeof(char));
    }

    if(taille > 1)
    {
        for(i=0;i<taille-1;i++)
        {
            strcpy(tab2[i],tab[i]);
        }
    }

    supprimerTabChaineCaractere (tab,taille-1,1);

    return tab2;
}


void supprimerTabChaineCaractere (char** tab,int taille,int nombre_case_en_plus)
{
    int i;
    int nombre_case=0;

    if(nombre_case_en_plus != 1)
    {
        nombre_case=taille+(taille%nombre_case_en_plus);
    }
    else
    {
        nombre_case=taille;
    }

    for(i=0;i<nombre_case;i++)
    {
        free(tab[i]);
    }
    free(tab);
}



