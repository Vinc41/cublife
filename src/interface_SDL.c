#include "interface_SDL.h"




int initSDL(Simulation* sim, bool init_juste_SDL)
{
    int mapCharger=0;
    char nom[TAILLE_MAX+12];

    SDL_Color blanc = {255,255,255};
    printf("la sdl va s'initialiser...");

    sim->largeur_ecran = 800; //50 cases
    sim->hauteur_ecran = 608; //38 cases

    //initialisation de la SDL et de la SDL_TTF
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    TTF_Init();

    //chargement de l'icone et création du nom de la fênetre
    SDL_WM_SetIcon(IMG_Load("media/iconeSDL.png"),NULL);
    if(strlen(sim->nomSimulation) != 0)
    {
        sprintf(nom,"Cubelife - %s",sim->nomSimulation);
    }
    else
    {
        strcpy(nom,"Cubelife");
    }
    SDL_WM_SetCaption(nom,NULL);

    //chargement de la police et initialisation de la couleur
    sim->police = TTF_OpenFont("fonts/calibri.ttf", TAILLE_POLICE_INDIVIDUS);
    sim->police_temps = TTF_OpenFont("fonts/game_over.ttf", TAILLE_POLICE_TEMPS);
    sim->couleurBlanche = blanc;

    //creation de l'écran
    sim->ecran = SDL_SetVideoMode(sim->largeur_ecran, sim->hauteur_ecran, 32, SDL_HWSURFACE|SDL_RESIZABLE|SDL_DOUBLEBUF);

    if(sim->ecran == NULL)
    {
        fprintf(stderr, "Impossible de charger le mode vidéo : %s\n",SDL_GetError());
        exit(EXIT_FAILURE);
    }

    //Affichage d'un ecran noir
    SDL_FillRect(sim->ecran, NULL, SDL_MapRGB((sim->ecran)->format, 0, 0, 0));

    //initialisation de la varaible zoom
    sim->zoom = 1;

    //initialisation de la vitesse
    sim->coeffTemps = 1;

    //initialisation de la variable contenant l'adresse de l'individu a afficher les caractéristiques
    sim->indStatistiques = NULL;

    if(init_juste_SDL == 0)
    {
        //generation de la map ou chargement de la map
        if(sim->genererNouvelleMap == 1)
        {
            initMap(&(sim->map));

            printf("OK\n");

            printf("generation de la map...");

            mapCharger=genererMap(&(sim->map),sim);

            printf("OK\n");

            //initialisation du temps et de la vitesse
            sim->tempsPasse = 0;
            sim->annee=0;
            sim->mois=0;

            sim->genererNouvelleMap=0;
        }
        else
        {
            printf("Chargement de la map...");

            printf("OK\n");
        }


        //Ajout des individus dan sla sdl en generant leur position ou pas
        if(sim->genererPosEspeces == 1)
        {
            sim->genererPosEspeces=0;

            //creation des individus
            initIndSDL(sim,1);

        }
        else
        {
            //creation des individus
            initIndSDL(sim,0);
        }

        //Initialisation des coordonées qui serviront ensuite à déplacer la map
        if( ((sim->map.surface->h) <= sim->hauteur_ecran) && ((sim->map.surface->w) <= sim->largeur_ecran) ) //la map est moins large et moins haute que l'écran
        {
            sim->pos_decoupage.x =0;
            sim->pos_decoupage.y =0;
        }
        if((sim->map.hauteur_map*LONGUEUR_CASE) <= sim->hauteur_ecran && (sim->map.largeur_map*LONGUEUR_CASE) > sim->largeur_ecran) //la map est plus large que l'écran
        {
           sim->pos_decoupage.x=((sim->map.largeur_map*LONGUEUR_CASE) - sim->largeur_ecran) / 2;
        }
        if((sim->map.largeur_map*LONGUEUR_CASE) <= sim->largeur_ecran && (sim->map.hauteur_map*LONGUEUR_CASE) > sim->hauteur_ecran) //la map est plus haute que l'écran
        {
           sim->pos_decoupage.y=((sim->map.hauteur_map*LONGUEUR_CASE) - sim->hauteur_ecran) / 2;
        }
        if( ((sim->map.hauteur_map*LONGUEUR_CASE) > sim->hauteur_ecran) && ((sim->map.largeur_map*LONGUEUR_CASE) > sim->largeur_ecran) ) //sinon la map est plus large et plus haute que l'écran
        {
           sim->pos_decoupage.x=((sim->map.largeur_map*LONGUEUR_CASE) - sim->largeur_ecran) / 2;
           sim->pos_decoupage.y=((sim->map.hauteur_map*LONGUEUR_CASE) - sim->hauteur_ecran) / 2;
        }
    }

    return mapCharger;
}


