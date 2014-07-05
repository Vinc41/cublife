#ifndef ESPECE_H_INCLUDED
#define ESPECE_H_INCLUDED

#include "individu.h"


typedef enum
{
    VEGETAL,
    CHAROGNARD,
    HERBIVORE,
    HOMNIVORE,
    CARNIVORE,
}Type;


typedef enum
{
    TERRESTRE,
    MER,
    CIEL,
}Milieu;


typedef SDL_Surface *Image;



struct Espece
{
    int id;
    Type type;
    char nom[TAILLE_MAX];
    int population;
    int populationInitiale;
    Milieu milieu;

    GdkRGBA couleur;
    Image male;
    Image femelle;
    //Image vieux;
    //Image mort;

    //int santeDebut;
    Temps dureeVieMoyenne;
    Temps vieEcartType;

    Temps ageMaturite;
    Temps delaiProchaineReproduction;
    int vitesseDeveloppement;

    int besoinLumiere;
    int besoinEau;

    float vitesseMoyenne;
    float acceleration;

    int besoinNourriture;

    struct Espece* suivante;
    Individu *premierIndividu;


};
typedef struct Espece Espece;


#endif // ESPECE_H_INCLUDED
