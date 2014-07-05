#include "map.h"


MATRICE initCases (int largeur, int hauteur)
{
    MATRICE c;
    int i;

    c=(TYPE**)malloc(hauteur*sizeof(TYPE*));

    for(i=0 ; i<hauteur ; i++)
    {
        c[i]=(TYPE*)malloc(largeur*sizeof(TYPE));
    }

    return c;
}

void initMap (MAP *map)
{
    map->mapInitialisee=1;

    if( map->largeur_map*map->hauteur_map < PETITE_MAP)
    {
        map->taille=PETITE;
    }
    if( map->largeur_map*map->hauteur_map <= GRANDE_MAP && map->largeur_map*map->hauteur_map >= PETITE_MAP )
    {
        map->taille=MOYENNE;
    }
    if( map->largeur_map*map->hauteur_map > GRANDE_MAP)
    {
        map->taille=GRANDE;
    }

    map->cases=initCases(map->largeur_map,map->hauteur_map);

    map->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, LONGUEUR_CASE*map->largeur_map, LONGUEUR_CASE*map->hauteur_map, 32, 0, 0, 0, 0);

    SDL_FillRect(map->surface,NULL,SDL_MapRGB(map->surface->format, 0, 0, 0));

    map->nb_cases_terre = 0;
    map->nb_cases_eau = 0;

    map->eau = SDL_LoadBMP("media/eau.bmp");
    map->terre = SDL_LoadBMP("media/terre.bmp");
}


int genererMap (MAP *map, Simulation *sim)
{
    CHARGEMENT *charg;

    //charg=debutChargementEtape1(sim);

    charg=initChargement();

    actualiserChargement(sim,charg,"chargement","Creation de la map ...",10,1);

    while (map->nb_cases_eau < (map->largeur_map*map->hauteur_map*0.03) || map->nb_cases_eau > (map->largeur_map*map->hauteur_map*0.45))//on veut une map avec 3% à 45% d'eau
    {
        genererMapAletatoire(map);
        SDL_Delay(500);

        lissageMap(sim,map,charg);
        SDL_Delay(100);
    }

    collerMapTexture(map,1);
    actualiserChargement(sim,charg,"chargement","Affichage de la map et des individus ...",100,1);
    supprimerChargement(charg);
    SDL_Delay(500);

    return 1;
}


void genererMapAletatoire (MAP *map)
{
    int i=0,j=0;

    map->nb_cases_terre = 0;
    map->nb_cases_eau = 0;

    for(i=0; i < map->hauteur_map ; i++)
    {
        for(j=0 ; j < map->largeur_map ; j++)
        {
           if( (Random(0,100)) > 28)
           {
                map->cases[i][j]=EAU;
                map->nb_cases_eau++;
           }
           else
           {
               map->cases[i][j]= TERRE;
               map->nb_cases_terre++;
           }
        }
    }
}