void initIndSDL(Simulation* sim, bool firstInit)
{
    int id;
    Espece *espece = sim->premiereEspece;
    Individu *ind;

    while(espece != NULL)
    {
        ind = espece->premierIndividu;
        id=0;

        espece->male = NULL;
        espece->femelle = NULL;

        creationSurfaceIndividu(sim,espece,sim->zoom);

        while(ind != NULL)
        {
            id++;
            ind->id=id;

            ajoutSDLIndividu(sim,ind,espece->nom,firstInit);

            ind = ind->suivant;
        }
        espece=espece->suivante;
    }
}


void creationSurfaceIndividu (Simulation* sim, Espece *espece, int zoom)
{
    //modification de la couleur noir car elle sera rendu transparente plus tard
    if((espece->couleur.red == 0) && (espece->couleur.green == 0) && (espece->couleur.blue == 0))
    {
        espece->couleur.red = 0.01;
        espece->couleur.green = 0.01;
        espece->couleur.blue = 0.01;
    }

    //vérification que les surfaces ne sont pas deja allouées
    if(espece->male != NULL)
    {
        SDL_FreeSurface(espece->male);
    }
    if(espece->femelle != NULL)
    {
        SDL_FreeSurface(espece->femelle);
    }

    //Creation de la surface pour les individu male (forme = carré)
    espece->male = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR_ANIMAUX*zoom, HAUTEUR_ANIMAUX*zoom, 32, 0, 0, 0, 0);
    SDL_FillRect(espece->male, NULL, SDL_MapRGB( (sim->map.surface) -> format, espece->couleur.red*255, espece->couleur.green*255, espece->couleur.blue*255));

    //Creation de la surface pour les individu femelle (forme = femelle)
    espece->femelle = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR_ANIMAUX*zoom, HAUTEUR_ANIMAUX*zoom, 32, 0, 0, 0, 0);
    filledCircleRGBA(espece->femelle, (LARGEUR_ANIMAUX/2)*zoom, (HAUTEUR_ANIMAUX/2)*zoom, RAYON_ANIMAUX*zoom, espece->couleur.red*255, espece->couleur.green*255, espece->couleur.blue*255, 255);
    SDL_SetColorKey(espece->femelle, SDL_SRCCOLORKEY, SDL_MapRGB(espece->femelle->format, 0, 0, 0));
}



void ajoutSDLIndividu(Simulation *sim, Individu *individu, char *nomEspece, bool genererPosAleatoire)
{
    char nomIndividu[40];

    //creation du nom de l'individu
    sprintf(nomIndividu, "%s %d", nomEspece, individu->id);
    individu->nom = TTF_RenderText_Blended(sim->police, nomIndividu, sim->couleurBlanche);

    //creation des coordonnées d'apparition de l'individu sur la map si c'est une nouvelle simulation ou que ce n'est pas une naissance

    if(genererPosAleatoire == 1)
    {
        do
        {
            individu->position.x = Random(0,LONGUEUR_CASE*sim->map.largeur_map - LARGEUR_ANIMAUX);
            individu->position.y = Random(0,LONGUEUR_CASE*sim->map.hauteur_map - HAUTEUR_ANIMAUX);
        }while(interactionIndividuMap(sim,individu)==EAU);

    }
}


void actualiserNomIndividu (Simulation* sim, Individu *individu, char *nomEspece)
{
    char nomIndividu[40];

    //libération des surfaces avantde d'en creer de nouvelle
    SDL_FreeSurface(individu->nom);

    //creation du nom de l'individu
    sprintf(nomIndividu, "%s %d", nomEspece, individu->id);
    individu->nom = TTF_RenderText_Blended(sim->police, nomIndividu, sim->couleurBlanche);
}


