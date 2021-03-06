#include <fstream>
#include <iostream>
#include <sstream>
#include "map.hpp"
#include "tools.cpp"

using namespace std;

Map::Map(string FileName)
{
    string completeFileName = "maps/" + FileName + ".gpm";
    ifstream fichier(completeFileName.c_str(), ios::in);
    Point temporaire;
    if(fichier)  // si l'ouverture a r�ussi
    {
        error_happened = false;

        string ligneLue;  // d�claration d'une cha�ne qui contiendra la ligne lue
        string variable_temp;  // d�claration d'une cha�ne qui contiendra la ligne lue
        getline(fichier, ligneLue);//lecture ligne 1

        getline(fichier, ligneLue);//lecture ligne 2
        variable_temp = ligneLue.substr(ligneLue.find(":")+1,ligneLue.find(";")-ligneLue.find(":")+1);
        largeur = strToInt(variable_temp);

        getline(fichier, ligneLue);//lecture ligne 3
        variable_temp = ligneLue.substr(ligneLue.find(":")+1,ligneLue.find(";")-ligneLue.find(":")+1);
        hauteur = strToInt(variable_temp);

        taille_case = 40;

        idalle.LoadFromFile("pictures/dalle.png");
        idalle_non_visible.LoadFromFile("pictures/dalle_non_visible.png");
        imur.LoadFromFile("pictures/mur.png");

        cout << largeur << ", " << hauteur << endl;

        //lecture des cases
        for(int i=0;i<hauteur;i++)
        {
            getline(fichier, ligneLue);//lecture ligne de la map
            for(int j=0;j<largeur;j++)
            {
                variable_temp = ligneLue.substr(j,1);
                sf::Sprite* temp = new sf::Sprite();
                sdalle.push_back(*temp);

                //toutes les cases sont invisibles par d�faut.
                tableau_cases_visible.push_back(0);

                if(strToInt(variable_temp) == 1)
                {
                    sdalle[i*largeur+j].SetImage(imur);
                    tableau_cases.push_back(1);
                    temporaire.x = j;
                    temporaire.y = i;
                    liste_mur.push_back(temporaire);
                }
                else if(strToInt(variable_temp)==0)
                {
                    sdalle[i*largeur+j].SetImage(idalle);
                    tableau_cases.push_back(0);
                }
            }
        }

        getline(fichier, ligneLue);//lecture ligne qui separe les maps

        //lecture des monstres
        for(int i=0;i<hauteur;i++)
        {
            getline(fichier, ligneLue);//lecture ligne de la map
            for(int j=0;j<largeur;j++)
            {
                variable_temp = ligneLue.substr(j,1);
                sf::Sprite* temp = new sf::Sprite();
                sdalle.push_back(*temp);

                if(strToInt(variable_temp)==2)
                {
                    Point temp_(j,i);
                    //Ajout du zombie dans la liste des zombies
                    tableau_zombie.push_back(temp_);

                    //ajout de la position du zombi dans la liste des cases occup�es
                    add_liste_case_occupee(temp_);
                }
            }
        }
        fichier.close();  // on ferme le fichier
        cout << "Map chargee !" << endl;
    }
    else  // sinon
    {
        error_happened = true;
        cout << "Impossible d'ouvrir la map : " << completeFileName << endl;
    }

}

