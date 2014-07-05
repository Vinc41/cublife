#include "interface_GTK.h"


void creerInterface(Interface* g)
{
    GtkWidget *VBox;
    GtkWidget *frame;

    GtkWidget *colonne, *barreBoutons;
    GtkToolItem *bouton;

    GtkWidget *barreEtat;


    gtk_init (NULL, NULL);

    g->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (g->window), "Cublife - Configuration");
    gtk_window_set_default_size(GTK_WINDOW(g->window), FENETRE_LARGEUR_DEFAUT, FENETRE_HAUTEUR_DEFAUT);
    g_signal_connect(G_OBJECT(g->window), "destroy", G_CALLBACK(quitGTK), g);


    //séparation en 3 parties verticales
    VBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(g->window), VBox);



    /// BARRE D'OUTILS
    //permet à la barre d'outils d'accéder à la simulation sans passer par la structure Interface
    g->barreOutils.s = &(g->s);

    creerBarreOutils(&(g->barreOutils));
    gtk_box_pack_start(GTK_BOX(VBox), g->barreOutils.toolbar, FALSE, FALSE, 0);


    g_signal_connect(G_OBJECT(g->barreOutils.nouveau), "clicked", G_CALLBACK(suppressionIntegraleEspeceGTK), g);
    g_signal_connect(G_OBJECT(g->barreOutils.ouvrir), "clicked", G_CALLBACK(afficheFenetreOuvrir), g);
    g_signal_connect(G_OBJECT(g->barreOutils.sauver), "clicked", G_CALLBACK(afficheFenetreSauver), g);



    /// CONTENU

    //création de ce qu'il y a en dessous de la barre d'outil
    g->paned = gtk_paned_new (GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(VBox), g->paned, TRUE, TRUE, 0);

    /// LISTE
    // à gauche
    frame = gtk_frame_new ("Liste des especes");
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
    gtk_widget_set_size_request (frame, 200, -1);

    colonne = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(frame), colonne);
    gtk_paned_pack1 (GTK_PANED (g->paned), frame, FALSE, FALSE);

    //l'affichage de la liste des espèces
    creerListeEspeceGTK(colonne, g);

    //boutons
    barreBoutons = gtk_toolbar_new();
    gtk_toolbar_set_style (GTK_TOOLBAR(barreBoutons), GTK_TOOLBAR_ICONS);
    gtk_box_pack_start(GTK_BOX(colonne), barreBoutons, FALSE, FALSE, 0);

    g->ajouter = gtk_tool_button_new_from_stock(GTK_STOCK_ADD);
    gtk_tool_item_set_tooltip_markup(g->ajouter, "<b>Ajouter</b> un espece");
    g_signal_connect(G_OBJECT(g->ajouter), "clicked", G_CALLBACK(ajoutEspeceGTK), g);
    gtk_toolbar_insert(GTK_TOOLBAR(barreBoutons), g->ajouter, -1);

    g->supprimer = gtk_tool_button_new_from_stock(GTK_STOCK_REMOVE);//GTK_STOCK_DELETE
    gtk_tool_item_set_tooltip_markup(g->supprimer, "<b>Supprimer</b> l'espece selectionnee");
    g_signal_connect(G_OBJECT(g->supprimer), "clicked", G_CALLBACK(supprimerEspeceGTK), g);
    gtk_toolbar_insert(GTK_TOOLBAR(barreBoutons), g->supprimer, -1);

    bouton = gtk_tool_button_new_from_stock(GTK_STOCK_CLEAR);//GTK_STOCK_DELETE
    gtk_tool_item_set_tooltip_markup(bouton, "<b>Reinitialiser</b> toutes les especes a la population d'origine");
    g_signal_connect(G_OBJECT(bouton), "clicked", G_CALLBACK(RAZSimulationGTK), g);
    gtk_toolbar_insert(GTK_TOOLBAR(barreBoutons), bouton, -1);

    /// CARACTÉRISTIQUES
    //les caractéristiques de l'espèce sélectionnée à droite
    g->frame = NULL;
    actualiseCaractEspece(g->selection, g);


    /// BARRE D'ÉTAT

    barreEtat = gtk_statusbar_new();
    gtk_box_pack_start(GTK_BOX(VBox), barreEtat, FALSE, FALSE, 0);



    gtk_widget_show_all(g->window);
}




