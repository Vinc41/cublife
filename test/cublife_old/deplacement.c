

#include <SDL/SDL.h>
#include <math.h>
#include "deplacement.h"

void orienter(CubType* type)
{
    switch (rand()%type->probChangementDirection)
    {
        case 0:
            transfoAngle( 0.1, &(type->courant->direction));
            break;
        case 1:
            transfoAngle( -0.1, &(type->courant->direction));
            break;
        default:
            break;
    }
}





void avancer(CubType* type)
{
    Cub* cube = type->courant;
    SDL_Surface *ecran = SDL_GetVideoSurface();
    float x =  cube->pos.x;
    float y =  cube->pos.y;
    x += type->vitesse * cube->direction.x;
    y += type->vitesse * cube->direction.y;


    // Collision
    if (type->ricoche == 0) // Reste collé aux parois
    {
        if (x<0) {
            x=0;
        }
        else if (x >= (ecran->w - type->surface->w)) {
            x = ecran->w - type->surface->w;
        }

        if (y<0) {
            y=0;
        }
        else if (y >= (ecran->h - type->surface->h)) {
            y = ecran->h - type->surface->h;
        }
    }
    else // Richoche sur les parois
    {
        if  ( (x<5.0 && cube->direction.x<0) ||  (x>=ecran->w-GRANDEUR_CUBE && cube->direction.x>0) )
            cube->direction.x *= -1;

        else if ( (y<5.0 && cube->direction.y<0) ||  (y>=ecran->h-GRANDEUR_CUBE && cube->direction.y>0) )
            cube->direction.y *= -1;
    }
    cube->pos.x = x;
    cube->pos.y = y;
}




void transfoAngle(Angle theta, Vec2 *vec)
{
    float xtr = vec->x*cos(theta) + vec->y*sin(theta);
    float ytr = -vec->x*sin(theta) + vec->y*cos(theta);
    vec->x = xtr;
    vec->y = ytr;
}

