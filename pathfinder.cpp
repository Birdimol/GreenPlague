#include "pathfinder.hpp"



bool operator==(Point const& a, Point const& b)
{
    if((a.x == b.x) && (a.y == b.y))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool operator!=(Point const& a, Point const& b)
{
    if((a.x != b.x) || (a.y != b.y))
    {
        return true;
    }
    else
    {
        return false;
    }
}

Pathfinder::Pathfinder(vector<int> carte_, int largeur_, int hauteur_)
{
    carte = carte_;
    largeur = largeur_;
    hauteur = hauteur_;
}

bool Pathfinder::contient(vector<Point> tableau,int x,int y)
{
    int taille = tableau.size();

    if(taille == 0)
    {
        return false;
    }

    for(int a = taille; a>-1 ;a--)
    {
        if(tableau[a].x == x && tableau[a].y == y)
        {
            return true;
        }
    }
    return false;
}

bool Pathfinder::existe_pas_dans_vector(int x, int y,vector<Point> points_deja_verifies)
{
    int size = points_deja_verifies.size();
    for(int i=0; i< size;i++)
    {
        if(points_deja_verifies[i].x == x && points_deja_verifies[i].y == y)
        {
            return false;
        }
    }
    return true;
}

vector<Point> Pathfinder::calcul_distance_carte_vector(Point depart,Point arrivee)
{
    int x = 0;
    int y = 0;

    vector<Point> parcourt;

    Case minimum(99,99);

    for(x=0;x<((int)carte.size());x++)
    {
        carte_distances.push_back(Case());
        carte_distances[x].F = -1;
        carte_distances[x].G = -1;
        carte_distances[x].H = -1;
    }

    carte_distances[depart.x + depart.y*hauteur].H = distance_entre_2_point(depart,arrivee);
    carte_distances[depart.x + depart.y*hauteur].G = 0;
    carte_distances[depart.x + depart.y*hauteur].F = carte_distances[depart.x + depart.y*hauteur].G + carte_distances[depart.x + depart.y*hauteur].H;

    vector<Point> points_deja_verifies;

    Point actuel(depart.x,depart.y);
    Point point_temp(0,0);

    //initialisation
    points_deja_verifies.push_back(depart);
    for(x=0;x<3;x++)
    {
        for(y=0;y<3;y++)
        {
            //si c'est dans la map
            if(actuel.x + (x-1) < 5 && actuel.x + (x-1) > -1 && actuel.y + (y-1) < 5 && actuel.y + (y-1) > -1 && carte[actuel.x + (x-1) + ((actuel.y + (y-1))*hauteur)] != 1)
            {
                if((!(x == 1 && y == 1 )) && (!(x == 0 && y == 0 )) && (!(x == 2 && y == 0 )) && (!(x == 0 && y == 2 )) && (!(x == 2 && y == 2 )))
                {
                    point_temp.x = actuel.x + (x-1);
                    point_temp.y = actuel.y + (y-1);
                    if(carte_distances[point_temp.x + point_temp.y*hauteur].G == -1 || carte_distances[point_temp.x + point_temp.y*hauteur].G > (carte_distances[actuel.x + actuel.y*hauteur].G + distance_entre_2_point(actuel,point_temp)))
                    {
                        carte_distances[point_temp.x + point_temp.y*hauteur].G = (carte_distances[actuel.x + actuel.y*hauteur].G + distance_entre_2_point(actuel,point_temp));
                        carte_distances[point_temp.x + point_temp.y*hauteur].H = distance_entre_2_point(point_temp,arrivee);
                        carte_distances[point_temp.x + point_temp.y*hauteur].F = carte_distances[point_temp.x + point_temp.y*hauteur].G+carte_distances[point_temp.x + point_temp.y*hauteur].H;
                        carte_distances[point_temp.x + point_temp.y*hauteur].set_parent(actuel.x,actuel.y);
                        //cout << "nouveau parent pour " << point_temp.x  << ", " << point_temp.y << " : " << actuel.x << ", " << actuel.y << endl;
                        //cout << "nouvelles valeurs pour " << point_temp.x  << ", " << point_temp.y << " G: "<< carte_distances[point_temp.x + point_temp.y*hauteur].G << ", H: "<< carte_distances[point_temp.x + point_temp.y*hauteur].H << ", F:" << carte_distances[point_temp.x + point_temp.y*hauteur].F<< endl;
                    }
                    else
                    {
                        //cout << carte_distances[point_temp.x + point_temp.y*hauteur].G << " < " << (carte_distances[actuel.x + actuel.y*hauteur].G + distance_entre_2_point(actuel,point_temp)) << endl;
                        //cout << "pas de nouvelles valeurs pour " << point_temp.x  << ", " << point_temp.y << " G: "<< carte_distances[point_temp.x + point_temp.y*hauteur].G << ", H: "<< carte_distances[point_temp.x + point_temp.y*hauteur].H << ", F:" << carte_distances[point_temp.x + point_temp.y*hauteur].F<< endl;
                    }
                }
            }
            else
            {
                //cout << actuel.x + (x-1) << "," << actuel.y + (y-1) << " est hors map." << endl;
            }
        }
    }

    int nbr_max_iterations = 100;
    int nombre_iterations = 0;

    while(nombre_iterations < nbr_max_iterations && actuel != arrivee)
    {
        //on bosse sur le point minimum
        minimum.F = 99;

        //cout << "recherche du point minimum" << endl;

        for(x=0;x<largeur;x++)
        {
            for(y=0;y<hauteur;y++)
            {
                if(minimum.F >= carte_distances[x + y*hauteur].F && carte_distances[x + y*hauteur].F != -1)
                {
                        if(existe_pas_dans_vector(x,y,points_deja_verifies))
                        {
                            //cout << "Nouveau minimum "<< x << ", " << y << " : " << carte_distances[x + y*hauteur].F << endl;
                            minimum = carte_distances[x + y*hauteur];
                            point_temp.x = x;
                            point_temp.y = y;
                        }
                        else
                        {
                            //cout << "Case  : " << x << ", " << y << " deja verifiee." << endl;
                        }

                }
                else
                {
                    //cout << "F trop elve sur case  : " << x << ", " << y << " : " << carte_distances[x + y*hauteur].F << endl;
                }
            }
        }

        points_deja_verifies.push_back(point_temp);
        actuel = point_temp;

        //cout << "checking : " << actuel.x << ", " << actuel.y << endl;

        for(x=0;x<3;x++)
        {
            for(y=0;y<3;y++)
            {
                //si c'est dans la map
                if(actuel.x + (x-1) < largeur && actuel.x + (x-1) > -1 && actuel.y + (y-1) < hauteur && actuel.y + (y-1) > -1 && carte[actuel.x + (x-1) + ((actuel.y + (y-1))*hauteur)] != 1)
                {
                    if((!(x == 1 && y == 1 )) && (!(x == 0 && y == 0 )) && (!(x == 2 && y == 0 )) && (!(x == 0 && y == 2 )) && (!(x == 2 && y == 2 )))
                    {
                        point_temp.x = actuel.x + (x-1);
                        point_temp.y = actuel.y + (y-1);
                        if(carte_distances[point_temp.x + point_temp.y*hauteur].G == -1 || carte_distances[point_temp.x + point_temp.y*hauteur].G > (carte_distances[actuel.x + actuel.y*hauteur].G + distance_entre_2_point(actuel,point_temp)))
                        {
                            carte_distances[point_temp.x + point_temp.y*hauteur].G = (carte_distances[actuel.x + actuel.y*hauteur].G + distance_entre_2_point(actuel,point_temp));
                            carte_distances[point_temp.x + point_temp.y*hauteur].H = distance_entre_2_point(point_temp,arrivee);
                            carte_distances[point_temp.x + point_temp.y*hauteur].F = carte_distances[point_temp.x + point_temp.y*hauteur].G+carte_distances[point_temp.x + point_temp.y*hauteur].H;
                            carte_distances[point_temp.x + point_temp.y*hauteur].set_parent(actuel.x,actuel.y);
                            //cout << "nouveau parent pour " << point_temp.x  << ", " << point_temp.y << " : " << actuel.x << ", " << actuel.y << endl;
                        }
                        else
                        {
                            //cout << carte_distances[point_temp.x + point_temp.y*hauteur].G << " < " << (carte_distances[actuel.x + actuel.y*hauteur].G + distance_entre_2_point(actuel,point_temp)) << endl;
                            //cout << "pas de nouvelles valeurs pour " << point_temp.x  << ", " << point_temp.y << " G: "<< carte_distances[point_temp.x + point_temp.y*hauteur].G << ", H: "<< carte_distances[point_temp.x + point_temp.y*hauteur].H << ", F:" << carte_distances[point_temp.x + point_temp.y*hauteur].F<< endl;
                        }
                    }
                }
                else
                {
                    //cout << actuel.x + (x-1) << "," << actuel.y + (y-1) << " est hors map." << endl;
                }
            }
        }
        nombre_iterations++;
    }

    //si on a trouvé une route
    if(actuel == arrivee)
    {
        point_temp = arrivee;
        while(point_temp != depart )
        {
            parcourt.push_back(point_temp);
            //cout << point_temp.x << ", " << point_temp.y << "dont le parent est : " << carte_distances[point_temp.x + point_temp.y*hauteur].parent.x << ", " << carte_distances[point_temp.x + point_temp.y*hauteur].parent.y  << endl;
            //cout << carte_distances[point_temp.x][point_temp.y].parent.x << ", " << carte_distances[point_temp.x][point_temp.y].parent.y << endl;
            point_temp = carte_distances[point_temp.x + point_temp.y*hauteur].parent;
        }


        parcourt.push_back(depart);
    }

    int taille_parcourt = parcourt.size();

    //si on a trouve un chemin, on simplifie la trajectoire
    if(taille_parcourt > 0)
    {
        int repere_precedent = taille_parcourt-1;
        int repere_actuel = taille_parcourt-1;

        vector<Point> inutiles;

        for(int z = taille_parcourt-1;z>-1;z--)
        {
            if(z!= 0 && z != taille_parcourt-1)
            {
                //cout << "test x1("<< parcourt[repere_precedent].x <<") = " << parcourt[repere_precedent].x*25+12.5 << ", x2("<< parcourt[z-1].x <<") = " << parcourt[z-1].x*25+12.5 << endl;
                float x1 = parcourt[repere_precedent].x*25+12.5;
                float x2 = parcourt[z-1].x*25+12.5;
                float y1 = parcourt[repere_precedent].y*25+12.5;
                float y2 = parcourt[z-1].y*25+12.5;;

                float x3;
                float y3;
                float x4;
                float y4;
                float x5;
                float y5;
                float x6;
                float y6;

                float angle;

                //vertical
                if(x1==x2)
                {
                   x3 = x1+5;
                   y3 = y1;
                   x4 = x2+5;
                   y4 = y2;

                   x5 = x1-5;
                   y5 = y1;
                   x6 = x2-5;
                   y6 = y2;
                }
                //horizontal
                else if(y1==y2)
                {
                   x3 = x1;
                   y3 = y1+5;
                   x4 = x2;
                   y4 = y2+5;

                   x5 = x1;
                   y5 = y1-5;
                   x6 = x2;
                   y6 = y2-5;
                }
                else
                {
                    float a = ((float)x2-(float)x1);
                    float b = ((float)y1-(float)y2);

                    angle = a/b;
                    //cout << " a : "<< a << "/" << b << " angle : " << angle << " distance : " << sqrt((5*(angle))*(5*(angle))+(5*(angle))*(5*(angle))) << endl;

                    a = -5 / sqrt(1 + angle*angle);
                    b = a*angle;

                    x3 = x1 + a;
                    y3 = y1 + b;

                    x4 = x2 + a;
                    y4 = y2 + b;

                    x5 = x1 - a;
                    y5 = y1 - b;

                    x6 = x2 - a;
                    y6 = y2 - b;
                }

                if((!traverse_mur(x1,y1,x2,y2))&&(!traverse_mur(x3,y3,x4,y4))&&(!traverse_mur(x5,y5,x6,y6)))
                {
                    //cout << "Pas de mur entre le repere " << parcourt[repere_precedent].x << ", " << parcourt[repere_precedent].y << " et la case  " << parcourt[z-1].x << ", " << parcourt[z-1].y << endl;
                    //cout << "case " << parcourt[z].x << ", " << parcourt[z].y << " inutile dans le parcourt." << endl << endl;
                    inutiles.push_back(parcourt[z]);
                }
                else
                {
                    //cout << "Mur entre la case " << parcourt[repere_precedent].x << ", " << parcourt[repere_precedent].y << " et la case  " << parcourt[z-1].x << ", " << parcourt[z-1].y  << endl << endl;
                    repere_precedent = z;
                }
            }
            //parcourt[z].afficher();
        }

        int suppression = 1;
        int a_supprimer = 0;

        while(suppression == 1)
        {
            suppression = 0;
            for(int z = taille_parcourt-1;z>-1;z--)
            {
                if(contient(inutiles,parcourt[z].x,parcourt[z].y))
                {
                    suppression = 1;
                    a_supprimer = z;
                }
            }

            if(suppression)
            {
                parcourt.erase(parcourt.begin()+a_supprimer);
            }
        }
    }

    return parcourt;
}

bool Pathfinder::traverse_mur(float x1,float y1,float x2,float y2)
{
    //cout << "la droite va en x de " << x1 << " a " << x2 << endl;

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
        for(int j=0;j<largeur;j++)
        {
            y = j * 25;
            x = i * 25;

            //si c'est un mur
            if(carte[(i)+j*largeur] == 1)
            {
                //il y a possibilité de chevauchement
                if(x1 <= x+25 && x2 >= x)
                {
                    float x_min=x;
                    float x_max=x+25;

                    if(x1 > x)
                    {
                        x_min = x1;
                    }

                    if(x2 < x+25)
                    {
                        x_max = x2;
                    }

                    //cout << "entre : " << x_min << ", et : " << x_max << endl;

                    //il faut maintenant savoir la valeur de la droite en x_min et x_max
                    float y_min = y1+((y1 - y2)/(x1 - x2))*(x_min - x1);
                    float y_max = y1+((y1 - y2)/(x1 - x2))*(x_max - x1);

                    if(x1 == x2)
                    {
                        y_min = y1;
                        y_max = y2;
                    }


                    if((y_min > y+25 && y_max > y+25) || (y_min < y && y_max < y))
                    {

                    }
                    else
                    {
                        //cout << x1 << ", " << y1 << " -> " << x2 << ", " << y2 << " traverse un mur." << endl;
                        return 1;
                    }

                }
            }
        }
    }
    //cout << x1 << ", " << y1 << " -> " << x2 << ", " << y2 << " ne traverse pas un mur." << endl;
    return 0;
}

