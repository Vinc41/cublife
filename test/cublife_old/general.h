#ifndef GENERAL_H_INCLUDED
#define GENERAL_H_INCLUDED

#define GRANDEUR_CUBE 4


typedef enum {false, true} bool;


typedef float Angle;


typedef struct {
    float x, y;
} Vec2 ;


struct Cub {
    SDL_Rect posEcran;
    Vec2 pos, direction;
    unsigned int dateNaissance, dateDuplication;
    int stockNourriture;
    struct Cub *suivant;
};
typedef struct Cub Cub;



struct CubType {
    Cub *premier, *courant, *precedent;
    SDL_Surface *surface;
    Uint32 couleur;
    int vitesse , probChangementDirection , ricoche;
    int dureeVieMin, dureeVieMax, delaiDuplicationMin, delaiDuplicationMax;
    struct CubType *proie;
};
typedef struct CubType CubType;

#endif // GENERAL_H_INCLUDED