void creerListeEspeceGTK(GtkWidget *box, Interface* g)
{
    GtkWidget *barreDefilement;
    GtkCellRenderer *renduCellule;

    g->listeEspeces = gtk_list_store_new(N_COLONNES, G_TYPE_POINTER, G_TYPE_STRING, G_TYPE_INT);


    actualiseListeEspeceGTK(g->listeEspeces, g);

    // Creation de la vue
    g->vueListe = gtk_tree_view_new_with_model(GTK_TREE_MODEL(g->listeEspeces));

    g->selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(g->vueListe));
    g_signal_connect(G_OBJECT(g->selection), "changed", G_CALLBACK(actualiseCaractEspece), g);

    // Creation de la premiere colonne
    renduCellule = gtk_cell_renderer_text_new();
    g->colonnesEspeces = gtk_tree_view_column_new_with_attributes("Nom",
        renduCellule,
        "text", COLONNE_NOM,
        NULL);

    // Ajout de la colonne à la vue
    gtk_tree_view_append_column(GTK_TREE_VIEW(g->vueListe), g->colonnesEspeces);

    /*// Creation de la deuxieme colonne //@todo : réactiver ce code quand l'affichage de la population pourra s'actualiser
    renduCellule = gtk_cell_renderer_text_new();
    g->colonnesEspeces = gtk_tree_view_column_new_with_attributes("Population",
        renduCellule,
        "text", COLONNE_POPULATION,
        NULL);

    // Ajout de la colonne à la vue
    gtk_tree_view_append_column(GTK_TREE_VIEW(g->vueListe), g->colonnesEspeces);
    */

    // Ajout de la vue a la fenetre
    barreDefilement = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(barreDefilement),
        GTK_POLICY_AUTOMATIC,
        GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(barreDefilement), g->vueListe);
    gtk_box_pack_start(GTK_BOX(box), barreDefilement, TRUE, TRUE, 0);

}



void actualiseListeEspeceGTK(GtkListStore *listeEspeces, Interface* g)
{
    Espece* p=g->s.premiereEspece;

    gtk_list_store_clear(listeEspeces);

    while(p!=NULL)
    {
        GtkTreeIter ligne;
        gtk_list_store_append(g->listeEspeces, &ligne);
        gtk_list_store_set(g->listeEspeces, &ligne,
            COLONNE_ADRESSE, p,
            COLONNE_NOM, p->nom,
            COLONNE_POPULATION, p->population,
            -1);
            p=p->suivante;
    }
}



void actualiseNomEspeceGTK(GtkTreeSelection *selection, Interface* g)
{
    GtkTreeIter ligne;
    GtkTreeModel* model = GTK_TREE_MODEL(g->listeEspeces);
    gtk_tree_selection_get_selected(g->selection, &model, &ligne);
    gtk_list_store_set(g->listeEspeces, &ligne, COLONNE_NOM, g->especeSelect->nom, -1);
}




void ajoutEspeceGTK(GtkWidget *boutonAjout, Interface* g)
{
    Espece* p = NULL;
    GtkTreeIter ligne;
    printf("\nAjout d'une espece...");

    p = ajoutEspece(&(g->s));
    //ajoutIndividu
    gtk_list_store_append(g->listeEspeces, &ligne);
    gtk_list_store_set(g->listeEspeces, &ligne,
        COLONNE_ADRESSE, p,
        COLONNE_NOM, p->nom,
        COLONNE_POPULATION, p->population,
        -1);

    printf("\nOK\n");
}