char* formaterTemps (Simulation *sim, float tempsSimulation)
{
    int s=0, min=0, h=0, j_calcule =0, j_affiche =0;
    char *temps;
    int tab[13] = {0,janvier,fevrier,mars,avril,mai,juin,juillet,aout,septembre,octobre,novembre,decembre};

    temps=(char*)malloc(TAILLE_MAX*sizeof(char));

    tempsSimulation = tempsSimulation * 60;

    if(tempsSimulation <2)
    {
        sim->mois=0;
        sim->annee=0;
    }

    j_calcule = tempsSimulation/86400;

    s = tempsSimulation - j_calcule*86400;

    h = s /3600;
    s = s - h*3600;

    min = s / 60;
    s = s - min*60;

    if(j_calcule == janvier +(365*sim->annee))
        sim->mois=1;
    if(j_calcule == fevrier +(365*sim->annee))
        sim->mois=2;
    if(j_calcule == mars +(365*sim->annee))
        sim->mois=3;
    if(j_calcule == avril +(365*sim->annee))
        sim->mois=4;
    if(j_calcule == mai +(365*sim->annee))
        sim->mois=5;
    if(j_calcule == juin +(365*sim->annee))
        sim->mois=6;
    if(j_calcule == juillet +(365*sim->annee))
        sim->mois=7;
    if(j_calcule == aout +(365*sim->annee))
        sim->mois=8;
    if(j_calcule == septembre +(365*sim->annee))
        sim->mois=9;
    if(j_calcule == octobre +(365*sim->annee))
        sim->mois=10;
    if(j_calcule == novembre +(365*sim->annee))
        sim->mois=11;
    if(j_calcule >= decembre  +(365*sim->annee))
    {
        sim->annee++;
        sim->mois=0;
    }
    j_affiche = j_calcule - (tab[sim->mois] + 365*(sim->annee));

    sprintf(temps, "temps : %dans %dmois %djours %dh %dmin %ds",sim->annee,sim->mois,j_affiche,h,min,s);

    return temps;
}



void afficheTemps (Simulation* sim)
{
    char *temps;
    SDL_Rect position;

    temps=formaterTemps(sim,sim->tempsPasse);

    sim->surface_temps = TTF_RenderText_Blended(sim->police_temps, temps, sim->couleurBlanche);

    position.x = sim->largeur_ecran - (sim->surface_temps->w+10);
    position.y = 5;

    SDL_BlitSurface(sim->surface_temps, NULL, sim->ecran, &position);

    free(temps);

    SDL_FreeSurface(sim->surface_temps);
}


void actualiserEcran(Simulation* sim)
{
    if( ((sim->map.surface->h) <= sim->hauteur_ecran) && ((sim->map.surface->w) <= sim->largeur_ecran) ) //la map est moins large et moins haute que l'écran
    {
        sim->pos_collage.x = (sim->largeur_ecran - (sim->map.surface->w)) /2;
        sim->pos_collage.y = (sim->hauteur_ecran -(sim->map.surface->h)) /2;

        SDL_BlitSurface(sim->map.surface, NULL, sim->ecran, &sim->pos_collage);
    }
    if( ((sim->map.surface->h) <= sim->hauteur_ecran) && ((sim->map.surface->w) > sim->largeur_ecran) ) //la map est plus large que l'écran
    {
        sim->pos_collage.x = 0;
        sim->pos_collage.y = (sim->hauteur_ecran -(sim->map.surface->h)) /2;
        sim->pos_decoupage.y=0;
        sim->pos_decoupage.w=sim->map.surface->w;
        sim->pos_decoupage.h=sim->hauteur_ecran;

        SDL_BlitSurface(sim->map.surface, &sim->pos_decoupage, sim->ecran, &sim->pos_collage);
    }
    if( ((sim->map.surface->w) <= sim->largeur_ecran) && ((sim->map.surface->h) > sim->hauteur_ecran) ) //la map est plus haute que l'écran
    {
        sim->pos_collage.x = (sim->largeur_ecran - (sim->map.surface->w)) /2;
        sim->pos_collage.y = 0;
        sim->pos_decoupage.x=0;
        sim->pos_decoupage.w=sim->largeur_ecran;
        sim->pos_decoupage.h=sim->map.surface->h;

        SDL_BlitSurface(sim->map.surface, &sim->pos_decoupage, sim->ecran, &sim->pos_collage);
    }
    if( ((sim->map.surface->h) > sim->hauteur_ecran) && ((sim->map.surface->w) > sim->largeur_ecran) ) //sinon la map est plus large et plus haute que l'écran
    {
        sim->pos_collage.x = 0;
        sim->pos_collage.y = 0;
        sim->pos_decoupage.w=sim->largeur_ecran;
        sim->pos_decoupage.h=sim->hauteur_ecran;

        SDL_BlitSurface(sim->map.surface, &sim->pos_decoupage, sim->ecran, &sim->pos_collage);
    }
}




