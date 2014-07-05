#ifndef INTERFACE_SDL_H_INCLUDED
#define INTERFACE_SDL_H_INCLUDED

#include "map.h"

int initSDL(Simulation* sim, bool init_juste_SDL);
void initIndSDL(Simulation* sim, bool firstInit);

void creationSurfaceIndividu (Simulation* sim, Espece *espece, int zoom);
void ajoutSDLIndividu(Simulation *sim, Individu *individu, char *nomEspece, bool genererPosAleatoire);

char* formaterTemps (Simulation *sim, float tempsSimulation);
void afficheTemps (Simulation* sim);

void actualiserNomIndividu (Simulation* sim, Individu *individu, char *nomEspece);
void actualiserEcran(Simulation* sim);
void afficheIndividus (Simulation *sim, int zoom, Individu* i);
void afficheStatistiquesInd (Simulation *sim, SDL_Rect* position);

SDL_Surface* creationBarreStat (Simulation *sim, int pourcentage, bool inverserBarre);
SDL_Surface* creationSurfaceStat(Simulation* sim, int stat, char* nomStat, bool statInverser);

void SDLevent (Simulation* sim);

Individu* compareIndividuSouris (Simulation *sim, int x, int y);

void simulationZoom (Simulation *sim);
void simulationDeZoom (Simulation *sim);

int quitSDL(Simulation* sim, bool quit_juste_sdl);


#endif
