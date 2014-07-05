#ifndef SIMULATION_H_INCLUDED
#define SIMULATION_H_INCLUDED

#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif


#include "interface_SDL.h"



void initSimulation (Simulation* sim);

void initListeEspece(Simulation* sim);
Espece* ajoutEspece(Simulation* sim);

void RAZSimulation (Simulation* sim);

void actualiserIndividus(Simulation* sim);
bool manger(Individu* ind,Individu* proie);
int deplacement(Individu* ind, Simulation *sim);

void reproduction(Individu* ind,Individu* proche,Simulation *sim);

Individu* quiEstProche(Individu* ind,Simulation* sim);

int sauvegardeEspece (Espece* listeEspece, FILE* saveFile);
void sauvegardeIndividus (FILE* fichierIndividu, Individu* listeIndividu);

int chargerEspece (Espece** listeEspece, FILE *saveFile);
void chargerIndividus (FILE* fichierIndividu, Individu** listeIndividu, Espece* espece, int pop);

void supprimerEspece (Simulation* sim, Espece *espece);

int sauvegardeSimulation (Simulation* sim, char* saveName);
int chargerSimulation (Simulation *sim, char* saveName);

char* enleverChemin (char* nomAvecChemin);

void supprimerSauvegarde (char *saveName);


#endif // SIMULATION_H_INCLUDED
