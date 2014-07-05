#include "interface_GTK.h"
#include <unistd.h>


int main(int argc, char *argv[])
{
    Interface gtk;

    gtk.quitter = false;
    initSimulation(&(gtk.s));
    initListeEspece(&(gtk.s));

    gtk.especeSelect = gtk.s.premiereEspece;

    creerInterface(&gtk);




    if(argc == 2 ) // si le programme à été lancé à partir d'un fichier .save
    {
        int i;
        char dossier_lancement[30];
        getcwd (dossier_lancement, 30); // récupération du dossier de lancement de la sauvegarde
        for (i= strlen(argv[0]) ; i>0; i--)
        {
            if (argv[0][i] == '\\')
            /* Lorsqu'on tombe sur le premier "\", on commence à recopier chemin_programme dans dossier */
            {
                dossier_lancement[i] = '\0';
                i--; /* On passe au caractère précédent pour commencer à recopier le dossier de lancement */
                while (i >= 0)
                {
                    dossier_lancement[i] = argv[0][i];
                    i--;
                }
            }
        }
        chdir(dossier_lancement); // changer le répertoire de lancement du programme pour trouver les dll
        chargerSimulation(&(gtk.s), argv[1]);
        actualiseListeEspeceGTK(gtk.listeEspeces, &gtk);
    }

    while(!gtk.quitter)
    {
        while (gtk_events_pending())
        {
            gtk_main_iteration(); //actualisation des fenêtres GTK
        }


        if (gtk.s.sdlInitialisee)   //mise à 1 et fonction initSDL faites
                                    //par gtk quand on clique sur "Simuler"
        {
            //on véréfie les événements
            SDLevent(&(gtk.s));

            //on efface l'ecran
            SDL_FillRect(gtk.s.ecran, NULL, SDL_MapRGB((gtk.s.ecran)->format, 0, 0, 0));

            //On actualise les dimensions de l'ecran puis on colle la map sur l'ecran
            actualiserEcran(&(gtk.s));

            if (gtk.s.simulationLancee) //mis à 1 par gtk quand on clique sur "play" avec la fonction actualiseBoutonStart()
            {
                gtk.s.tempsPasse += gtk.s.coeffTemps/SDL_getFramerate(&(gtk.s.fps));
                actualiserIndividus(&(gtk.s));
            }
            afficheIndividus(&(gtk.s),gtk.s.zoom,gtk.s.indStatistiques);
            afficheTemps(&(gtk.s));

            SDL_Flip(gtk.s.ecran);

            if(gtk.s.event.type == SDL_QUIT)    //désallocation de la simulation uniquement à la fin
                                                //quand on est sur qu'elle ne sera plus utilisée
            {
                gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(gtk.barreOutils.simuler), false);
                //envoie un signal qui appelle actualiseBoutonSimulation() qui appelle quitSDL()
            }
        }

        SDL_framerateDelay(&(gtk.s.fps));
    }

    if(gtk.s.map.mapInitialisee == 1)
    {
        deleteMap(&(gtk.s.map));
    }
    SDL_Quit();
    free(gtk.s.nomSimulation);
    printf("\nprogramme ferme avec succes");
    return 0;
}