void afficheIndividus (Simulation *sim, int zoom, Individu* indSelectionne)
{
 Espece *espece = sim->premiereEspece;
    Individu *ind;
    SDL_Rect pos;

    //on colle tous les éléments en considérant leur déplacements si les animaux sont sur l'écran
    while(espece != NULL)
    {
        ind = espece->premierIndividu;

        while(ind != NULL)
        {
            if( ((sim->map.surface->h) <= sim->hauteur_ecran) && ((sim->map.surface->w) <= sim->largeur_ecran) ) //la map est moins large et moins haute que l'écran
            {
                //On prend en compte lefait que la map est centré dans l'écran
                pos.x = ((sim->largeur_ecran - sim->map.surface->w)/2) + ind->position.x*zoom;
                pos.y = ((sim->hauteur_ecran - sim->map.surface->h)/2) + ind->position.y*zoom;

                //On blite l'individu avec une image differente suivant son sexe
                if(ind->sexe == MALE)
                {
                    SDL_BlitSurface(ind->espece->male, NULL, sim->ecran, &(pos));
                }
                if(ind->sexe == FEMELLE || ind->sexe == BOTH)
                {
                    SDL_BlitSurface(ind->espece->femelle, NULL, sim->ecran, &(pos));
                }

                //on affiche les caractéristiques de l'individu si un individu est selectionné
                if(ind == indSelectionne)
                {
                    afficheStatistiquesInd(sim,&pos);
                }

                //On colle le nom au dessus de l'individu
                pos.x = pos.x - (ind->nom->w - LARGEUR_ANIMAUX*zoom)/2;
                pos.y -= 16;

                SDL_BlitSurface(ind->nom, NULL, sim->ecran, &(pos));
            }
            if( ((sim->map.surface->h) <= sim->hauteur_ecran) && ((sim->map.surface->w) > sim->largeur_ecran) ) //la map est plus large que l'écran
            {
                //on met les coordonnées de l'individu dans le repère écran
                pos.x = ind->position.x*zoom - sim->pos_decoupage.x;

                if( (ind->position.x*zoom +MARGE_AFFICHAGE_INDIVIDU >= sim->pos_decoupage.x) && (ind->position.x*zoom - MARGE_AFFICHAGE_INDIVIDU <= sim->pos_decoupage.x+sim->largeur_ecran) ) //individu présent à l'écran
                {
                    //On prend en compte lefait que la map est centré dans l'écran
                    pos.y = ((sim->hauteur_ecran - sim->map.surface->h)/2) + ind->position.y*zoom;

                    //On blite l'individu avec une image differente suivant son sexe
                    if(ind->sexe == MALE)
                    {
                        SDL_BlitSurface(ind->espece->male, NULL, sim->ecran, &(pos));
                    }
                    if(ind->sexe == FEMELLE || ind->sexe == BOTH)
                    {
                        SDL_BlitSurface(ind->espece->femelle, NULL, sim->ecran, &(pos));
                    }

                    //on affiche les caractéristiques de l'individu si un individu est selectionné
                    if(ind == indSelectionne)
                    {
                        afficheStatistiquesInd(sim,&pos);
                    }

                    //On colle le nom au dessus de l'individu
                    pos.x = pos.x - (ind->nom->w - LARGEUR_ANIMAUX*zoom)/2;
                    pos.y -= 16;

                    SDL_BlitSurface(ind->nom, NULL, sim->ecran, &(pos));
                }
            }
            if( ((sim->map.surface->w) <= sim->largeur_ecran) && ((sim->map.surface->h) > sim->hauteur_ecran) ) //la map est plus haute que l'écran
            {
                //on met les coordonnées de l'individu dans le repère écran
                pos.y = ind->position.y*zoom - sim->pos_decoupage.y;

                if( (ind->position.y*zoom + MARGE_AFFICHAGE_INDIVIDU >= sim->pos_decoupage.y) && (ind->position.y*zoom - MARGE_AFFICHAGE_INDIVIDU <= sim->pos_decoupage.y+sim->hauteur_ecran) ) //individu présent à l'écran
                {
                    //On prend en compte le fait que la map est centré dans l'écran
                    pos.x = ((sim->largeur_ecran - sim->map.surface->w)/2) + ind->position.x*zoom;

                    //On blite l'individu avec une image differente suivant son sexe
                    if(ind->sexe == MALE)
                    {
                        SDL_BlitSurface(ind->espece->male, NULL, sim->ecran, &(pos));
                    }
                    if(ind->sexe == FEMELLE || ind->sexe == BOTH)
                    {
                        SDL_BlitSurface(ind->espece->femelle, NULL, sim->ecran, &(pos));
                    }

                    //on affiche les caractéristiques de l'individu si un individu est selectionné
                    if(ind == indSelectionne)
                    {
                        afficheStatistiquesInd(sim,&pos);
                    }

                    //On colle le nom au dessus de l'individu
                    pos.x = pos.x - (ind->nom->w - LARGEUR_ANIMAUX*zoom)/2;
                    pos.y -= 16;

                    SDL_BlitSurface(ind->nom, NULL, sim->ecran, &(pos));
                }
            }
            if( ((sim->map.surface->h) > sim->hauteur_ecran) && ((sim->map.surface->w) > sim->largeur_ecran) ) //sinon la map est plus large et plus haute que l'écran
            {
                //on met les coordonnées de l'individu dans le repère écran
                pos.x = ind->position.x*zoom - sim->pos_decoupage.x;
                pos.y = ind->position.y*zoom - sim->pos_decoupage.y;

                if( (ind->position.x*zoom + MARGE_AFFICHAGE_INDIVIDU >= sim->pos_decoupage.x) && (ind->position.x*zoom - MARGE_AFFICHAGE_INDIVIDU <= sim->pos_decoupage.x+sim->largeur_ecran) && (ind->position.y*zoom + MARGE_AFFICHAGE_INDIVIDU >= sim->pos_decoupage.y) && (ind->position.y*zoom - MARGE_AFFICHAGE_INDIVIDU <= sim->pos_decoupage.y+sim->hauteur_ecran))
                {
                    //individu présent à l'écran

                    //On blite l'individu avec une image differente suivant son sexe
                    if(ind->sexe == MALE)
                    {
                        SDL_BlitSurface(ind->espece->male, NULL, sim->ecran, &(pos));
                    }
                    if(ind->sexe == FEMELLE || ind->sexe == BOTH)
                    {
                        SDL_BlitSurface(ind->espece->femelle, NULL, sim->ecran, &(pos));
                    }

                    //on affiche les caractéristiques de l'individu si un individu est selectionné
                    if(ind == indSelectionne)
                    {
                        afficheStatistiquesInd(sim,&pos);
                    }

                    //On colle le nom au dessus de l'individu
                    pos.x = pos.x - (ind->nom->w - LARGEUR_ANIMAUX*zoom)/2;
                    pos.y -= 16;

                    SDL_BlitSurface(ind->nom, NULL, sim->ecran, &(pos));
                }
            }

            //On passe à l'individu suivant
            ind = ind->suivant;
        }
        espece=espece->suivante;
    }
}



