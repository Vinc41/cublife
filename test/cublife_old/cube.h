#ifndef CUBE_H_INCLUDED
#define CUBE_H_INCLUDED

#include "general.h"
#include "deplacement.h"


void actualiserCubType(CubType* typeCub);

void initCubType(CubType* typeCube, int nombre, Uint32 couleur);

void ajouterCub(CubType* typeCube);

void dupliquerCub(CubType* typeCube);

void cubDebut(CubType* typeCube);

void cubSuivant(CubType* typeCube);

void supprimerCub(CubType* type);

void manger(CubType* predateur);

float norme(Vec2 pos1, Vec2 pos2);


#endif // CUBE_H_INCLUDED
