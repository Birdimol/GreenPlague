#include "map.hpp"

using namespace std;

Map::Map(int largeur_envoyee, int hauteur_envoyee)
{
    largeur = largeur_envoyee;
    hauteur = hauteur_envoyee;
    int nbr_cases = largeur*hauteur;

    idalle.LoadFromFile("pictures/dalle.png");
    imur.LoadFromFile("pictures/mur.png");

    for(int i=0;i<nbr_cases;i++)
    {
        sf::Sprite* temp = new sf::Sprite();
        sdalle.push_back(*temp);
        if(i%largeur == 3 && i > 20 && i < 560)
        {
            sdalle[i].SetImage(imur);
            tableau_cases.push_back(1);
        }
        else
        {
            sdalle[i].SetImage(idalle);
            tableau_cases.push_back(0);
        }
    }
}

bool Map::pointdansmur(float x, float y)
{
    float ligne_bloc = ((int)(y/20));
    float rangee_bloc = ((int)(x/20));
    return tableau_cases[(ligne_bloc*largeur)+rangee_bloc];
}

int Map::getL()
{
    return largeur;
}

int Map::getH()
{
    return hauteur;
}

bool Map::traverse_mur(float x1,float y1,float x2,float y2)
{
    if(x2 < x1)
    {
        float xtemp = x1;
        float ytemp = y1;

        x1 = x2;
        y1 = y2;

        x2 = xtemp;
        y2 = ytemp;
    }

    float x,y;

    for(int i=0;i<hauteur;i++)
    {
        y = i * 20;
        for(int j=0;j<largeur;j++)
        {
            x = j * 20;
            //si c'est un mur
            if(tableau_cases[(i*largeur)+j] == 1)
            {
                //il y a possibilité de chevauchement
                if(x1 <= x+20 && x2 >= x)
                {
                    float x_min=x;
                    float x_max=x+20;

                    if(x1 > x)
                    {
                        x_min = x1;
                    }

                    if(x2 < x+20)
                    {
                        x_max = x2;
                    }

                    //il faut maintenant savoir la valeur de la droite en x_min et x_max
                    float y_min = y1+((y1 - y2)/(x1 - x2))*(x_min - x1);
                    float y_max = y1+((y1 - y2)/(x1 - x2))*(x_max - x1);

                    if((y_min > y+20 && y_max > y+20) || (y_min < y && y_max < y))
                    {

                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

int Map::collision(float x_cercle,float y_cercle,int *x_bloc,int *y_bloc)
{
    float x,y;
    for(int i=0;i<hauteur;i++)
    {
        y = i * 20;
        for(int j=0;j<largeur;j++)
        {
            x = j * 20;
            //si c'est un mur
            if(tableau_cases[(i*largeur)+j] == 1)
            {
                if(x_cercle + 20 <= x || x_cercle - 40 >= x || y_cercle + 20 <= y || y_cercle - 40 >= y)
                {
                    //collision avec le bord
                    if(x_cercle + 20 > 800 || x_cercle - 20 < 0 || y_cercle + 20 > 600 || y_cercle - 20 < 0)
                    {
                        return 1;
                    }

                }
                else
                {
                    //collision
                    *x_bloc = j;
                    *y_bloc = i;
                    return 1;
                }
            }
        }
    }
    return 0;
}

void Map::afficher(sf::RenderWindow* App)
{
    int x=0;
    int y=0;

    for(int i=0;i<hauteur;i++)
    {
        y = i * 20;
        for(int j=0;j<largeur;j++)
        {
            x = j * 20;
            sdalle[(i*largeur)+j].SetPosition(x,y);
            App->Draw(sdalle[(i*largeur)+j]);
        }
    }
}
