#include "simulation.h"


void initSimulation (Simulation* sim)
{
    //initialisation de la simulation
    sim->sdlInitialisee = false;
    sim->simulationLancee = false;
    sim->map.largeur_map = NB_CASE_LARGEUR_DEFAUT;
    sim->map.hauteur_map = NB_CASE_HAUTEUR_DEFAUT;
    sim->genererNouvelleMap=1;
    sim->genererPosEspeces=1;
    sim->map.mapInitialisee=0;
    sim->nomSimulation=(char*)malloc(TAILLE_MAX*sizeof(char));
    strcpy(sim->nomSimulation,"nouvelle simulation");

    //initialisation de la generation aleatoire
    srand(time(NULL));

    //initialisation de SDL_framerate qui gere le fps
    SDL_Init(SDL_INIT_TIMER         // pour SDL_framerate
            |SDL_INIT_NOPARACHUTE); // pour mieux débugger, à enlever si besoin
    SDL_initFramerate(&(sim->fps));
    SDL_setFramerate((&sim->fps), 30);

    //évite le problème d'affichage de la première ligne // @fixme: aucun printf ne marche sur windows, les g_print non plus (à vérifier)
    printf("\n");
}



void initListeEspece(Simulation* sim)
{

    Espece* p = NULL;

    // initialisation de la liste
    sim->premiereEspece = NULL;
    sim->nombreEspeces = 0;

    // définitions provisoire des espèces présentes au départ pour les tests
    // à rajouter dans une sauvegarde
    p = ajoutEspece(sim);
    strcpy(p->nom, "Vache");
    p->type = HERBIVORE;
    p->vitesseMoyenne=Random(1.,3);
    p->ageMaturite = 20;
    p->delaiProchaineReproduction = 60;
    p->dureeVieMoyenne = 90;
    p->populationInitiale=3;
    /*for(i=0; i<6; i++)
    {
        ajoutIndividu(&(p->premierIndividu),i%2,p, sim->tempsPasse);
    }
*/
    p = ajoutEspece(sim);
    strcpy(p->nom, "Lion");
    p->type = CARNIVORE;
    p->vitesseMoyenne=Random(2,3);
    p->ageMaturite = 20;
    p->delaiProchaineReproduction = 35;
    p->dureeVieMoyenne = 80;
    p->populationInitiale=3;
    p->couleur.red = 1.0;
    p->couleur.green = 0.1;
    p->couleur.blue = 0.1;
    /*for(i=0; i<3; i++)
    {
        ajoutIndividu(&(p->premierIndividu),i%2,p, sim->tempsPasse);

    }*/

    p = ajoutEspece(sim);
    strcpy(p->nom, "Pissenlit");
    p->type = VEGETAL;
    p->ageMaturite = 30;
    p->delaiProchaineReproduction = 30;
    p->dureeVieMoyenne = 50;
    p->populationInitiale=3;
    p->couleur.red = 0.6;
    p->couleur.green = 0.8;
    p->couleur.blue = 0.0;
    /*for(i=0; i<3; i++)
    {
        ajoutIndividu(&(p->premierIndividu),BOTH,p, sim->tempsPasse);
    }
    */


}