void afficheStatistiquesInd (Simulation *sim, SDL_Rect* position)
{
    SDL_Surface *statistiques;
    SDL_Surface *sante, *fatigue, *stockNourriture, *stockEau;
    SDL_Rect pos;


    statistiques=SDL_CreateRGBSurface(SDL_HWSURFACE, 113, 110, 32, 0, 0, 0, 0);
    SDL_FillRect(statistiques, NULL, SDL_MapRGB((sim->ecran)->format, 0, 0, 0));


    //creation du texte pour afficher la santé de l'individu
    sante = creationSurfaceStat(sim,sim->indStatistiques->sante,"sante",0);

    pos.x = 5;
    pos.y = 0;

    SDL_BlitSurface(sante, NULL, statistiques, &(pos));

    SDL_FreeSurface(sante);

    //creation du texte pour afficher la fatigue de l'individu
    fatigue = creationSurfaceStat(sim,sim->indStatistiques->fatigue,"fatigue",1);

    pos.x = 5;
    pos.y += fatigue->h;

    SDL_BlitSurface(fatigue, NULL, statistiques, &(pos));

    SDL_FreeSurface(fatigue);

    //creation du texte pour afficher la stocknourriture de l'individu
    stockNourriture = creationSurfaceStat(sim,sim->indStatistiques->stockNourriture,"stock nourriture",0);

    pos.x = 5;
    pos.y += stockNourriture->h;

    SDL_BlitSurface(stockNourriture, NULL, statistiques, &(pos));

    SDL_FreeSurface(stockNourriture);

    //creation du texte pour afficher la stockeau de l'individu
    stockEau = creationSurfaceStat(sim,sim->indStatistiques->stockEau,"stock eau",0);

    pos.x = 5;
    pos.y += stockEau->h;

    SDL_BlitSurface(stockEau, NULL, statistiques, &(pos));

    SDL_FreeSurface(stockEau);

    //on colle la surface statistiques et on libère la surface
    position->x = position->x - (statistiques->w - LARGEUR_ANIMAUX*sim->zoom)/2;
    position->y -= statistiques->h;

    SDL_SetAlpha(statistiques, SDL_SRCALPHA, 128);
    SDL_BlitSurface(statistiques, NULL, sim->ecran, position);

    SDL_FreeSurface(statistiques);

    //on remet position x a sa position initial pour les prochain collage
    position->x = position->x + (statistiques->w - LARGEUR_ANIMAUX*sim->zoom)/2;

}


