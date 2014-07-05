//A FAIRE - Les rouges mangent les bleus


#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <time.h>
#include "main.h"


int main(int argc, char *argv[])
{
    SDL_Surface *ecran = NULL;
    SDL_Event event;
    unsigned int continuer = 1 , tempsActuel = SDL_GetTicks(), tempsPrecedent = tempsActuel, tempsPasse = 0, limiteMilliSec = 20;
    TTF_Font *police = NULL;
    CubType rouge, bleu;

    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if(TTF_Init() == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    police = TTF_OpenFont("Harabara.ttf", 20);
    if(police == NULL)
    {
        fprintf(stderr, "Erreur : la police n'a pas ete trouvee.\n");
        exit(EXIT_FAILURE);
    }

    ecran = SDL_SetVideoMode(1200, 800, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
    if (ecran == NULL)    {
        fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }


    SDL_WM_SetCaption("Simulateur", NULL);

    srand((unsigned)time(NULL));



    initCubType(&rouge, 1, SDL_MapRGB(ecran->format, 255, 0, 0));
    rouge.probChangementDirection = 2;
    initCubType(&bleu, 1, SDL_MapRGB(ecran->format, 0, 0, 255));
    rouge.proie = &bleu;

    while (continuer)
    {
        SDL_PollEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        continuer = 0;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }

        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 17, 160, 100));

        actualiserCubType(&rouge);
        actualiserCubType(&bleu);

        affichageFPS(tempsPasse, police);

        SDL_Flip(ecran);

        tempsActuel = SDL_GetTicks();
        tempsPasse = tempsActuel - tempsPrecedent;
        tempsPrecedent = tempsActuel;
        if (tempsPasse < limiteMilliSec)
            SDL_Delay( limiteMilliSec - tempsPasse );
    }


    SDL_FreeSurface(rouge.surface);
    SDL_FreeSurface(bleu.surface);

    TTF_CloseFont(police);
    TTF_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}




void affichageFPS(int dureeExec, TTF_Font* police)
{
    SDL_Surface *surface = NULL;
    SDL_Color couleurNoire = {0, 0, 0};
    SDL_Rect position = {0, 0};
    char chaine[11] = "";

    sprintf(chaine, "%d", dureeExec); //millisecondes
    //sprintf(chaine, "%f", (1000*(1.0/dureeExec))); //FPS
    surface = TTF_RenderText_Blended(police, chaine, couleurNoire);
    SDL_BlitSurface(surface, NULL, SDL_GetVideoSurface(), &position);
    SDL_FreeSurface(surface);
}