void supprimerEspeceGTK(GtkWidget *boutonSuppr, Interface* g) //@fixme : désactiver l'affichage des carractéristiques quand il n'y a plus d'espèces
{
    GtkTreeIter ligne;
    GtkTreeModel* model = GTK_TREE_MODEL(g->listeEspeces);

    printf("\nSuppression d'une espece...");

    if (gtk_tree_selection_get_selected(g->selection, &model, &ligne))
    {
        supprimerEspece(&(g->s), g->especeSelect);
        gtk_list_store_remove(g->listeEspeces, &ligne);
        printf("\nOK\n");
    }
    else {
        printf("\nRien a supprimer\n");
    }

}

void suppressionIntegraleEspeceGTK(GtkWidget *boutonNouveau, Interface* g)
{
    while(g->s.premiereEspece != NULL)
    {
        supprimerEspece(&(g->s), g->s.premiereEspece);
    }
    gtk_list_store_clear(g->listeEspeces);

    strcpy(g->s.nomSimulation,"nouvelle simulation");
}


void actualiseCaractEspece(GtkTreeSelection *selection, Interface *g)
{

    GtkTreeIter iter;
    GtkTreeModel* model = GTK_TREE_MODEL(g->listeEspeces);
    g->selection = selection;

    if(g->frame!=NULL)
    {
        gtk_widget_destroy(g->frame);
    }

    if (gtk_tree_selection_get_selected(selection, &model, &iter))
    {
        GtkWidget *defil;
        GtkWidget *grid;
        GtkWidget *scale;
        GtkWidget *boutonCouleur;
        int ligne = 0; // on incrémente pour passer d'une ligne à une autre dans la GtkGrid


        g->frame = gtk_frame_new ("Caracteristique de l'espece");
        gtk_frame_set_shadow_type (GTK_FRAME (g->frame), GTK_SHADOW_IN);
        gtk_paned_pack2 (GTK_PANED (g->paned), g->frame, TRUE, FALSE);



        gtk_tree_model_get (model, &iter, COLONNE_ADRESSE, &(g->especeSelect), -1);
        g_print("\nCaracteristique de l'espece %s\n", g->especeSelect->nom);


        grid = gtk_grid_new();


        //@note: vous preferez afficher les tooltips sur le label de gauche ou sur le widget de droite ?
        gtk_grid_attach (GTK_GRID(grid), gtk_label_new("Nom"), 0, ligne, 1, 1);
        g->entryNom = gtk_entry_new();
        gtk_grid_attach (GTK_GRID(grid), g->entryNom, 1, ligne, 1, 1);
        gtk_entry_set_text(GTK_ENTRY(g->entryNom), g->especeSelect->nom);
        g_signal_connect(G_OBJECT(g->entryNom), "activate", // quand on appuie sur la touche Entrée
                         G_CALLBACK(changerValeurNom), g);



        ligne++;
        gtk_grid_attach (GTK_GRID(grid), gtk_label_new("Type"), 0, ligne, 1, 1);
        g->listeType = gtk_combo_box_text_new ();
        gtk_widget_set_tooltip_markup(g->listeType, "Le <b>regime alimentaire</b> de l'espece.");
        gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(g->listeType), "v", "Vegetal");
        gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(g->listeType), "h", "Herbivore");
        gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(g->listeType), "c", "Carnivore");
        gtk_grid_attach (GTK_GRID(grid), g->listeType, 1, ligne, 1, 1);
        gtk_widget_set_sensitive(g->listeType, true);
        switch (g->especeSelect->type)
        {
            case VEGETAL:
                gtk_combo_box_set_active_id(GTK_COMBO_BOX(g->listeType), "v");
                break;
            case HERBIVORE:
                gtk_combo_box_set_active_id(GTK_COMBO_BOX(g->listeType), "h");
                break;
            case CARNIVORE:
                gtk_combo_box_set_active_id(GTK_COMBO_BOX(g->listeType), "c");
                break;
            default:
                break;
        }
        g_signal_connect(G_OBJECT(g->listeType), "changed", G_CALLBACK(changerValeurType), g);



        ligne++;
        gtk_grid_attach (GTK_GRID(grid), gtk_label_new("Couleur"), 0, ligne, 1, 1);
        boutonCouleur = gtk_color_button_new_with_rgba(&(g->especeSelect->couleur));
        g_signal_connect(G_OBJECT(boutonCouleur), "color-set", G_CALLBACK(changerValeurCouleur), g);
        gtk_grid_attach (GTK_GRID(grid), boutonCouleur, 1, ligne, 1, 1);



        creerScale(&(scale), grid, "Population", 0, 100, 1.0, &ligne, "La population presente au debut de la simulation." );
        gtk_range_set_value(GTK_RANGE(scale), g->especeSelect->populationInitiale);
        g_signal_connect(G_OBJECT(scale), "value-changed", G_CALLBACK(changerValeurScaleInt), &(g->especeSelect->populationInitiale));



        creerScale(&(g->scaleVitesse), grid, "Vitesse", 0, 50, 0.1, &ligne, "La <b>vitesse de deplacement</b> moyenne d'un individu de l'espece." );
        gtk_widget_set_hexpand(g->scaleVitesse, TRUE); // toute la colonne s'étendra
        if(g->especeSelect->type==VEGETAL) {
            gtk_widget_set_sensitive(g->scaleVitesse, false);
        }
        else {
            gtk_widget_set_sensitive(g->scaleVitesse, true);
        }
        gtk_range_set_value(GTK_RANGE(g->scaleVitesse), g->especeSelect->vitesseMoyenne);
        g_signal_connect(G_OBJECT(g->scaleVitesse), "value-changed", G_CALLBACK(changerValeurScaleFloat), &(g->especeSelect->vitesseMoyenne));
        // les signaux se mettent après avoir mis la bonne valeur au widget (set_value),
        // sinon la fonction callback s'exécute tout de suite au lieu d'attendre l'utilisateur


        creerScale(&(scale), grid, "Vie", 0, 500, 1.0, &ligne, "L'<b>esperence de vie</b> moyenne d'un individu de l'espece." );
        gtk_range_set_value(GTK_RANGE(scale), g->especeSelect->dureeVieMoyenne);
        g_signal_connect(G_OBJECT(scale), "value-changed", G_CALLBACK(changerValeurScaleInt), &(g->especeSelect->dureeVieMoyenne));


        creerScale(&(scale), grid, "Maturite", 0, 500, 1.0, &ligne, "L'age apres lequel un individu pourra se reproduite." );
        gtk_range_set_value(GTK_RANGE(scale), g->especeSelect->ageMaturite);
        g_signal_connect(G_OBJECT(scale), "value-changed", G_CALLBACK(changerValeurScaleInt), &(g->especeSelect->ageMaturite));


        creerScale(&(scale), grid, "Delai reproduction", 0, 500, 1.0, &ligne, "Temps maximum qu'il faut à un individu entre deux reproductions." );
        gtk_range_set_value(GTK_RANGE(scale), g->especeSelect->delaiProchaineReproduction);
        g_signal_connect(G_OBJECT(scale), "value-changed", G_CALLBACK(changerValeurScaleInt), &(g->especeSelect->delaiProchaineReproduction));



        defil = gtk_scrolled_window_new(NULL, NULL);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(defil),
            GTK_POLICY_AUTOMATIC,
            GTK_POLICY_AUTOMATIC);
        gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(defil), grid);
        gtk_container_add(GTK_CONTAINER(g->frame), defil);
        gtk_widget_show_all(g->frame);
    }

}


