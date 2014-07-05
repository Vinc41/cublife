#include "barreOutils.h"

void creerBarreOutils(BarreOutils *barre)
{
    //création de la barre d'outil
    barre->toolbar = gtk_toolbar_new();
    gtk_toolbar_set_style (GTK_TOOLBAR(barre->toolbar), GTK_TOOLBAR_BOTH_HORIZ);

    //remplissage de la barre d'outil
    barre->nouveau = gtk_tool_button_new_from_stock(GTK_STOCK_NEW);
    gtk_tool_item_set_tooltip_text(barre->nouveau, "Nouveau ecosysteme");
    gtk_toolbar_insert(GTK_TOOLBAR(barre->toolbar), barre->nouveau, -1);

    barre->ouvrir = gtk_tool_button_new_from_stock(GTK_STOCK_OPEN);
    gtk_tool_item_set_tooltip_text(barre->ouvrir, "Ouvrir");
    gtk_toolbar_insert(GTK_TOOLBAR(barre->toolbar), barre->ouvrir, -1);

    barre->sauver = gtk_tool_button_new_from_stock(GTK_STOCK_SAVE);
    gtk_widget_set_sensitive(GTK_WIDGET(barre->sauver), FALSE);
    gtk_tool_item_set_tooltip_text(barre->sauver, "Sauvegarder");
    gtk_toolbar_insert(GTK_TOOLBAR(barre->toolbar), barre->sauver, -1);


    gtk_toolbar_insert(GTK_TOOLBAR(barre->toolbar), gtk_separator_tool_item_new(), -1);


    barre->simuler = gtk_toggle_tool_button_new_from_stock(GTK_STOCK_EXECUTE);
    g_signal_connect(G_OBJECT(barre->simuler), "clicked", G_CALLBACK(actualiseBoutonSimulation), barre);
    gtk_toolbar_insert(GTK_TOOLBAR(barre->toolbar), barre->simuler, -1);
    gtk_tool_button_set_label(GTK_TOOL_BUTTON(barre->simuler), "Simuler");
    gtk_tool_item_set_is_important(barre->simuler, TRUE);

    barre->start = gtk_toggle_tool_button_new_from_stock(GTK_STOCK_MEDIA_PLAY);
    g_signal_connect(G_OBJECT(barre->start), "clicked", G_CALLBACK(actualiseBoutonStart), barre);
    gtk_toolbar_insert(GTK_TOOLBAR(barre->toolbar), barre->start, -1);

    barre->lent = gtk_tool_button_new_from_stock(GTK_STOCK_MEDIA_REWIND);
    g_signal_connect(G_OBJECT(barre->lent), "clicked", G_CALLBACK(actualiseBoutonRalentir), barre);
    gtk_toolbar_insert(GTK_TOOLBAR(barre->toolbar), barre->lent, -1);

    barre->rapide = gtk_tool_button_new_from_stock(GTK_STOCK_MEDIA_FORWARD);
    g_signal_connect(G_OBJECT(barre->rapide), "clicked", G_CALLBACK(actualiseBoutonAccelerer), barre);
    gtk_toolbar_insert(GTK_TOOLBAR(barre->toolbar), barre->rapide, -1);

    gtk_toolbar_insert(GTK_TOOLBAR(barre->toolbar), gtk_separator_tool_item_new(), -1);


    barre->preferences = gtk_toggle_tool_button_new_from_stock(GTK_STOCK_PREFERENCES);
    g_signal_connect(G_OBJECT(barre->preferences), "clicked", G_CALLBACK(afficheFenetrePreferences), barre);
    gtk_toolbar_insert(GTK_TOOLBAR(barre->toolbar), barre->preferences, -1);
    gtk_tool_button_set_label(GTK_TOOL_BUTTON(barre->preferences), "Preferences");
    gtk_tool_item_set_is_important(barre->preferences, TRUE);



    // actualise une fois pour mettre les valeurs par défaut au lancement du programme
    actualiseBoutonSimulation(GTK_WIDGET(barre->simuler), barre);
    actualiseBoutonStart(GTK_WIDGET(barre->start), barre);
    actualiseInfoVitesse(barre);
}



void actualiseBoutonSimulation(GtkWidget* boutonSimuler, BarreOutils* barre)
{
    if (gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(boutonSimuler)))
    {
        gtk_widget_set_sensitive(GTK_WIDGET(barre->start), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(barre->lent), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(barre->rapide), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(barre->sauver), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(barre->ouvrir), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(barre->preferences), FALSE);
        gtk_tool_item_set_tooltip_text(barre->simuler, "Arreter la simulation");

        initSDL(barre->s,false);

        barre->s->sdlInitialisee = true;
    }
    else
    {
        gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(barre->start), FALSE);
        actualiseBoutonStart(GTK_WIDGET(barre->start), barre);
        gtk_widget_set_sensitive(GTK_WIDGET(barre->start), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(barre->lent), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(barre->rapide), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(barre->ouvrir), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(barre->preferences), TRUE);
        gtk_tool_item_set_tooltip_text(barre->simuler, "Initialiser la simulation");

        if (barre->s->sdlInitialisee)
        {
            printf("la sdl va quitter...");
            quitSDL(barre->s,0);
            printf("OK\n");
            barre->s->sdlInitialisee = false;
        }
    }
}