SDL_Surface* creationBarreStat (Simulation *sim, int pourcentage, bool inverserBarre)
{
    SDL_Surface* barreFinal;
    SDL_Surface* barrePourcentageRestant;
    SDL_Surface* barrePourcentagePerdu;
    SDL_Rect pos;



    //allocation des surfaces
    barreFinal = SDL_CreateRGBSurface(SDL_HWSURFACE, 52, 6, 32, 0, 0, 0, 0);
    barrePourcentageRestant = SDL_CreateRGBSurface(SDL_HWSURFACE, (50*pourcentage/100), 4, 32, 0, 0, 0, 0);
    barrePourcentagePerdu = SDL_CreateRGBSurface(SDL_HWSURFACE, (50*(100-pourcentage)/100), 4, 32, 0, 0, 0, 0);

    //coloration des surfaces
    SDL_FillRect(barreFinal, NULL, SDL_MapRGB( (sim->map.surface) -> format, 255, 255, 255));
    SDL_FillRect(barrePourcentagePerdu, NULL, SDL_MapRGB( (sim->map.surface) -> format, 0, 0, 0));

    //inversion du code couleur si c'est demande
    if(inverserBarre == 1)
    {
        if(pourcentage <= 50)
        SDL_FillRect(barrePourcentageRestant, NULL, SDL_MapRGB( (sim->map.surface) -> format, 0, 255, 0));

        if(pourcentage <= 75 && pourcentage > 50)
        SDL_FillRect(barrePourcentageRestant, NULL, SDL_MapRGB( (sim->map.surface) -> format, 255, 255, 0));

        if(pourcentage > 75)
        SDL_FillRect(barrePourcentageRestant, NULL, SDL_MapRGB( (sim->map.surface) -> format, 255, 0, 0));
    }
    else
    {
        if(pourcentage >= 50)
        SDL_FillRect(barrePourcentageRestant, NULL, SDL_MapRGB( (sim->map.surface) -> format, 0, 255, 0));

        if(pourcentage >= 25 && pourcentage < 50)
        SDL_FillRect(barrePourcentageRestant, NULL, SDL_MapRGB( (sim->map.surface) -> format, 255, 255, 0));

        if(pourcentage < 25)
        SDL_FillRect(barrePourcentageRestant, NULL, SDL_MapRGB( (sim->map.surface) -> format, 255, 0, 0));
    }

    //on blit la barreRestant et la barrePerdu sur la barre final
    pos.x = 1;
    pos.y = 1;

    SDL_BlitSurface(barrePourcentageRestant, NULL, barreFinal, &pos);

    pos.x = 1+(50*pourcentage/100);

    SDL_BlitSurface(barrePourcentagePerdu, NULL, barreFinal, &pos);

    //on libère la mémoire
    SDL_FreeSurface(barrePourcentagePerdu);
    SDL_FreeSurface(barrePourcentageRestant);

    //on retourne notre barre final
    return barreFinal;
}


SDL_Surface* creationSurfaceStat(Simulation* sim, int stat, char* nomStat, bool statInverser)
{
    SDL_Surface *surfaceStat;
    SDL_Surface *surfaceTexte;
    SDL_Surface *surfacePourcentage;
    SDL_Surface *barre;
    char texte[TAILLE_MAX];
    char pourcent;
    SDL_Rect pos;

    pourcent='%';

    surfaceStat=SDL_CreateRGBSurface(SDL_HWSURFACE, 103, 26, 32, 0, 0, 0, 0);
    SDL_FillRect(surfaceStat, NULL, SDL_MapRGB((sim->ecran)->format, 0, 0, 0));


    //creation de surfaceTexte
    surfaceTexte = TTF_RenderText_Blended(sim->police, nomStat, sim->couleurBlanche);

    //creation de pourcentage
    sprintf(texte, "%d%c", stat, pourcent);
    surfacePourcentage = TTF_RenderText_Blended(sim->police, texte, sim->couleurBlanche);

    //creation de barre
    barre = creationBarreStat(sim,stat,statInverser);

    //Collage des diiférentes surfaces sur stat
    pos.x = 0;
    pos.y = 0;
    SDL_BlitSurface(surfaceTexte, NULL, surfaceStat, &(pos));

    pos.x = 0;
    pos.y = surfaceTexte->h+2;
    SDL_BlitSurface(barre, NULL, surfaceStat, &(pos));

    pos.x = barre->w + 10;
    pos.y = pos.y - (surfacePourcentage->h - barre->h)/2;
    SDL_BlitSurface(surfacePourcentage, NULL, surfaceStat, &(pos));

    //liberation de la memoire
    SDL_FreeSurface(surfaceTexte);
    SDL_FreeSurface(barre);
    SDL_FreeSurface(surfacePourcentage);

    //on retourne stat
    return surfaceStat;
}