void creerScale (GtkWidget** scale, GtkWidget* grid, char* nom, int min, int max, double pas, int* ligne, char* tooltip)
{
        (*ligne)++;
        gtk_grid_attach (GTK_GRID(grid), gtk_label_new(nom), 0, *ligne, 1, 1);
        *scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, min, max, pas);
        gtk_widget_set_tooltip_markup(*scale, tooltip);
        gtk_grid_attach (GTK_GRID(grid), *scale, 1, *ligne, 1, 1);
}




void changerValeurNom(GtkWidget* entry, Interface *g)
{
    Individu* ind = g->especeSelect->premierIndividu;

    changerValeurEntry(entry, g->especeSelect->nom);
    actualiseNomEspeceGTK(g->selection, g);

    while(ind == NULL)
    {
        actualiserNomIndividu (&(g->s), ind, g->especeSelect->nom);
        ind = ind->suivant;
    }
}

void changerValeurEntry(GtkWidget* entry, char* mot)
{
    strcpy( mot , gtk_entry_get_text(GTK_ENTRY(entry)) );
}



void changerValeurType(GtkWidget* comboBox, Interface *g)
{
    char* ch = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(comboBox));
    if(!strcmp("Vegetal", ch))
    {
        g->especeSelect->type = VEGETAL;
    }
    else if(!strcmp("Herbivore", ch))
    {
        g->especeSelect->type = HERBIVORE;
    }
    else if(!strcmp("Carnivore", ch))
    {
        g->especeSelect->type = CARNIVORE;
    }

    if(g->especeSelect->type==VEGETAL)
    {
        gtk_widget_set_sensitive(g->scaleVitesse, false);
        g->especeSelect->vitesseMoyenne = 0;
        gtk_range_set_value(GTK_RANGE(g->scaleVitesse), g->especeSelect->vitesseMoyenne);
    }
    else
    {
        gtk_widget_set_sensitive(g->scaleVitesse, true);
    }
}



