#ifndef INDIVIDU_H_INCLUDED
#define INDIVIDU_H_INCLUDED

#include "commun.h"

typedef enum {MALE,FEMELLE,BOTH} Sexe;

typedef double Angle;


struct Individu
{
    int id;
    Sexe sexe;
    Temps naissance;
    Vec position;
    int sante;
    //int grandeur;
    Temps derniereReproduction;
    float stockEau;
    //int lumiereRecue;

    float vitesse;
    Angle direction;

    SDL_Surface *nom;
    bool repos;

    float fatigue;
    float stockNourriture;
    //struct Individu *cible;

    struct Individu *suivant;
    struct Espece *espece;
};
typedef struct Individu Individu;




#endif // INDIVIDU_H_INCLUDED
