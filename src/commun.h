#ifndef COMMUN_H_INCLUDED
#define COMMUN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include <SDL/SDL.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_framerate.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL_rotozoom.h>

#include <gtk/gtk.h>


///Constante pour la taille maximum des chaines de caractères
#define TAILLE_MAX 50

///Constante Map
#define LONGUEUR_CASE 16
#define NB_CASE_LARGEUR_DEFAUT 30
#define NB_CASE_HAUTEUR_DEFAUT 30
#define NB_CASE_LARGEUR_MAX 200
#define NB_CASE_HAUTEUR_MAX 150
#define PETITE_MAP 2500
#define GRANDE_MAP 9000

///Constante Event
#define PRECISION_DETECTION_SOURIS 4
#define VITESSE_DEPLACEMENT_MAP 3
#define DISTANCE_DEPACEMENT_MAP 50

///Constante taille du texte
#define TAILLE_POLICE_INDIVIDUS 16
#define TAILLE_POLICE_TEMPS 50

///Constante Individu
#define LARGEUR_ANIMAUX 8
#define HAUTEUR_ANIMAUX 8
#define RAYON_ANIMAUX 4
#define MARGE_AFFICHAGE_INDIVIDU 20

///Constante Temps
#define PAS_COEFF_TEMPS 1.5

///Constante mois
#define janvier 31
#define fevrier 59
#define mars 90
#define avril 120
#define mai 151
#define juin 181
#define juillet 212
#define aout 243
#define septembre 273
#define octobre 304
#define novembre 334
#define decembre 365

///Macro foreach
#define foreach(s, esp, ind) for(esp=s->premiereEspece ; esp!=NULL ; esp=esp->suivante) for(ind=esp->premierIndividu ; ind!=NULL ; ind=ind->suivant)


//typedef enum {false, true} bool;

typedef Uint32 Temps; // durée de 49 jours max

//typedef enum {JANVIER,FEVRIER,MARS,AVRIL,MAI,JUIN,JUILLET,AOUT,SEPTEMBRE,OCTOBRE,NOVEMBRE,DECEMBRE}MOIS;

typedef struct Vec
{
    float x;
    float y;
    //float z;
}Vec;


#endif // COMMUN_H_INCLUDED