void lissageMap (Simulation *sim, MAP *map, CHARGEMENT *charg)
{
    int lissage, i, j;
    int coefLissage;

    coefLissage=CalculCoefLissage(map);

    actualiserChargement(sim,charg,"chargement","Repartition de l'eau sur la map ...",10,1);
    SDL_Delay(500);

    for(lissage=0; lissage < coefLissage ; lissage ++)
    {
        for(i=0; i < map->hauteur_map ; i++)
        {
            for(j=0 ; j < map->largeur_map ; j++)
            {
                if(map->cases[i][j] == EAU)
                {
                    if(i == 0 || j == 0 || i == (map->hauteur_map - 1) || j == (map->largeur_map - 1))
                    {
                        if(i == 0 && j == 0)
                        {
                            if((map->cases[i+1][j] == EAU && map->cases[i][j+1] == EAU))
                            {
                                //rien
                            }
                            else
                            {
                                map->cases[i][j]= TERRE;
                                map->nb_cases_terre++;
                                map->nb_cases_eau--;
                            }
                        }
                        if(i == (map->hauteur_map-1) && j == 0)
                        {
                            if((map->cases[i-1][j] == EAU && map->cases[i][j+1] == EAU))
                            {
                                //rien
                            }
                            else
                            {
                                map->cases[i][j]= TERRE;
                                map->nb_cases_terre++;
                                map->nb_cases_eau--;
                            }
                        }
                        if(i == (map->hauteur_map - 1) && j == (map->largeur_map - 1))
                        {
                            if((map->cases[i-1][j] == EAU && map->cases[i][j-1] == EAU))
                            {
                                //rien
                            }
                            else
                            {
                                map->cases[i][j]= TERRE;
                                map->nb_cases_terre++;
                                map->nb_cases_eau--;
                            }
                        }
                        if(i == 0 && j == (map->largeur_map - 1))
                        {
                            if((map->cases[i+1][j] == EAU && map->cases[i][j-1] == EAU))
                            {
                                //rien
                            }
                            else
                            {
                                map->cases[i][j]= TERRE;
                                map->nb_cases_terre++;
                                map->nb_cases_eau--;
                            }
                        }
                        if(i == 0 && j != 0 && i != (map->hauteur_map - 1) && j != (map->largeur_map - 1))
                        {
                            if((map->cases[i+1][j] == EAU && map->cases[i][j+1]) || (map->cases[map->hauteur_map-1][j] == EAU && map->cases[i][j-1] == EAU))
                            {
                                //rien
                            }
                            else
                            {
                                map->cases[i][j]= TERRE;
                                map->nb_cases_terre++;
                                map->nb_cases_eau--;
                            }
                        }
                        if(i != 0 && j == 0 && i != (map->hauteur_map - 1) && j != (map->largeur_map - 1))
                        {
                            if((map->cases[i+1][j] == EAU && map->cases[i][j+1]) || (map->cases[i-1][j] == EAU && map->cases[i][map->hauteur_map-1] == EAU))
                            {
                                //rien
                            }
                            else
                            {
                                map->cases[i][j]= TERRE;
                                map->nb_cases_terre++;
                                map->nb_cases_eau--;
                            }
                        }
                        if(i != 0 && j != 0 && i == (map->hauteur_map - 1) && j != (map->largeur_map - 1))
                        {
                            if((map->cases[i-1][j] == EAU && map->cases[i][j-1] == EAU) || (map->cases[i - map->hauteur_map + 1][j] == EAU && map->cases[i][j+1] == EAU))
                            {
                                //rien
                            }
                            else
                            {
                                map->cases[i][j]= TERRE;
                                map->nb_cases_terre++;
                                map->nb_cases_eau--;
                            }
                        }
                        if(i != 0 && j != 0 && i != (map->hauteur_map - 1) && j == (map->largeur_map - 1))
                        {
                            if((map->cases[i-1][j] == EAU && map->cases[i][j-1] == EAU) || (map->cases[i+1][j] == EAU && map->cases[i][j - map->largeur_map + 1] == EAU))
                            {
                                //rien
                            }
                            else
                            {
                                map->cases[i][j]= TERRE;
                                map->nb_cases_terre++;
                                map->nb_cases_eau--;
                            }
                        }
                    }
                    else
                    {
                        if( (map->cases[i-1][j] == EAU && map->cases[i][j-1] == EAU) || (map->cases[i+1][j] == EAU && map->cases[i][j+1] == EAU))
                        {
                            //rien
                        }
                        else
                        {
                            map->cases[i][j]= TERRE;
                            map->nb_cases_terre++;
                            map->nb_cases_eau--;
                        }
                    }
                }
                else
                {
                    if(i != 0 && j != 0 && i != (map->hauteur_map - 1) && j != (map->largeur_map - 1))
                    {
                        if((map->cases[i-1][j] == EAU && map->cases[i][j-1] == EAU) && (map->cases[i+1][j] == EAU && map->cases[i][j+1] == EAU))
                        {
                            map->cases[i][j]=EAU;
                            map->nb_cases_terre--;
                            map->nb_cases_eau++;
                        }
                    }
                }
            }
        }

        actualiserChargement(sim,charg,NULL,NULL,(80/coefLissage)*lissage + 10,0);
    }
}


void collerMapTexture (MAP *map, int zoom)
{
    int i,j;
    SDL_Rect position;

    for(i=0; i < map->hauteur_map ; i++)
    {
        position.y =i*LONGUEUR_CASE*zoom;

        for(j=0 ; j < map->largeur_map ; j++)
        {
           position.x = j*LONGUEUR_CASE*zoom;

           if(map->cases[i][j] == TERRE)
           {
               SDL_BlitSurface(map->terre, NULL, map->surface, &position);
           }
           if(map->cases[i][j] == EAU)
           {
               SDL_BlitSurface(map->eau, NULL, map->surface, &position);
           }
        }
    }

    SDL_FreeSurface(map->eau);
    SDL_FreeSurface(map->terre);
}



