#ifndef INTERFACE_GTK_H_INCLUDED
#define INTERFACE_GTK_H_INCLUDED

#include "barreOutils.h"

//liste des conteneurs :
//window
// - box (VBox)
//    - toolbar (barreOutil)
//    - paned
//       - frame (Liste)
//          - box (colonne)
//             - scrolled_window (defil)
//             - box (boutons)
//       - frame (Caractéristiques)
//          - scrolled_window (defil)
//             - grid
//                - label  |  (valeur)
//                -  ...   |    ...
//    - statusbar (barreEtat)


#define FENETRE_LARGEUR_DEFAUT 500
#define FENETRE_HAUTEUR_DEFAUT 300



typedef struct Interface
{
    GtkWidget *window;

    BarreOutils barreOutils;

    GtkListStore *listeEspeces;
    GtkWidget *vueListe;
    GtkTreeSelection *selection;
    Espece *especeSelect; //espèce sélectionnée

    GtkTreeViewColumn *colonnesEspeces;
    GtkToolItem *ajouter;
    GtkToolItem *supprimer;

    GtkWidget *paned;
    GtkWidget *frame; //Caractéristiques

    GtkWidget *entryNom;
    GtkWidget *listeType;
    GtkWidget *scaleVitesse; // pour enregistrer l'adresse du widget qui sera modifiée dans changerValeurType()
    //les autres scales n'ont pas besoin d'être enregistrés


    Simulation s;

    bool quitter;

}Interface;



// repères des colonnes de la liste des espèces
enum {
    COLONNE_ADRESSE,    //pour accéder à toutes les caractéristiques
    COLONNE_NOM,        //sera affiché
    COLONNE_POPULATION, //sera affiché
    N_COLONNES          //pour donner au ListeStore le nombre de colonnes
};


// crée la fenêtre principale et certains widgets. Fait appel aux autres fonctions gtk
void creerInterface(Interface* g);


void creerListeEspeceGTK(GtkWidget *box, Interface* g);
void actualiseListeEspeceGTK(GtkListStore *listeEspeces, Interface* g);// recrée totalement la liste gtk à partir de la liste chainée des espèces
void actualiseNomEspeceGTK(GtkTreeSelection *selection, Interface* g); // quand un nom est modifié

void suppressionIntegraleEspeceGTK(GtkWidget *boutonNouveau, Interface* g);
void ajoutEspeceGTK(GtkWidget *boutonAjout, Interface* g);          // quand on clique sur le bouton ajouter
void supprimerEspeceGTK(GtkWidget *boutonSuppr, Interface* g);      // quand on clique sur le bouton supprimer
void actualisePopulation();     // quand la population a changé



// crée et affiche les nouveaux widgets de modif des caractéristiques quand l'espèce selectionnée a changée
void actualiseCaractEspece(GtkTreeSelection *selection, Interface *g);

// crée les widgets de saisie de nombre du type :  min---\/----max et les accrochent à la grille
void creerScale (GtkWidget** scale, GtkWidget* grid, char* nom, int min, int max, double pas, int* ligne, char* tooltip);



/// fonctions qui modifient les carractéristiques des espèces avec les signaux des widgets
void changerValeurNom(GtkWidget* entry, Interface *g);
void changerValeurEntry(GtkWidget* entry, char* mot);
void changerValeurType(GtkWidget* comboBox, Interface *g);
void changerValeurCouleur(GtkWidget* boutonCouleur, Interface* g);
void changerValeurScaleInt(GtkWidget* scale, int* valeurAChanger);
void changerValeurScaleFloat(GtkWidget* scale, float* valeurAChanger);


void afficheFenetreOuvrir(GtkWidget* dialog, Interface* g);
void afficheFenetreSauver(GtkWidget* sauver, Interface* g);

void RAZSimulationGTK(GtkWidget* bouton, Interface* g);

void quitGTK(GtkWidget* window, Interface* g);




#endif // INTERFACE_H_INCLUDED