Espece* ajoutEspece(Simulation* sim)
{
    Espece* nouvelle = (Espece*)malloc(sizeof(Espece));
    int i=0;
    int sexe;

    // insertion dans la chaine
    if(sim->premiereEspece == NULL)
    {
        sim->premiereEspece = nouvelle;
    }
    else
    {
        Espece* p=sim->premiereEspece;

        while (p->suivante!=NULL)
            p=p->suivante;

        p->suivante = nouvelle;
    }

    sim->nombreEspeces++;
    nouvelle->suivante = NULL;
    nouvelle->population = 0;
    nouvelle->premierIndividu=initIndividus();

    //valeurs par défaut
    strcpy(nouvelle->nom, "Nouvelle");
    nouvelle->milieu = TERRESTRE;
    nouvelle->type = HERBIVORE;

    nouvelle->couleur.red = 0.3;
    nouvelle->couleur.green = 0.1;
    nouvelle->couleur.blue = 1.0;
    nouvelle->couleur.alpha = 1.0; // opaque

    nouvelle->populationInitiale = 3;
    nouvelle->vitesseMoyenne = 0;
    nouvelle->acceleration = 0.1;
    nouvelle->ageMaturite = 15;
    nouvelle->delaiProchaineReproduction = 10;
    nouvelle->dureeVieMoyenne = 40;
    nouvelle->vieEcartType = 10;
    for(i=0;i<nouvelle->populationInitiale;i++)
    {
        sexe=Random(0,100);
        ajoutIndividu(&(nouvelle->premierIndividu),sexe%2,nouvelle,sim->tempsPasse);
    }
    // à compléter en fonction de nos besoins

    return nouvelle;
}



void RAZSimulation (Simulation* sim)
{
    int i;
    Espece* espece = sim->premiereEspece;
    Sexe s;


    //on regénère la positionn des individus
    sim->genererPosEspeces=true;

    //Raz du temps
    sim->tempsPasse = 0;

    //Raz de la population
    while (espece != NULL)
    {
        //Suppression des individus deja existant
        supprimerIndividus(espece);

        //Ajout des nouveaux individus
        for(i=0;i<espece->populationInitiale;i++)
        {
            //choix du sexe aleatoire
            if(espece->type ==  VEGETAL)
            {
                s=BOTH;
            }
            else
            {
                s=Random(0,1);
            }

            ajoutIndividu(&(espece->premierIndividu),s,espece, sim->tempsPasse);
        }

        espece=espece->suivante;
    }
}