void actualiseBoutonStart(GtkWidget* boutonStart, BarreOutils* barre)
{
    if (gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(barre->start)))
    {
        gtk_tool_item_set_tooltip_text(barre->start, "Suspendre la simulation");
        barre->s->simulationLancee = 1;
    }
    else
    {
        gtk_tool_item_set_tooltip_text(barre->start, "Demarrer la simulation");
        barre->s->simulationLancee = 0;
    }
}





void actualiseBoutonRalentir(GtkWidget* boutonLent, BarreOutils* barre)
{
    barre->s->coeffTemps /= PAS_COEFF_TEMPS;
    actualiseInfoVitesse(barre);
}



void actualiseBoutonAccelerer(GtkWidget* boutonRapide, BarreOutils* barre)
{
    barre->s->coeffTemps *= PAS_COEFF_TEMPS;
    actualiseInfoVitesse(barre);
}



void actualiseInfoVitesse(BarreOutils *barre)
{
    char nom[30];
    sprintf(nom, "Ralentir a x%1.2g", barre->s->coeffTemps / PAS_COEFF_TEMPS);
    gtk_tool_item_set_tooltip_text(barre->lent, nom);
    sprintf(nom, "Accelerer a x%1.2g", barre->s->coeffTemps * PAS_COEFF_TEMPS);
    gtk_tool_item_set_tooltip_text(barre->rapide, nom);
}





void afficheFenetrePreferences(GtkWidget* boutonPreferences, BarreOutils* barre)
{
    if (gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(barre->preferences)))
    {
        GtkWidget *grid;
        GtkWidget *boutonGenererMap;

        barre->prefWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title (GTK_WINDOW (barre->prefWindow), "Preferences");
        gtk_window_set_default_size(GTK_WINDOW(barre->prefWindow), 300, 150);
        g_signal_connect(G_OBJECT(barre->prefWindow), "destroy",
                         G_CALLBACK(desactiveBoutonPreferences), barre);

        grid = gtk_grid_new();

        gtk_grid_attach (GTK_GRID(grid), gtk_label_new("Hauteur ecran "), 0, 0, 1, 1);
        barre->scaleHauteurEcran = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 2, NB_CASE_HAUTEUR_MAX, 1);
        gtk_range_set_value(GTK_RANGE(barre->scaleHauteurEcran), barre->s->map.hauteur_map);
        gtk_grid_attach (GTK_GRID(grid), barre->scaleHauteurEcran, 1, 0, 1, 1);
        gtk_widget_set_hexpand(barre->scaleHauteurEcran, TRUE); // toute la colonne s'étendra

        gtk_grid_attach (GTK_GRID(grid), gtk_label_new("Largeur ecran "), 0, 1, 1, 1);
        barre->scaleLargeurEcran = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 2, NB_CASE_LARGEUR_MAX, 1);
        gtk_range_set_value(GTK_RANGE(barre->scaleLargeurEcran), barre->s->map.largeur_map);
        gtk_grid_attach (GTK_GRID(grid), barre->scaleLargeurEcran, 1, 1, 1, 1);

        boutonGenererMap = gtk_button_new_with_label("Recharger la carte");
        g_signal_connect(G_OBJECT(boutonGenererMap), "clicked", G_CALLBACK(regenererMapGTK), barre->s);
        gtk_grid_attach (GTK_GRID(grid), boutonGenererMap, 0, 2, 1, 1);

        gtk_container_add(GTK_CONTAINER(barre->prefWindow), grid);


        gtk_widget_show_all(barre->prefWindow);
    }
    else
    {
        barre->s->map.hauteur_map = gtk_range_get_value(GTK_RANGE(barre->scaleHauteurEcran));
        barre->s->map.largeur_map = gtk_range_get_value(GTK_RANGE(barre->scaleLargeurEcran));

        gtk_widget_destroy(GTK_WIDGET(barre->prefWindow));
        barre->prefWindow = NULL;
    }

}

void desactiveBoutonPreferences(GtkWidget* boutonPreferences, BarreOutils* barre)
{
        barre->s->map.hauteur_map = gtk_range_get_value(GTK_RANGE(barre->scaleHauteurEcran));
        barre->s->map.largeur_map = gtk_range_get_value(GTK_RANGE(barre->scaleLargeurEcran));
        barre->s->genererNouvelleMap = 1;

    gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(barre->preferences), FALSE);
}


void regenererMapGTK(GtkWidget* boutonGenererMap, Simulation* sim)
{
    sim->genererNouvelleMap = 1;
}