void SDLevent (Simulation* sim)
{
    Uint8 *keystates = SDL_GetKeyState( NULL );
    SDL_EnableKeyRepeat(10,10);


    SDL_PollEvent(&sim->event);

    switch(sim->event.type)
    {
        if( (sim->map.surface->w > sim->largeur_ecran) || (sim->map.surface->h > sim->hauteur_ecran) ) //fonction active si une des dimensions de la map est plus grande que l'écran
        {
            case SDL_KEYDOWN:
                if(sim->map.surface->w > sim->largeur_ecran) //la map est plus large que l'écran
                {
                    if(keystates[SDLK_RIGHT]  && sim->pos_decoupage.x < sim->map.surface->w - sim->largeur_ecran + DISTANCE_DEPACEMENT_MAP) //flèche droite
                        sim->pos_decoupage.x += VITESSE_DEPLACEMENT_MAP;

                    if(keystates[SDLK_LEFT]  && sim->pos_decoupage.x > -DISTANCE_DEPACEMENT_MAP) //flèche de gauche
                        sim->pos_decoupage.x -= VITESSE_DEPLACEMENT_MAP;
                }
                if(sim->map.surface->h > sim->hauteur_ecran) //la map est plus haute que l'écran
                {
                    if(keystates[SDLK_UP] && sim->pos_decoupage.y > -DISTANCE_DEPACEMENT_MAP) //Flèche du haut
                        sim->pos_decoupage.y -= VITESSE_DEPLACEMENT_MAP;

                    if(keystates[SDLK_DOWN]  && sim->pos_decoupage.y < sim->map.surface->h - sim->hauteur_ecran + DISTANCE_DEPACEMENT_MAP) //Flèche du bas
                        sim->pos_decoupage.y += VITESSE_DEPLACEMENT_MAP;
                }
                if(keystates[SDLK_KP_PLUS]) //Touche + du clavier numérique
                {
                    if(sim->zoom < 4)
                    {
                        simulationZoom(sim);
                    }
                }
                if(keystates[SDLK_KP_MINUS]) //touche - du clavier numérique
                {
                    if(sim->zoom > 1)
                    {
                        simulationDeZoom(sim);
                    }
                }
                break;

            case SDL_MOUSEMOTION:
            // @todo: améliorer la vitesse de détection de la souris sur ubuntu car pour l'instant la file d'attente d'evenements doit se remplir inutilement et se décharge d'un seul évenement (utile ou non) à chaque image.
            // ça mais environ deux seconde à décharger la file pour trouver SDL_MOUSEMOTION
            // il faudrait peut etre utiliser un  while(SDL_PollEvent(&sim->event))  et/ou  SDL_GetMouseState(&x,&y)  mais j'arrive pas l'utiliser correctement
                if(sim->event.motion.x < PRECISION_DETECTION_SOURIS*LONGUEUR_CASE && sim->pos_decoupage.x > -DISTANCE_DEPACEMENT_MAP) //Coté gauche de l'écran
                {
                    sim->pos_decoupage.x -= VITESSE_DEPLACEMENT_MAP;
                }
                if( sim->event.motion.x > (sim->largeur_ecran - PRECISION_DETECTION_SOURIS*LONGUEUR_CASE) && sim->pos_decoupage.x < sim->map.surface->w - sim->largeur_ecran + DISTANCE_DEPACEMENT_MAP) //Coté droit de l'écran
                {
                    sim->pos_decoupage.x += VITESSE_DEPLACEMENT_MAP;
                }
                if(sim->event.motion.y < PRECISION_DETECTION_SOURIS*LONGUEUR_CASE && sim->pos_decoupage.y > -DISTANCE_DEPACEMENT_MAP) //Coté haut de l'écran
                {
                    sim->pos_decoupage.y -= VITESSE_DEPLACEMENT_MAP;
                }
                if(sim->event.motion.y > (sim->hauteur_ecran - PRECISION_DETECTION_SOURIS*LONGUEUR_CASE) && sim->pos_decoupage.y < sim->map.surface->h - sim->hauteur_ecran + DISTANCE_DEPACEMENT_MAP) //Coté bas de l'écran
                {
                    sim->pos_decoupage.y += VITESSE_DEPLACEMENT_MAP;
                }
                break;
        }

        case SDL_MOUSEBUTTONDOWN:
            if (sim->event.button.button == SDL_BUTTON_WHEELDOWN)
            {
                if(sim->zoom > 1)
                {
                    simulationDeZoom(sim);
                }
            }
            if (sim->event.button.button == SDL_BUTTON_WHEELUP)
            {
                if(sim->zoom < 4)
                {
                    simulationZoom(sim);
                }
            }
            break;


        case SDL_MOUSEBUTTONUP:
            if (sim->event.button.button == SDL_BUTTON_LEFT)
            {
                sim->indStatistiques = compareIndividuSouris(sim, sim->event.button.x, sim->event.button.y);
            }
            break;


        case SDL_VIDEORESIZE:
            sim->largeur_ecran = sim->event.resize.w;
            sim->hauteur_ecran = sim->event.resize.h;
            sim->ecran = SDL_SetVideoMode(sim->largeur_ecran, sim->hauteur_ecran, 32, SDL_HWSURFACE|SDL_RESIZABLE|SDL_DOUBLEBUF);
            break;

        default:
            break;

    }
}