void actualiserIndividus(Simulation* sim)
{

    Espece *espece = sim->premiereEspece;
    Individu *ind;
    Individu *indSuiv;
    Individu *proche=NULL;
    bool regulationVitesseActive=false;
    bool mangee=false;

    int dep=0;

    while(espece!=NULL)
    {
        ind=espece->premierIndividu;
        while(ind!=NULL)
        {


                indSuiv=ind->suivant; // sauvegarde l'individu suivant au cas où l'individu courant est désalloué par sa mort
                proche=NULL;
                regulationVitesseActive=false;
                dep=0;
                mangee=false;

                ///DEPLACEMENT
                dep=deplacement(ind,sim);
                if(dep)
                {
                    deplacementAleatoire(ind);
                }

                //on deplace l'individu suivant l'angle donné précédement.
                ind->position.x += ind->vitesse * cos((ind->direction)*M_PI/180.0) * sim->coeffTemps;
                ind->position.y -= ind->vitesse * sin((ind->direction)*M_PI/180.0) * sim->coeffTemps;


                ///ETAT EAU ET NOURRITURE ET FATIGUE
                if((ind->espece->type!=VEGETAL)&&((ind->stockEau>0)||(ind->stockNourriture>0)))
                {
                    if(ind->stockEau>0.1)
                    {
                        ind->stockEau-=0.1*sim->coeffTemps;
                        if(ind->stockEau<0)
                        {
                            ind->stockEau=0;
                        }
                    }
                    if(ind->stockNourriture>0.05)
                    {
                        ind->stockNourriture-=0.05*sim->coeffTemps;
                        if(ind->stockNourriture<0)
                        {
                            ind->stockNourriture=0;
                        }
                    }



                }
                if(ind->repos==0)
                {
                    ind->fatigue+=0.1*sim->coeffTemps;
                    if(ind->fatigue>100)
                        {
                            ind->fatigue=100;
                        }
                }

                if((ind->fatigue<10)&&(ind->repos==1))
                {
                    ind->repos=0;
                }
                if((ind->fatigue>90)&&(ind->repos==0))
                {
                    if((interactionIndividuMap(sim,ind)==TERRE)&&(Random(0,100)>95))
                    {
                        ind->vitesse=0;

                        ind->repos=1;
                    }
                }
                if(ind->repos)
                {
                    regulationVitesseActive=1;
                    ind->vitesse=0;
                    ind->fatigue-=0.5*sim->coeffTemps;
                    if(ind->fatigue<0)
                        {
                            ind->fatigue=0;
                        }
                }

                if((ind->stockEau<10)||(ind->stockNourriture<10))
                {
                    ind->vitesse=ind->espece->vitesseMoyenne*0.6;
                    regulationVitesseActive=true;
                }
                //stock eau
                if(interactionIndividuMap(sim,ind)==EAU)
                {
                    ind->stockEau=100;
                    ind->vitesse=ind->espece->vitesseMoyenne*0.7;
                    regulationVitesseActive=true;
                }

                if((ind->stockEau<1)||(ind->stockNourriture<1))
                {
                    ind->sante-=0.5*sim->coeffTemps;
                    if(ind->sante<0)
                        {
                            ind->sante=0;
                        }
                }
                else if(ind->sante<100)
                {
                    ind->sante+=0.5*sim->coeffTemps;
                    if(ind->sante>100)
                        {
                            ind->sante=100;
                        }
                }
                if(regulationVitesseActive==0)
                {
                    regulerVitesse(ind);
                }

                //boucle de verif

                if(ind->espece->type==VEGETAL)//reproduction plante
                {
                    if((ind->derniereReproduction+ind->espece->delaiProchaineReproduction)<(sim->tempsPasse))
                    {
                        if(Random(0,100)>75)
                        {

                           reproduction(ind,NULL,sim);

                        }
                    }
                }
                proche=quiEstProche(ind,sim);
                if(proche!=NULL)
                {
                    if(proche->espece->type==ind->espece->type)
                    {
                        if(proche->espece==ind->espece)
                        {
                            if(proche->sexe!=ind->sexe)
                            {
                                if((ind->derniereReproduction+ind->espece->delaiProchaineReproduction)<(sim->tempsPasse))
                                {
                                    reproduction(ind,proche,sim);
                                }
                            }

                        }

                    }
                    else
                    {
                       mangee=manger(ind,proche);
                    }



                }
                if(mangee==false)
                {
                    if(((ind->espece->dureeVieMoyenne+ind->naissance)<sim->tempsPasse)||(ind->sante==0))//durée de vie de l'individu
                    {
                        if(Random(0,100)>90)
                        {

                            mortIndividu(ind);

                        }
                    }

                }

        ind=indSuiv;
        }

        espece=espece->suivante;
    }


}




bool manger(Individu* ind,Individu* ind2)
{
    if(ind->espece->type==VEGETAL&&ind2->espece->type==HERBIVORE)
    {
        mortIndividu(ind);
        ind2->stockNourriture=100;
        return 1;
    }
    else if((ind->espece->type==CARNIVORE&&ind2->espece->type==HERBIVORE)
            ||(ind->espece->type==CARNIVORE&&ind2->espece->type==HOMNIVORE))
    {
        ind->stockNourriture=100;
        mortIndividu(ind2);
        return 0;
    }


    return 0;


}