void changerValeurCouleur(GtkWidget* boutonCouleur, Interface* g)
{
    gtk_color_button_get_rgba (GTK_COLOR_BUTTON(boutonCouleur), &(g->especeSelect->couleur));

    if(g->s.sdlInitialisee==1)
        creationSurfaceIndividu(&g->s,g->especeSelect,g->s.zoom);
}



void changerValeurScaleInt(GtkWidget* scale, int* valeurAChanger)
{
    *valeurAChanger = gtk_range_get_value(GTK_RANGE(scale));
}



void changerValeurScaleFloat(GtkWidget* scale, float* valeurAChanger)
{
    *valeurAChanger = gtk_range_get_value(GTK_RANGE(scale));
}







void afficheFenetreOuvrir(GtkWidget* ouvrir, Interface* g)
{
    GtkWidget *dialog;

    dialog = gtk_file_chooser_dialog_new ("Ouvrir",
                                          GTK_WINDOW(g->window),
                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);

    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), "./save");

    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;

        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
        chargerSimulation(&(g->s), filename);
        actualiseListeEspeceGTK(g->listeEspeces, g);
        g_free (filename);
    }

    gtk_widget_destroy (dialog);
}


void afficheFenetreSauver(GtkWidget* sauver, Interface* g)
{
    GtkWidget *dialog;

    dialog = gtk_file_chooser_dialog_new ("Sauvegarder",
                                          GTK_WINDOW(g->window),
                                          GTK_FILE_CHOOSER_ACTION_SAVE,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                          NULL);

    gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);

    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), "./save");

    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        char saveName[3*TAILLE_MAX];

        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
        if ( strlen(filename) > (3*TAILLE_MAX)-10 )
        {
            strncpy(saveName,filename,(3*TAILLE_MAX)-10);
            strcpy(filename,saveName);
        }


        sauvegardeSimulation(&(g->s), filename);
        g_free (filename);
    }

    gtk_widget_destroy (dialog);
}





void RAZSimulationGTK(GtkWidget* bouton, Interface* g)
{
    gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(g->barreOutils.simuler), false);
    //envoie un signal qui appelle actualiseBoutonSimulation() qui appelle quitSDL()

    RAZSimulation(&(g->s));
}





void quitGTK(GtkWidget* window, Interface* g)
{
    g->quitter = true;
    //gtk_main_quit();
}