bool Map::estMur(Point case_)
{
    if(tableau_cases[case_.x+case_.y*largeur] == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int Map::strToInt(string chaine)
{
    stringstream ss( chaine );
    int temp;
    ss >> temp;
    return temp;
}

vector<Point> Map::get_liste_mur()
{
    return liste_mur;
}

Point Map::getCaseLibreProche(Point point,vector<Point> liste_case_occupee)
{
    //si c'est pas un mur, ni une case occupee
    if(!estMur(point) && !Tools::contient_point(liste_case_occupee,point))
    {
        return point;
    }

    //on parcourt les cases autour de celle envoyee, en etant de plus en plus large
    int iteration_repere = 0;
    bool trouve = false;

    while(!trouve && iteration_repere < 50)
    {
        for(int i=-iteration_repere;i<iteration_repere+1;i++)
        {
            for(int j=-iteration_repere;j<iteration_repere+1;j++)
            {
                if(abs(i) >= iteration_repere || abs(j) >= iteration_repere)
                {
                    //si la case est bien dans les limites de la carte
                    if((point.x+j*iteration_repere) > -1 && (point.x+j) < largeur && (point.y+i) > -1 && (point.y+i) < hauteur)
                    {
                        Point temp((point.x+j),(point.y+i));
                         //si la case proche n'est pas un mur ou n'est pas occupp�e, �a fait l'affaire !

                        if(!estMur(temp) && !Tools::contient_point(liste_case_occupee,temp))
                        {
                            //Il faudrait verifier si cette case vide est accessible !
                            return temp;
                        }
                    }
                }
            }
        }
        iteration_repere++;
    }

    //on renonce a trouver une case vide
    return point;
}

bool Map::getErrorHappened()
{
    return error_happened;
}

vector<int> Map::get_tableau_cases()
{
    return tableau_cases;
}

vector<Point> Map::get_tableau_zombie()
{
    return tableau_zombie;
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
        y = i * taille_case;
        for(int j=0;j<largeur;j++)
        {
            x = j * taille_case;
            //si c'est un mur
            if(tableau_cases[(i*largeur)+j] == 1)
            {
                //il y a possibilit� de chevauchement
                if(x1 <= x+taille_case && x2 >= x)
                {
                    if(x1 == x2)
                    {
                        if(y1 > y2)
                        {
                            float y_max = y1;
                            float y_min = y2;

                            if(y_min <= y+taille_case && y_max >= y)
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            float y_max = y2;
                            float y_min = y1;

                            if(y_min <= y+taille_case && y_max >= y)
                            {
                                return 1;
                            }
                        }
                    }
                    else if(y1 == y2)
                    {
                        if(y1 <= y+taille_case && y1 >= y)
                        {
                            if(x1 > x2)
                            {
                                float x_max = x1;
                                float x_min = x2;

                                if(x_min <= x+taille_case && x_max >= x)
                                {
                                    return 1;
                                }
                            }
                            else
                            {
                                {
                                    float x_max = x2;
                                    float x_min = x1;

                                    if(x_min <= x+taille_case && x_max >= x)
                                    {
                                        return 1;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        float x_min=x;
                        float x_max=x+taille_case;

                        if(x1 > x)
                        {
                            x_min = x1;
                        }

                        if(x2 < x+taille_case)
                        {
                            x_max = x2;
                        }
                        //il faut maintenant savoir la valeur de la droite en x_min et x_max
                        float y_min = y1+((y1 - y2)/(x1 - x2))*(x_min - x1);
                        float y_max = y1+((y1 - y2)/(x1 - x2))*(x_max - x1);

                        if(!((y_min > y+taille_case && y_max > y+taille_case) || (y_min < y && y_max < y)))
                        {
                            return 1;
                        }
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
        y = i * taille_case;
        for(int j=0;j<largeur;j++)
        {
            x = j * taille_case;
            //si c'est un mur
            if(tableau_cases[(i*largeur)+j] == 1)
            {
                if(x_cercle + 18 <= x || x_cercle - 18 >= x+taille_case || y_cercle + 18 <= y || y_cercle - 18 >= y+taille_case)
                {
                    //collision avec le bord
                    if(x_cercle + 20 > 800 || x_cercle - 20 < 0 || y_cercle + 20 > 600 || y_cercle - 20 < 0)
                    {
                        //cout << x_cercle << ", " << y_cercle <<endl;
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

int Map::getTailleCase()
{
    return taille_case;
}

void Map::add_liste_case_occupee(Point a)
{
    liste_case_occupee.push_back(a);
}

void Map::delete_liste_case_occupee(Point a)
{
    bool erased = false;
    do
    {
        int size = liste_case_occupee.size();
        erased = false;
        for(int i=0;i<size;i++)
        {
            if(liste_case_occupee[i].x == a.x && liste_case_occupee[i].y == a.y)
            {
                liste_case_occupee.erase(liste_case_occupee.begin()+i);
                erased = true;
            }
        }
    }while(erased);
}

vector<Point> Map::get_liste_case_occupee()
{
    return liste_case_occupee;
}

void Map::afficher(sf::RenderWindow* App, sf::Rect<float> rect_view)
{
    int x=0;
    int y=0;

    int case_haut = floor(rect_view.Top/taille_case);
    int case_bas  = floor(rect_view.Bottom/taille_case);
    int case_gauche  = floor(rect_view.Left/taille_case);
    int case_droite  = floor(rect_view.Right/taille_case)+1;

    for(int i=case_haut;i<case_bas-1;i++)
    {
        y = i * taille_case;
        for(int j=case_gauche;j<case_droite;j++)
        {
            x = j * taille_case;
            if(tableau_cases[(i*largeur)+j] == 1)
            {
                sdalle[(i*largeur)+j].SetPosition(x,y);
                App->Draw(sdalle[(i*largeur)+j]);
            }
            else if(tableau_cases_visible[(i*largeur)+j] == 1)
            {
                sdalle[(i*largeur)+j].SetImage(idalle);
                sdalle[(i*largeur)+j].SetPosition(x,y);
                App->Draw(sdalle[(i*largeur)+j]);
            }
            else
            {
                sdalle[(i*largeur)+j].SetImage(idalle_non_visible);
                sdalle[(i*largeur)+j].SetPosition(x,y);
                App->Draw(sdalle[(i*largeur)+j]);
            }
        }
    }
}

void Map::set_visible(Point a)
{
    if(a.x < largeur && a.y < hauteur)
    {
        tableau_cases_visible[(a.y*largeur)+a.x] = 1;
    }
}

void Map::reset_visible()
{
    int size = tableau_cases_visible.size();
    for(int i =0; i<size; i++)
    {
        tableau_cases_visible[i] = 0;
    }
}

void Map::set_visible(int x, int y)
{
    if(x < largeur && y < hauteur && x >= 0 && y >= 0)
    {
        tableau_cases_visible[(y*largeur)+x] = 1;
    }
}

void Map::set_visible_verifie(int origine_x, int origine_y,int destination_x, int destination_y, int origine_pixel_pos_x, int origine_pixel_pos_y)
{
    float destination_pixel_pos_x = origine_pixel_pos_x+(destination_x-origine_x)*taille_case;
    float destination_pixel_pos_y = origine_pixel_pos_y+(destination_y-origine_y)*taille_case;

    if(destination_x < largeur && destination_y < hauteur && destination_x >=0 && destination_y >= 0)
    {
        if(!traverse_mur(origine_pixel_pos_x,origine_pixel_pos_y,destination_pixel_pos_x,destination_pixel_pos_y))
        {
            tableau_cases_visible[(destination_y*largeur)+destination_x] = 1;
        }
    }
}