int deplacement(Individu* ind, Simulation *sim)
{

    int depart=ind->direction;
    ///TEST REGULATION VITESSE
    if((ind->position.x<=12)
       ||(ind->position.y<=12)
       ||(ind->position.x>=(LONGUEUR_CASE*sim->map.largeur_map)-12)
       ||(ind->position.y>=(LONGUEUR_CASE*sim->map.hauteur_map)-12))

    {
        ind->vitesse=ind->espece->vitesseMoyenne*0.7;
    }
    if((ind->position.x<=6)
       ||(ind->position.y<=6)
       ||(ind->position.x>=(LONGUEUR_CASE*sim->map.largeur_map)-6)
       ||(ind->position.y>=(LONGUEUR_CASE*sim->map.hauteur_map)-6))
    {
        ind->vitesse=ind->espece->vitesseMoyenne*0.5;
    }

    //on verifie qu'on ne rencontre pas le mur


    if(ind->position.x<=3)//le mur gauche
    {

        if(ind->direction>180)
        {
            ind->direction=ind->direction+90;
        }
        if(ind->direction<180)
        {
            ind->direction=ind->direction+270;
        }
        if(ind->direction==180)
        {
            ind->direction=ind->direction+180;
        }

        if(ind->direction>360)
        {
            ind->direction=ind->direction-360;
        }
        ind->position.x=ind->position.x+3;
    }
    if(ind->position.y<=3)//le mur du haut
    {
        if(ind->direction>90)
        {
            ind->direction=ind->direction+90;
        }
        if(ind->direction<90)
        {
            ind->direction=ind->direction+270;
        }
        if(ind->direction==90)
        {
            ind->direction=ind->direction+180;
        }

        if(ind->direction>360)
        {
            ind->direction=ind->direction-360;
        }
        ind->position.y=ind->position.y+3;
    }
    if(ind->position.x>=(LONGUEUR_CASE*sim->map.largeur_map)-3)//mur droit
    {
        if(ind->direction<90)
        {
            ind->direction=ind->direction+90;
        }
        if(ind->direction>270)
        {
            ind->direction=ind->direction+270;
        }
        if(ind->direction==0||ind->direction==360)
        {
            ind->direction=ind->direction+180;
        }

        if(ind->direction>360)
        {
            ind->direction=ind->direction-360;
        }
        ind->position.x=ind->position.x-3;
    }
    if(ind->position.y>=(LONGUEUR_CASE*sim->map.hauteur_map)-3)//mur du bas
    {
        if(ind->direction>270)
        {
            ind->direction=ind->direction+90;
        }

        if(ind->direction<270)
        {
            ind->direction=ind->direction+270;
        }

        if(ind->direction==270)
        {
            ind->direction=ind->direction+180;
        }


        if(ind->direction>360)
        {
            ind->direction=ind->direction-360;
        }
        ind->position.y=ind->position.y-3;
    }

if((depart==ind->direction)||(ind->vitesse=ind->espece->vitesseMoyenne))
{
    return 1;
}
else
{
    return 0;
}

}

void reproduction(Individu* ind,Individu* proche,Simulation *sim)
{
    Individu *enfant;
    int i=Random(0,100);
    i=i%2;
    int debug=1;

    if((ind->espece->type==VEGETAL))
    {
        ajoutIndividu(&(ind->espece->premierIndividu),BOTH,ind->espece, sim->tempsPasse);
        enfant=ind->espece->premierIndividu;
        do
        {
            enfant->position.x=debug*12*(Random(-1,1))+ind->position.x;
            enfant->position.y=debug*12*(Random(-1,1))+ind->position.y;
            debug++;
        }while(interactionIndividuMap(sim,enfant)==EAU);

    }
    else
    {
        ajoutIndividu(&(ind->espece->premierIndividu),i,ind->espece, sim->tempsPasse);
        enfant=ind->espece->premierIndividu;
        enfant->position.x=12*(Random(-1,1))+ind->position.x;
        enfant->position.y=12*(Random(-1,1))+ind->position.y;
        proche->derniereReproduction=sim->tempsPasse;
    }
    ind->derniereReproduction=sim->tempsPasse;

    ajoutSDLIndividu(sim, enfant, enfant->espece->nom, 0);
}


Individu* quiEstProche(Individu* ind, Simulation* sim)
{
    Espece* p1=sim->premiereEspece;
    Individu* p2=p1->premierIndividu;
    while(p1!=NULL)
    {
        while(p2!=NULL)
        {
            if((rayonAction(ind->position,p2->position))&&(p2!=ind))
            {
                return p2;
            }
            p2=p2->suivant;
        }
        p1=p1->suivante;
    }
    return NULL;
}