Individu* compareIndividuSouris (Simulation *sim, int x, int y)
{
    Espece *espece;
    Individu *ind;
    Vec pos;


    //projeter les coordonnées de la souris dans le plan de la map

    if( ((sim->map.surface->h) <= sim->hauteur_ecran) && ((sim->map.surface->w) <= sim->largeur_ecran) ) //la map est moins large et moins haute que l'écran
    {
        pos.x = (x/sim->zoom - (sim->largeur_ecran - sim->map.surface->w/sim->zoom)/2);
        pos.y = (y/sim->zoom - (sim->hauteur_ecran - sim->map.surface->h/sim->zoom)/2);
    }
    if( ((sim->map.surface->h) <= sim->hauteur_ecran) && ((sim->map.surface->w) > sim->largeur_ecran) ) //la map est plus large que l'écran
    {
        pos.x = (x + sim->pos_decoupage.x)/sim->zoom;
        pos.y = (y- ((sim->hauteur_ecran - sim->map.surface->h)/2)/sim->zoom );
    }
    if( ((sim->map.surface->w) <= sim->largeur_ecran) && ((sim->map.surface->h) > sim->hauteur_ecran) ) //la map est plus haute que l'écran
    {
        pos.y = (y + sim->pos_decoupage.y)/sim->zoom;
        pos.x = (x - ((sim->largeur_ecran - sim->map.surface->w)/2))/sim->zoom;
    }
    if( ((sim->map.surface->h) > sim->hauteur_ecran) && ((sim->map.surface->w) > sim->largeur_ecran) ) //sinon la map est plus large et plus haute que l'écran
    {
        pos.x = (x + sim->pos_decoupage.x)/sim->zoom;
        pos.y = (y + sim->pos_decoupage.y)/sim->zoom;
    }

    foreach(sim, espece, ind)
    {
        if(pos.x >= ind->position.x - (LARGEUR_ANIMAUX*sim->zoom) && pos.x <= ind->position.x + (LARGEUR_ANIMAUX*sim->zoom)*2)
        {
            if(pos.y >= ind->position.y - HAUTEUR_ANIMAUX*sim->zoom && pos.y <= ind->position.y + (HAUTEUR_ANIMAUX*sim->zoom)*2)
            {
                return ind;
            }
        }
    }

    return NULL;
}



void simulationZoom (Simulation *sim)
{
    Espece *espece = sim->premiereEspece;

    sim->zoom ++;

    sim->pos_decoupage.x = sim->pos_decoupage.x*2;
    sim->pos_decoupage.y = sim->pos_decoupage.y*2;

    mapZoom(&(sim->map),sim->zoom);

    while(espece != NULL)
    {
        creationSurfaceIndividu(sim,espece,sim->zoom);

        espece=espece->suivante;
    }
}


void simulationDeZoom (Simulation *sim)
{
    Espece *espece = sim->premiereEspece;

    sim->zoom --;

    sim->pos_decoupage.x=sim->pos_decoupage.x/2;
    sim->pos_decoupage.y=sim->pos_decoupage.y/2;

    mapZoom(&(sim->map),sim->zoom);

    while(espece != NULL)
    {
        creationSurfaceIndividu(sim,espece,sim->zoom);

        espece=espece->suivante;
    }
}


int quitSDL(Simulation* sim, bool quit_juste_sdl)
{
    Espece *espece = sim->premiereEspece;
    Individu *individu;

    if(quit_juste_sdl == 0)
    {
        while(espece != NULL)
        {
            individu = espece->premierIndividu;

            SDL_FreeSurface(espece->male);
            SDL_FreeSurface(espece->femelle);

            while(individu != NULL)
            {
                SDL_FreeSurface(individu->nom);

                individu = individu->suivant;
            }
            espece=espece->suivante;
        }
    }


    SDL_FreeSurface(sim->ecran);

    TTF_CloseFont(sim->police);
    TTF_CloseFont(sim->police_temps);

    TTF_Quit();

    SDL_QuitSubSystem(SDL_INIT_VIDEO);

    return EXIT_SUCCESS;
}