float Pathfinder::distance_entre_2_point(Point a,Point b)
{
    float diffx = a.x - b.x;
    float diffy = a.y - b.y;

    if(diffx < 0)
    {
        diffx *= -1;
    }

    if(diffy < 0)
    {
        diffy *= -1;
    }

    return sqrt((diffx * diffx)+(diffy * diffy));
}

void Pathfinder::afficher_parcourt(vector<int> carte_vector, int largeur_carte, int hauteur_carte, Point depart, Point arrivee, vector<Point> parcourt)
{
    sf::RenderWindow App(sf::VideoMode(largeur_carte*25, hauteur_carte*25, 32), "SFML Graphics");
    App.SetFramerateLimit(60);

    float temoin = App.GetFrameTime();
    float ElapsedTime = App.GetFrameTime();

    sf::Color couleur(0,0,0);
    sf::Color couleur2(70,70,235);

    sf::Shape Line = sf::Shape::Line(0, 0, 100, 100, 1, couleur2);

    int taille_parcourt = parcourt.size();

    sf::Image icase_vide;
    sf::Sprite scase_vide;
    icase_vide.LoadFromFile("pictures/case_vide.png");
    scase_vide.SetImage(icase_vide);

    sf::Image imur;
    sf::Sprite smur;
    imur.LoadFromFile("pictures/mur.png");
    smur.SetImage(imur);

    sf::Image idepart;
    sf::Sprite sdepart;
    idepart.LoadFromFile("pictures/depart.png");
    sdepart.SetImage(idepart);

    sf::Image iarrivee;
    sf::Sprite sarrivee;
    iarrivee.LoadFromFile("pictures/arrivee.png");
    sarrivee.SetImage(iarrivee);

    sf::Image ipassage;
    sf::Sprite spassage;
    ipassage.LoadFromFile("pictures/passage.png");
    spassage.SetImage(ipassage);

    int compte =0;

    while(temoin + 60 > ElapsedTime && App.IsOpened())
    {
        sf::Event Event;
        while (App.GetEvent(Event))
        {
            if (Event.Type == sf::Event::Closed)
                App.Close();
        }

        if (App.GetInput().IsKeyDown(sf::Key::Escape))
        {
            App.Close();
        }

        App.Clear(couleur);

        int taille_map = ((int)carte_vector.size());
        for(int a=0;a < taille_map;a++)
        {
            if((a%largeur_carte) == depart.x && floor(a/largeur_carte) == depart.y)
            {
                sdepart.SetPosition((a%largeur_carte)*25,floor(a/largeur_carte)*25);
                App.Draw(sdepart);
            }
            else if((a%largeur_carte) == arrivee.x && floor(a/largeur_carte) == arrivee.y)
            {
                sarrivee.SetPosition((a%largeur_carte)*25,floor(a/largeur_carte)*25);
                App.Draw(sarrivee);
            }
            else if(contient(parcourt,(a%largeur_carte),floor(a/largeur_carte)))
            {
                spassage.SetPosition((a%largeur_carte)*25,floor(a/largeur_carte)*25);
                App.Draw(spassage);
            }
            else if(carte_vector[a]==0)
            {
                scase_vide.SetPosition((a%largeur_carte)*25,floor(a/largeur_carte)*25);
                App.Draw(scase_vide);
            }
            else
            {
                smur.SetPosition((a%largeur_carte)*25,floor(a/largeur_carte)*25);
                App.Draw(smur);
            }
        }

        if(taille_parcourt > 0)
        {
            for(int z = taille_parcourt-1;z>-1;z--)
            {
                if(z != 0)
                {
                    Line = sf::Shape::Line(parcourt[z-1].x*25+12.5, parcourt[z-1].y*25+12.5, parcourt[z].x*25+12.5, parcourt[z].y*25+12.5, 1, couleur2);
                    App.Draw(Line);
                }
            }
        }

        App.Display();

        ElapsedTime += App.GetFrameTime();
        compte++;
    }
}
