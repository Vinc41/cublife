

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <math.h>
#include "cube.h"




void actualiserCubType(CubType* typeCub)
{
    cubDebut(typeCub);
    while (typeCub->courant != NULL)
    {
        if (typeCub->proie != NULL)
            manger(typeCub);
        dupliquerCub(typeCub);
        avancer(typeCub);
        orienter(typeCub);
        typeCub->courant->posEcran.x = typeCub->courant->pos.x;
        typeCub->courant->posEcran.y = typeCub->courant->pos.y;
        SDL_BlitSurface(typeCub->surface, NULL, SDL_GetVideoSurface(), &(typeCub->courant->posEcran));
        supprimerCub(typeCub);
        cubSuivant(typeCub);
    }
}



void cubDebut(CubType* typeCube)
{
    typeCube->courant = typeCube->premier;
    typeCube->precedent = typeCube->premier;
}


void cubSuivant(CubType* typeCube)
{
    typeCube->precedent = typeCube->courant;
    if(typeCube->courant!=NULL)
        typeCube->courant = typeCube->courant->suivant;
}



void initCubType(CubType* typeCube, int nombre, Uint32 couleur)
{
    int i;
    typeCube->surface = NULL;
    typeCube->couleur = couleur;
    typeCube->probChangementDirection = 30;
    typeCube->vitesse = 2;
    typeCube->ricoche = 1;
    typeCube->dureeVieMin = 5000;
    typeCube->dureeVieMax = 10000;
    typeCube->delaiDuplicationMin = 2000;
    typeCube->delaiDuplicationMax = 5000;
    typeCube->proie = NULL;

    typeCube->premier = malloc(sizeof(Cub));
    typeCube->premier->pos.x = rand()%(SDL_GetVideoSurface()->w-GRANDEUR_CUBE);
    typeCube->premier->pos.y = rand()%(SDL_GetVideoSurface()->h-GRANDEUR_CUBE);
    typeCube->premier->direction.x = 1;
    typeCube->premier->direction.y = 1;
    typeCube->premier->dateNaissance = SDL_GetTicks();
    typeCube->premier->dateDuplication = SDL_GetTicks();
    typeCube->premier->stockNourriture = 1000;
    typeCube->premier->suivant = NULL;

    typeCube->courant = typeCube->premier;

    for (i=1; i<nombre; i++)
    {
        ajouterCub(typeCube);
    }

    typeCube->surface = SDL_CreateRGBSurface(SDL_HWSURFACE, GRANDEUR_CUBE, GRANDEUR_CUBE, 32, 0, 0, 0, 0);

    SDL_FillRect(typeCube->surface, NULL, typeCube->couleur);
}




void ajouterCub(CubType* typeCube)
{
        Cub *nouveau = malloc(sizeof(Cub));
        nouveau->pos.x = rand()%(SDL_GetVideoSurface()->w-GRANDEUR_CUBE);
        nouveau->pos.y = rand()%(SDL_GetVideoSurface()->h-GRANDEUR_CUBE);
        nouveau->direction.x = 1;
        nouveau->direction.y = 1;
        nouveau->dateNaissance = SDL_GetTicks();
        nouveau->dateDuplication = SDL_GetTicks();
        nouveau->stockNourriture = 1000;

        nouveau->suivant = typeCube->premier;
        typeCube->premier = nouveau;
}




void dupliquerCub(CubType* type)
{
    if (SDL_GetTicks() - type->courant->dateDuplication  >  type->delaiDuplicationMin + rand()%(type->delaiDuplicationMax-type->delaiDuplicationMin) )
    {
        Cub *nouveau = malloc(sizeof(Cub));
        nouveau->pos.x = type->courant->pos.x;
        nouveau->pos.y = type->courant->pos.y;
        nouveau->direction.x = type->courant->direction.x;
        nouveau->direction.y = type->courant->direction.y;
        nouveau->dateNaissance = SDL_GetTicks();
        nouveau->dateDuplication = SDL_GetTicks();
        nouveau->stockNourriture = 1000;

        nouveau->suivant = type->premier;
        type->premier = nouveau;

        type->courant->dateDuplication = SDL_GetTicks();
    }
}


void supprimerCub(CubType* type)
{
    if (SDL_GetTicks() - type->courant->dateNaissance  >  type->dureeVieMin + rand()%(type->dureeVieMax-type->dureeVieMin))
    {
        if (type->precedent == type->courant)
        {
            type->courant = type->courant->suivant;
            free(type->precedent);
            type->precedent = type->courant;
            type->premier = type->courant;
        }
        else
        {
            /*type->courant = type->courant->suivant; //code alternatif
            free(type->precedent->suivant);
            type->precedent->suivant = type->courant;*/
            type->precedent->suivant = type->courant->suivant;
            free(type->courant);
            type->courant = type->precedent->suivant;
        }
    }
}



void manger(CubType* predateur)
{
    if (predateur->courant->stockNourriture < 50)
    {
        fprintf(stderr, "Faim");
        CubType *proie = predateur->proie;
        cubDebut(proie);

        while (proie != NULL)
        {
            if ( norme(predateur->courant->pos, proie->courant->pos) < 10.0 )
            {
                if (proie->precedent == proie->courant)
                {
                    proie->courant = proie->courant->suivant;
                    free(proie->precedent);
                    proie->precedent = proie->courant;
                    proie->premier = proie->courant;
                }
                else
                {
                    /*proie->courant = proie->courant->suivant; //code alternatif
                    free(proie->precedent->suivant);
                    proie->precedent->suivant = proie->courant;*/
                    proie->precedent->suivant = proie->courant->suivant;
                    free(proie->courant);
                    proie->courant = proie->precedent->suivant;
                }
            }

            cubSuivant(proie);
        }


    }
    predateur->courant->stockNourriture--;
}


float norme(Vec2 pos1, Vec2 pos2)
{
    Vec2 distance = { abs(pos1.x-pos2.x) , abs(pos1.y-pos2.y) } ;
    return sqrt(distance.x*distance.x + distance.y*distance.y);
}


