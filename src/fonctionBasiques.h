#ifndef FONCTIONBASIQUES_H_INCLUDED
#define FONCTIONBASIQUES_H_INCLUDED

#include "espece.h"

Individu* initIndividus();
Espece* initEspece(int nombre);
void nouvelleEspece(Espece** listeE);
void ajoutIndividu(Individu** listeI, Sexe s, Espece* espece, Temps naissance);
void remplissage(Espece* espece, Individu* ind, Sexe s);
Individu* rechercheParPos(Individu* listeI, Vec pos);
void deplacementAleatoire(Individu* ind);
void regulerVitesse(Individu* ind);
bool rayonAction(Vec pos1, Vec pos2);
float distance(Vec pos1, Vec pos2);

float Random (int Min, int Max);

void supprimerIndividus (Espece *espece);

void mortIndividu(Individu* ind);

void supprimerEspeceEtIndividu (Espece **listeEspece);

char** initTableauChaineCaractrere ();
char** augmenterTailleTabChaineCaractere(int taille, char** tab, int nombre_case_en_plus);
void supprimerTabChaineCaractere (char** tab,int taille,int nombre_case_en_plus);
#endif // FONCTIONBASIQUES_H_INCLUDED
