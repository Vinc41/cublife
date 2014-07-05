#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#include "fonctionBasiques.h"


typedef enum {TERRE,EAU}TYPE;

typedef enum {PETITE,MOYENNE,GRANDE}TAILLE;

typedef TYPE** MATRICE;

typedef struct
{
    bool mapInitialisee;

    SDL_Surface *surface;
    SDL_Surface *terre, *eau;
    MATRICE cases;
    int largeur_map;
    int hauteur_map;
    TAILLE taille;
    int nb_cases_terre;
    int nb_cases_eau;
}MAP;

typedef struct
{
    SDL_Surface *etat_chargement;
    SDL_Surface *barre_chargement;
    SDL_Surface *texteTitre;
    SDL_Surface *texteSecondaire;

    TTF_Font *police;

}CHARGEMENT;

typedef struct Simulation
{
    bool sdlInitialisee;

    char *nomSimulation;

    //execution: 1 ou stop: 0
    bool simulationLancee;

    //défilement du temps, 1 par defaut
    float coeffTemps;

    //temps entre chaque affichage d'écran
    FPSmanager fps;

    //temps formaté
    int annee;
    int mois;

    // défile uniquement quand on appuis sur play
    float tempsPasse; // en secondes

    // mode 2D: 0, mode 3D: 1
    bool dim3D;

    //liste d'espèce
    Espece* premiereEspece;
    int nombreEspeces;

    //Variable SDL
    MAP map;

    SDL_Surface *ecran;
    SDL_Surface *surface_temps;
    SDL_Event event;
    SDL_Rect pos_decoupage, pos_collage;

    int zoom;
    SDL_Surface *surface_map_zoom;

    Individu* indStatistiques;

    //Texte
    TTF_Font *police;
    TTF_Font *police_temps;
    SDL_Color couleurBlanche;

    //Taille de l'écran
    int largeur_ecran, hauteur_ecran;

    //sauvegarde
    bool genererNouvelleMap;
    bool genererPosEspeces;

}Simulation;



MATRICE initCases (int largeur, int hauteur);
void initMap (MAP *map);

int genererMap (MAP *map, Simulation *sim);

void genererMapAletatoire (MAP *map);
void lissageMap (Simulation *sim, MAP *map, CHARGEMENT *charg);

void collerMapTexture (MAP *map, int zoom);
int CalculCoefLissage (MAP *map);

void mapZoom (MAP *map, int zoom);

TYPE interactionIndividuMap (Simulation *sim, Individu *individu);

CHARGEMENT* initChargement ();
void actualiserChargement (Simulation *sim, CHARGEMENT *chargement, char *texteTitre, char *texteSecondaire, int pourcentage, bool actualiserTexte);
void affichageChargement (Simulation *sim, CHARGEMENT *struct_charg, bool actualiserTexte);
void supprimerChargement (CHARGEMENT* charg);

int sauvegardeMap (MAP map, FILE *fichierMap);
int chargerMap (Simulation* sim, MAP *map, FILE* fichierMap, CHARGEMENT* charg);

void deleteMap (MAP *map);
void delCases (MATRICE cases, int largeur, int hauteur);

#endif // MAP_H_INCLUDED