int CalculCoefLissage (MAP *map)
{
    if(map->taille == PETITE)
    {
        return ((map->hauteur_map*map->largeur_map) / ((map->hauteur_map+map->largeur_map)*2));
    }
    if(map->taille == MOYENNE)
    {
        return ((map->hauteur_map*map->largeur_map) / ((map->hauteur_map+map->largeur_map)*3));
    }
    if(map->taille == GRANDE)
    {
        return ((map->hauteur_map*map->largeur_map) / ((map->hauteur_map+map->largeur_map)*7));
    }

    return 0;
}


void mapZoom (MAP *map, int zoom)
{

    SDL_Surface *terre;
    SDL_Surface *eau;

    map->eau = SDL_LoadBMP("media/eau.bmp");
    map->terre = SDL_LoadBMP("media/terre.bmp");

    terre = SDL_CreateRGBSurface(SDL_HWSURFACE, LONGUEUR_CASE*zoom, LONGUEUR_CASE*zoom, 32, 0, 0, 0, 0);
    eau = SDL_CreateRGBSurface(SDL_HWSURFACE, LONGUEUR_CASE*zoom, LONGUEUR_CASE*zoom, 32, 0, 0, 0, 0);

    terre = rotozoomSurface(map->terre,0,zoom,SMOOTHING_ON);
    eau = rotozoomSurface(map->eau,0,zoom,SMOOTHING_ON);

    SDL_FreeSurface(map->surface);
    SDL_FreeSurface(map->eau);
    SDL_FreeSurface(map->terre);

    map->terre = terre;
    map->eau = eau;
    map->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, zoom*LONGUEUR_CASE*map->largeur_map, zoom*LONGUEUR_CASE*map->hauteur_map, 32, 0, 0, 0, 0);

    collerMapTexture(map,zoom);
}



TYPE interactionIndividuMap (Simulation *sim, Individu *individu)
{
    int x=0, y=0;

    x=individu->position.x/LONGUEUR_CASE;
    y=individu->position.y/LONGUEUR_CASE;

    if(x >= sim->map.largeur_map || y >= sim->map.hauteur_map)
    {
        if(x >= sim->map.largeur_map)
        {
            x=sim->map.largeur_map-1;
        }
        if(y >= sim->map.hauteur_map)
        {
            y=sim->map.hauteur_map-1;
        }
    }
    if(x < 0 || y < 0)
    {
        if(x < 0)
        {
            x=0;
        }
        if(y < 0)
        {
            y=0;
        }
    }


    return sim->map.cases[y][x];
}


CHARGEMENT* initChargement ()
{
    CHARGEMENT *chargement;

    chargement=(CHARGEMENT*)malloc(sizeof(CHARGEMENT));
    chargement->police = TTF_OpenFont("fonts/game_over.ttf", 100);

    chargement->texteTitre = NULL;
    chargement->texteSecondaire = NULL;
    chargement->barre_chargement = NULL;
    chargement->etat_chargement = NULL;

    return chargement;
}


void actualiserChargement (Simulation *sim, CHARGEMENT *chargement, char *texteTitre, char *texteSecondaire, int pourcentage, bool actualiserTexte)
{
    if(actualiserTexte == 1)
    {
        //on efface l'ecran
        SDL_FillRect(sim->ecran, NULL, SDL_MapRGB((sim->ecran)->format, 0, 0, 0));

        //on libère les surfaces textes
        SDL_FreeSurface(chargement->texteTitre);
        SDL_FreeSurface(chargement->texteSecondaire);

        //on crée les nouvelles surfaces textes
        chargement->texteTitre = TTF_RenderText_Blended(chargement->police, texteTitre, sim->couleurBlanche);

        chargement->texteSecondaire = TTF_RenderText_Blended(sim->police, texteSecondaire, sim->couleurBlanche);
    }

    chargement->barre_chargement = SDL_CreateRGBSurface(SDL_HWSURFACE, 404, 44, 32, 0, 0, 0, 0);
    SDL_FillRect(chargement->barre_chargement, NULL, SDL_MapRGB( chargement->barre_chargement -> format, 255, 255, 255));

    chargement->etat_chargement = SDL_CreateRGBSurface(SDL_HWSURFACE, (400 - pourcentage*4), 40, 32, 0, 0, 0, 0);
    SDL_FillRect(chargement->etat_chargement, NULL, SDL_MapRGB( chargement->etat_chargement -> format, 0, 0, 0));

    affichageChargement(sim,chargement,actualiserTexte);
}


void supprimerChargement (CHARGEMENT* charg)
{
    TTF_CloseFont(charg->police);

    SDL_FreeSurface(charg->texteSecondaire);
    SDL_FreeSurface(charg->texteTitre);

    free(charg);
}