void supprimerEspece (Simulation* sim, Espece *espece)
{
    Espece* p=sim->premiereEspece;
    supprimerIndividus(p);

    if(espece==p)
    {
        sim->premiereEspece=espece->suivante;
    }
    else
    {
        while(p->suivante!=espece)
        {
            p=p->suivante;
        }
        p->suivante=espece->suivante;
    }
    sim->nombreEspeces--;
    free(espece);
}



int sauvegardeEspece (Espece *listeEspece, FILE* saveFile)
{
    Espece *espece = listeEspece;
    int nbEspece=0;


    if(saveFile != NULL)
    {
        while(espece != NULL)
        {
            nbEspece++;
            espece = espece->suivante;
        }
        fprintf(saveFile, "%d\n", nbEspece);

        espece=listeEspece;
        while(espece != NULL)
        {
            fputs(espece->nom, saveFile);
            fprintf(saveFile, "\n%d\n%d\n%u\n%u\n",espece->population,espece->id,espece->type,espece->milieu);
            fprintf(saveFile, "%d\n%d\n%d\n%d\n",espece->dureeVieMoyenne,espece->vieEcartType,espece->ageMaturite,espece->delaiProchaineReproduction);
            fprintf(saveFile, "%f\n%f\n%f\n",espece->couleur.red,espece->couleur.green,espece->couleur.blue);
            fprintf(saveFile, "%d\n%d\n%d\n",espece->besoinEau,espece->besoinLumiere,espece->besoinNourriture);
            fprintf(saveFile, "%d\n%f\n%f\n",espece->vitesseDeveloppement,espece->vitesseMoyenne,espece->acceleration);

            if(espece->population != 0)
            {
                sauvegardeIndividus(saveFile,espece->premierIndividu);
            }

            espece = espece->suivante;
        }

        return 1;
    }
    else
    {
        return 0;
    }
}


int chargerEspece (Espece** listeEspece, FILE *saveFile)
{
    int nbEspece, i, j,taille=0;
    Espece *espece = *listeEspece;
    char chaine[TAILLE_MAX];
    char** tab;
    char retour_ligne = '\n';
    char caractere_fin = '\0';

    if(saveFile != NULL)
    {
        //initialisation avant la boucle
        fgets(chaine, TAILLE_MAX, saveFile);
        nbEspece=atof(chaine);

        if(nbEspece > 0)
        {
            espece=(Espece*)malloc(sizeof(Espece));
            *listeEspece=espece;
        }

        for(i=0;i<nbEspece;i++)
        {
            tab=initTableauChaineCaractrere();

            for(j=0;j<18;j++)
            {
                fgets(chaine, TAILLE_MAX, saveFile);
                tab=augmenterTailleTabChaineCaractere((j+1),tab,1);

                strcpy(tab[j],chaine);
            }
            j=0;

            while (tab[0][j] != retour_ligne)
            {
                j++;
            }
            strncpy(espece->nom,tab[0],j);
            espece->nom[j]=caractere_fin;

            espece->population=atof(tab[1]);
            espece->populationInitiale=atof(tab[1]);
            espece->id=atof(tab[2]);
            espece->type=atof(tab[3]);
            espece->milieu=atof(tab[4]);
            espece->dureeVieMoyenne=atof(tab[5]);
            espece->vieEcartType=atof(tab[6]);
            espece->ageMaturite=atof(tab[7]);
            espece->delaiProchaineReproduction=atof(tab[8]);
            espece->couleur.red=atof(tab[9]);
            espece->couleur.green=atof(tab[10]);
            espece->couleur.blue=atof(tab[11]);
            espece->besoinEau=atof(tab[12]);
            espece->besoinLumiere=atof(tab[13]);
            espece->besoinNourriture=atof(tab[14]);
            espece->vitesseDeveloppement=atof(tab[15]);
            espece->vitesseMoyenne=atof(tab[16]);
            espece->acceleration=atof(tab[17]);

            //On charge les individu
            if(espece->population != 0)
            {
                chargerIndividus(saveFile,&espece->premierIndividu, espece,espece->population);
            }
            else
            {
                espece->premierIndividu = NULL;
            }

            //On regarde si c'était la dernière espece
            if(i+1 < nbEspece)
            {
                espece->suivante=(Espece*)malloc(sizeof(Espece));
            }
            else
            {
                espece->suivante=NULL;
            }

            supprimerTabChaineCaractere(tab,taille,1);

            espece=espece->suivante;
        }

        return 1;
    }
    else
    {
        return 0;
    }
}

