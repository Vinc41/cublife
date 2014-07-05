#ifndef BARREOUTILS_H_INCLUDED
#define BARREOUTILS_H_INCLUDED

#include "simulation.h"

typedef struct BarreOutils
{
    GtkWidget *toolbar;

    GtkToolItem *nouveau;
    GtkToolItem *ouvrir;
    GtkToolItem *sauver;

    GtkToolItem *simuler;
    GtkToolItem *start;
    GtkToolItem *lent;
    GtkToolItem *rapide;


    GtkToolItem *preferences;
    GtkWidget *prefWindow;

    GtkWidget *scaleHauteurEcran;
    GtkWidget *scaleLargeurEcran;


    // pointeur vers la simulation déja créée dans la structure Interface.
    Simulation *s;

}BarreOutils;


void creerBarreOutils(BarreOutils*);

void actualiseBoutonSimulation(GtkWidget* boutonSimuler, BarreOutils*);
void actualiseBoutonStart(GtkWidget* boutonStart, BarreOutils*);

void actualiseBoutonRalentir(GtkWidget* boutonLent, BarreOutils*);
void actualiseBoutonAccelerer(GtkWidget* boutonRapide, BarreOutils*);
void actualiseInfoVitesse(BarreOutils*);


void afficheFenetrePreferences(GtkWidget* boutonPreferences, BarreOutils*);
void desactiveBoutonPreferences(GtkWidget* boutonPreferences, BarreOutils*);

void regenererMapGTK(GtkWidget* boutonGenererMap, Simulation* sim);

#endif // BARREOUTILS_H_INCLUDED
