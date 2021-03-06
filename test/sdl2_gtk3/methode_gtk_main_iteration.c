/// Programme de test permettant d'ouvrir une fenetre SDL2 et GTK+3
/// Les deux fenêtres se réactualisent dans la même boucle.
/// Utilisation de gtk_main_iteration()
/// ATTENTION : - problème quand on quitte les fenetres ?
///             - ne pas oublier d'inclure ant.bmp
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

typedef enum {false, true} bool;


static void maFonction(GtkRange *scale, SDL_Rect* rect)
{
    printf("%f\n",  gtk_range_get_value(scale));
    rect->x = gtk_range_get_value(scale);
}

static void helloWorld (GtkWidget *wid, GtkWidget *win)
{
  GtkWidget *dialog = NULL;

  dialog = gtk_message_dialog_new (GTK_WINDOW (win), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Hello World!");
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}

void createFenetreConfig(SDL_Rect* rect)
{
    GtkWidget *button = NULL;
    GtkWidget *win = NULL;
    GtkWidget *box = NULL;
    GtkWidget *barre = NULL;

    // Initialize GTK+
    g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
    gtk_init (NULL, NULL);
    g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

    // Create the main window
    win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (win), 8);
    gtk_window_set_title (GTK_WINDOW (win), "Parametres");
    gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);
    gtk_widget_realize (win);
    g_signal_connect (win, "destroy", gtk_main_quit, NULL);

    // Create a vertical box with buttons
    box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 6);
    gtk_container_add (GTK_CONTAINER (win), box);

    button = gtk_button_new_from_stock (GTK_STOCK_DIALOG_INFO);
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (helloWorld), (gpointer) win);
    gtk_box_pack_start (GTK_BOX (box), button, TRUE, TRUE, 0);

    //Création de la barre
    barre = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 800, 5);
    g_signal_connect (G_OBJECT (barre), "value-changed", G_CALLBACK (maFonction), (gpointer) rect);
    gtk_box_pack_start (GTK_BOX (box), barre, TRUE, TRUE, 0);

    button = gtk_button_new_from_stock (GTK_STOCK_CLOSE);
    g_signal_connect (button, "clicked", gtk_main_quit, NULL);
    gtk_box_pack_start (GTK_BOX (box), button, TRUE, TRUE, 0);

    // Enter the main loop
    gtk_widget_show_all (win);
}



int main(int argc, char *argv[]) // choie du mode étude
{
    SDL_Window* fenetre = NULL;
    SDL_Event evenements;
    bool terminer = false;
    SDL_Renderer *renderer;
    SDL_Surface *surf = NULL;
    SDL_Rect rect;
    SDL_Texture* texture = NULL;

    createFenetreConfig(&rect); //crée la fenetre GTK

    // Initialisation de la SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf( "Erreur lors de l'initialisation de la SDL : %s\n", SDL_GetError() );
        SDL_Quit();
        return -1;
    }

    // Création de la fenêtre
    fenetre = SDL_CreateWindow("Visualisation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(fenetre, -1, 0);

    surf = SDL_LoadBMP("ant.bmp");
    texture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    rect.x = 20;
    rect.y = 20;
    rect.w = 48;
    rect.h = 48;

    // Boucle principale
    while(!terminer)
    {
        while (gtk_events_pending())
        {
            gtk_main_iteration();
        }
        SDL_PollEvent(&evenements);
        if(evenements.window.event == SDL_WINDOWEVENT_CLOSE)
        {
            terminer = true;
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy (renderer,texture, NULL,&rect);
        SDL_RenderPresent(renderer);
    }

    gtk_main_quit();
    // On quitte la SDL
    SDL_DestroyWindow(fenetre);
    SDL_Quit();

    return 0;
}