void sauvegardeIndividus (FILE* fichierIndividu, Individu* listeIndividu)
{
    Individu *individu = listeIndividu;

    while(individu != NULL)
    {
        fprintf(fichierIndividu, "%d\n%d\n",individu->id,individu->naissance);
        fprintf(fichierIndividu, "%d\n%d\n", individu->sante,individu->derniereReproduction);
        fprintf(fichierIndividu, "%f\n%f\n%f\n",individu->stockEau,individu->stockNourriture,individu->fatigue);
        fprintf(fichierIndividu, "%u\n",individu->sexe);
        fprintf(fichierIndividu, "%f\n", individu->position.x);
        fprintf(fichierIndividu, "%f\n", individu->position.y);

        individu = individu->suivant;
    }
}


void chargerIndividus (FILE* fichierIndividu, Individu** listeIndividu, Espece* espece, int pop)
{
    Individu *individu = *listeIndividu;
    int i,j,taille=0;
    char** tab;
    char chaine[TAILLE_MAX];

    //initialisation avant la boucle
    individu=(Individu*)malloc(sizeof(Individu));
    *listeIndividu=individu;

    for(i=0;i<pop;i++)
    {
        tab=initTableauChaineCaractrere();

        for(j=0;j<10;j++)
        {
            fgets(chaine, TAILLE_MAX, fichierIndividu);
            tab=augmenterTailleTabChaineCaractere((j+1),tab,1);

            strcpy(tab[j],chaine);
        }

        individu->id=atof(tab[0]);
        individu->naissance=atof(tab[1]);
        individu->sante=atof(tab[2]);
        individu->derniereReproduction=atof(tab[3]);
        individu->stockEau=atof(tab[4]);
        individu->stockNourriture=atof(tab[5]);
        individu->fatigue=atof(tab[6]);
        individu->sexe=atof(tab[7]);
        individu->position.x=atof(tab[8]);
        individu->position.y=atof(tab[9]);

        individu->espece=espece;


        //On regarde si c'était la dernière espece
        if(i+1 < pop)
        {
            individu->suivant=(Individu*)malloc(sizeof(Individu));
        }
        else
        {
            individu->suivant=NULL;
        }

        supprimerTabChaineCaractere(tab,taille,1);

        individu = individu->suivant;
    }
}