void affichageChargement (Simulation *sim, CHARGEMENT *struct_charg, bool actualiserTexte)
{
    SDL_Rect position;

    //On blit toutes les surfaces sauf les surfaces textes si actualiserTexte est faux

    if(actualiserTexte == 1)
    {
        position.x=(sim->largeur_ecran - struct_charg->texteTitre->w)/2;
        position.y=sim->hauteur_ecran/4;
        SDL_BlitSurface(struct_charg->texteTitre, NULL, sim->ecran, &position);

        position.x=(sim->largeur_ecran - struct_charg->texteSecondaire->w)/2;
        position.y=position.y + struct_charg->texteTitre->h + sim->hauteur_ecran/6;
        SDL_BlitSurface(struct_charg->texteSecondaire, NULL, sim->ecran, &position);
    }
    else
    {
        position.y=sim->hauteur_ecran/4;
        position.y=position.y + struct_charg->texteTitre->h + sim->hauteur_ecran/6;
    }


    position.x=(sim->largeur_ecran - struct_charg->barre_chargement->w)/2;
    position.y=position.y + struct_charg->texteSecondaire->h + sim->hauteur_ecran/6;
    SDL_BlitSurface(struct_charg->barre_chargement, NULL, sim->ecran, &position);

    position.x += 2 + (struct_charg->barre_chargement->w - struct_charg->etat_chargement->w -4);
    position.y += 2;
    SDL_BlitSurface(struct_charg->etat_chargement, NULL, sim->ecran, &position);

    //on libère la mémoire alloué au surface pour la barre
    SDL_FreeSurface(struct_charg->barre_chargement);
    SDL_FreeSurface(struct_charg->etat_chargement);

    SDL_Flip(sim->ecran);
}



int sauvegardeMap (MAP map, FILE *fichierMap)
{
    int i,j;

    if(fichierMap != NULL)
    {
        fprintf(fichierMap, "%d\n%d\n",map.largeur_map,map.hauteur_map);

        for(i=0;i<map.hauteur_map;i++)
        {
            for(j=0;j<map.largeur_map;j++)
            {
                fprintf(fichierMap, "%d\n", map.cases[i][j]);
            }
        }

        return 1;
    }
    else
    {
        return 0;
    }
}




int chargerMap (Simulation* sim, MAP *map, FILE* fichierMap, CHARGEMENT* charg)
{
    int i,j,taille=0;
    SDL_Rect position;
    char** tab;
    char chaine[TAILLE_MAX];
    int pourcentage = 10;

    if(fichierMap != NULL)
    {
        tab=initTableauChaineCaractrere();

        while( fgets(chaine, TAILLE_MAX, fichierMap) != NULL )
        {
            if( taille%50 == 0 || taille==0)
            {
                tab=augmenterTailleTabChaineCaractere(taille+1,tab,50);
            }

            strcpy(tab[taille],chaine);
            taille++;

            if( (taille % 375) == 0)
            {
                pourcentage +=1;
                actualiserChargement(sim,charg,NULL,NULL,pourcentage,0);
            }
        }

        map->largeur_map=atof(tab[0]);
        map->hauteur_map=atof(tab[1]);

        initMap(map);

        for(i=0;i<map->hauteur_map;i++)
        {
            position.y =i*LONGUEUR_CASE;

            for(j=0;j<map->largeur_map;j++)
            {
                position.x =j*LONGUEUR_CASE;

                map->cases[i][j]=atof(tab[i*map->largeur_map + j + 2]);
                if((map->cases[i][j]) == TERRE)
                {
                    map->nb_cases_terre++;
                    SDL_BlitSurface(map->terre, NULL, map->surface, &position);
                }
                if((map->cases[i][j]) == EAU)
                {
                    map->nb_cases_eau++;
                    SDL_BlitSurface(map->eau, NULL, map->surface, &position);
                }
            }
        }

        supprimerTabChaineCaractere(tab,taille,50);

        SDL_FreeSurface(map->eau);
        SDL_FreeSurface(map->terre);

        return 1;
    }
    else
    {
        return 0;
    }
}

void deleteMap (MAP *map)
{
    map->mapInitialisee=0;

    delCases(map->cases,map->largeur_map,map->hauteur_map);
    SDL_FreeSurface(map->surface);

}

void delCases (MATRICE cases, int largeur, int hauteur)
{
    int i;

    for(i=0;i<hauteur;i++)
    {
        free(cases[i]);
    }

    free(cases);
}