int sauvegardeSimulation (Simulation* sim, char* saveName)
{
    FILE* saveFile = NULL;
    int map, espece;
    char nom[(3*TAILLE_MAX)+6];
    char* nomSimulation;
    int i;

    if(sim->map.mapInitialisee == 1)
    {
        for (i= strlen(saveName) ; i>0; i--)
        {
           if (saveName[i] == '.')
            {
                if(saveName[i+1] == 's' && saveName[i+2] == 'c' && saveName[i+3] == 'b' && saveName[i+4] == 'l' && saveName[i+5] == 'f')
                {
                    saveName[i] = '\0';
                }
            }
        }

        sprintf(nom,"%s.scblf",saveName);
        saveFile = fopen(nom, "w+");

        nomSimulation=enleverChemin(nom);
        if(strcmp(nomSimulation,"nouvelle simulation") == 0)
        {
            free(sim->nomSimulation);
        }
        strcpy(sim->nomSimulation,nomSimulation);

        if(saveFile != NULL)
        {
            fprintf(saveFile, "%f\n%d\n%d\n", sim->tempsPasse, sim->annee, sim->mois);
            espece=sauvegardeEspece(sim->premiereEspece,saveFile);
            map=sauvegardeMap(sim->map,saveFile);


            if(map ==0 || espece ==0)
            {
                return 0;
            }

            //On ferme le fichier
            fclose(saveFile);

            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}


int chargerSimulation (Simulation *sim, char* saveName)
{
    FILE* saveFile = NULL;
    int map_charger, espece_charger;
    char chaine [TAILLE_MAX];
    int taille=0, i=0;
    char** tab;
    char* nomSimulation;
    CHARGEMENT* sdl_chargement;

    if(sim->sdlInitialisee == 0)
    {
        nomSimulation=enleverChemin(saveName);
        if(strcmp(nomSimulation,"nouvelle simulation") == 0)
        {
            free(sim->nomSimulation);
        }
        strcpy(sim->nomSimulation,nomSimulation);

        initSDL(sim,true);
        sdl_chargement=initChargement();
        actualiserChargement(sim,sdl_chargement,"chargement...","...des diiférentes espèces et différents individus",10,1);

        //vidage de la memoire atribué à la map, si c'est le cas
        if(sim->map.mapInitialisee == true)
        {
            deleteMap (&(sim->map));
        }

        //vidage de la memoire attribué aux especes et individus
        supprimerEspeceEtIndividu(&(sim->premiereEspece));

        //On ouvre le fichier
        saveFile = fopen(saveName, "r");

        if(saveFile != NULL)
        {
            // initialisation de la liste
            sim->premiereEspece = NULL;
            sim->nombreEspeces = 0;

            sim->genererNouvelleMap = 0;
            sim->genererPosEspeces = 0;

            tab=initTableauChaineCaractrere();

            for(i=0;i<3;i++)
            {
                fgets(chaine, TAILLE_MAX, saveFile);
                tab=augmenterTailleTabChaineCaractere((i+1),tab,1);

                strcpy(tab[i],chaine);
            }

            sim->tempsPasse=atof(tab[0]);
            sim->annee=atof(tab[1]);
            sim->mois=atof(tab[2]);

            supprimerTabChaineCaractere(tab,taille,1);

            espece_charger=chargerEspece(&sim->premiereEspece,saveFile);
            SDL_Delay(100);

            actualiserChargement(sim,sdl_chargement,"chargement...","...du terrain de la simulation",10,1);
            SDL_Delay(100);
            map_charger=chargerMap(sim, &(sim->map), saveFile, sdl_chargement);

            actualiserChargement(sim,sdl_chargement,"chargement...","...incorporation des animaux et végétaux sur le terrain",100,1);
            SDL_Delay(500);

            supprimerChargement(sdl_chargement);

            //On ferme le fichier
            fclose(saveFile);

            quitSDL(sim,true);


            if(map_charger == 0 || espece_charger == 0)
            {
                return 0;
            }

            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}


char* enleverChemin (char* nomAvecChemin)
{
    char *nomSansChemin=(char*)malloc(((3*TAILLE_MAX)+6)*sizeof(char));
    int i=0, j=0;
    int tailleNom=0;
    int positionNom;

    for(i=strlen(nomAvecChemin); i>0; i--)
    {
        if (nomAvecChemin[i] == '\\')
        {
            positionNom=i+1;
            tailleNom=strlen(nomAvecChemin) - positionNom;

            for(j=0;j<tailleNom;j++)
            {
                nomSansChemin[j] = nomAvecChemin[positionNom+j];
            }
            nomSansChemin[j]='\0';

            return nomSansChemin;
        }
    }

    return NULL;
}

void supprimerSauvegarde (char *saveName) //@todo : appeler cette fonction pour supprimer des sauvegarde
{
    remove(saveName);
}
