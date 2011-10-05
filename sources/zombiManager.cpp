#include "zombiManager.hpp"
#include "tools.cpp"


using namespace std;

ZombiManager::ZombiManager(Map *map_envoyee)
{
    map = map_envoyee;

    vector<Point> liste_zombie = map->get_tableau_zombie();
    int size = liste_zombie.size();
    for(int i=0; i< size;i++)
    {
        tableau_zombi.push_back(new Zombi(map,20+liste_zombie[i].x*map->getTailleCase(),20+liste_zombie[i].y*map->getTailleCase()));
    }

    image_cible.LoadFromFile("pictures/cercle_selection.png");
    sprite_cible.SetImage(image_cible);
}

void ZombiManager::detecte_selection(int xMousePressed,int yMousePressed,int xMouseReleased,int yMouseReleased)
{
    int temp = tableau_zombi.size();
    for(int i=0; i<temp; i++)
    {
        //si le zombi est dans la selection
        tableau_zombi[i]->est_dans_rectangle(xMousePressed, yMousePressed, xMouseReleased, yMouseReleased);
    }

}

void ZombiManager::set_destination_to_active_zombies(int xMousePressed,int yMousePressed,sf::RenderWindow* App, Pathfinder* pathfinder)
{
    int temp = tableau_zombi.size();
    int compte = 0;

    vector<int> liste_x;
    vector<int> liste_y;
    vector<Point> parcourt;

    Point depart;
    Point arrivee;

    for(int i=0; i<temp; i++)
    {
        if(tableau_zombi[i]->getCible())
        {
            liste_x.push_back(tableau_zombi[i]->getSprite().GetPosition().x);
            liste_y.push_back(tableau_zombi[i]->getSprite().GetPosition().y);
        }
    }

    float moyenne_x = 0;
    float moyenne_y = 0;

    int nombre_total = liste_x.size();

    for(int z=0;z<nombre_total;z++)
    {
        moyenne_x+=liste_x[z];
    }

    for(int z=0;z<nombre_total;z++)
    {
        moyenne_y+=liste_y[z];
    }

    moyenne_x = moyenne_x/nombre_total;
    moyenne_y = moyenne_y/nombre_total;

    float difference_x = moyenne_x - xMousePressed;
    float difference_y = moyenne_y - yMousePressed;

    float droite_x = difference_y;
    float droite_y = -difference_x;

    float rapportxy = droite_y/droite_x;
    float distance_x_entre_destination = 20 / sqrt(rapportxy*rapportxy + 1);
    float distance_y_entre_destination = distance_x_entre_destination * rapportxy;

    sf::Color border(0,200,0,255);
    sf::Shape line = sf::Shape::Line(moyenne_x,moyenne_y, xMousePressed, yMousePressed, 3, border);

    sf::Color border2(255,200,0,255);
    sf::Shape line2 = sf::Shape::Line(xMousePressed+distance_x_entre_destination,yMousePressed+distance_y_entre_destination, xMousePressed-distance_x_entre_destination, yMousePressed-distance_y_entre_destination, 3, border);

    App->Draw(line);
    App->Draw(line2);

    vector<Point> liste_case_occupee;

    int nombre_zombies = get_nombre_zombis();
    for(int i=0;i<nombre_zombies;i++)
    {
        if(!tableau_zombi[i]->getCible())
        {
            liste_case_occupee.push_back(tableau_zombi[i]->get_position());
        }
    }

    for(int i=0;i<liste_case_occupee.size();i++)
    {
        cout << "case " << liste_case_occupee[i].x << ", " << liste_case_occupee[i].y << "occupee." << endl;;
    }

    arrivee.x = xMousePressed/map->getTailleCase();
    arrivee.y = yMousePressed/map->getTailleCase();

    //si l'arrivee est un mur
    cout << "est-ce un mur ?" << endl;
    if(map->estMur(arrivee))
    {
        cout << "arrivee est un mur !" << endl;
        arrivee = map->getCaseLibreProche(arrivee,liste_case_occupee);
    }
    else
    {
        cout << "arrivee est pas un mur !" << endl;
    }

    for(int i=0; i<nombre_zombies; i++)
    {
        cout << "pour zombie selectionne " << i << endl;

        Point arrivee_reelle = arrivee;

        if(tableau_zombi[i]->getCible())
        {
            //on va envoyer les zombies d'abord à la case voulue, puis sur la case vide la plus proche et etc...
            cout << "arrivee reelle" << arrivee_reelle.x << ", " << arrivee_reelle.y << endl;
            if(Tools::contient_point(liste_case_occupee,arrivee_reelle))
            {
                cout << "La case est deja occupee par un zombie !" << endl;
                arrivee_reelle = map->getCaseLibreProche(arrivee_reelle,liste_case_occupee);
            }
            else
            {
                cout << "La case est libre !" << endl;
            }

            depart.x = tableau_zombi[i]->getSprite().GetPosition().x/map->getTailleCase();
            depart.y = tableau_zombi[i]->getSprite().GetPosition().y/map->getTailleCase();

            cout << "calcul du path() pour aller sur la case : " << arrivee_reelle.x << ", " << arrivee_reelle.y << endl;
            parcourt = pathfinder->calcul_path(depart,arrivee_reelle);

            cout << "Path de taille : " << parcourt.size() << endl;
            if(parcourt.size() > 0)
            {
                for(int j=0; j<parcourt.size(); j++)
                {
                    if(j==0)
                    {
                        //la premiere fois on supprime les anciennes destinations
                        tableau_zombi[i]->set_cible_du_zombie(parcourt[j].x*map->getTailleCase()+map->getTailleCase()/2,parcourt[j].y*map->getTailleCase()+map->getTailleCase()/2);
                    }
                    else
                    {
                        tableau_zombi[i]->add_cible_du_zombie(parcourt[j].x*map->getTailleCase()+map->getTailleCase()/2,parcourt[j].y*map->getTailleCase()+map->getTailleCase()/2);
                    }
                    cout << "x,y du path ajoute : " << parcourt[j].x*map->getTailleCase()+map->getTailleCase()/2 <<", "<<parcourt[j].y*map->getTailleCase()+map->getTailleCase()/2<< endl;
                }
            }
            cout << endl;
            /*
            }
            else if(compte%2 == 0)
            {
                tableau_zombi[i]->set_cible_du_zombie(xMousePressed+(compte-1)*distance_x_entre_destination+distance_x_entre_destination,yMousePressed+(compte-1)*distance_y_entre_destination+distance_y_entre_destination);
                cout << ((int)(compte/2)-1) << endl<< endl;
            }
            else
            {
                tableau_zombi[i]->set_cible_du_zombie(xMousePressed+(compte)*-distance_x_entre_destination-distance_x_entre_destination,yMousePressed+(compte)*-distance_y_entre_destination-distance_y_entre_destination);
                cout << ((int)(compte/2)) << endl<< endl;
            }

            compte++;
            */
        }
    }
}

void ZombiManager::iteration_vie_zombi(float ElapsedTime)
{
    int temp = tableau_zombi.size();
    for(int i=0; i<temp; i++)
    {
        //le zombie ne fait rien actuellement
        tableau_zombi[i]->tourner(ElapsedTime);
        tableau_zombi[i]->avancer(ElapsedTime);
    }
}

int ZombiManager::get_nombre_zombis()
{
    return tableau_zombi.size();
}

bool ZombiManager::balle_touche_zombi(float x, float y)
{
    int temp = tableau_zombi.size();
    for(int i=0; i<temp; i++)
    {
        float distance_x = x - tableau_zombi[i]->getSprite().GetPosition().x;
        float distance_y = y - tableau_zombi[i]->getSprite().GetPosition().y;

        if(distance_x < 0)
        {
            distance_x = distance_x*-1;
        }

        if(distance_y < 0)
        {
            distance_y = distance_y*-1;
        }

        if(sqrt((distance_x * distance_x) + (distance_y * distance_y)) < 30)
        {
            tableau_zombi.erase(tableau_zombi.begin()+i);
            return true;
        }
    }
    return false;
}

void ZombiManager::afficher_data()
{
    system("cls");
    int temp = tableau_zombi.size();
    for(int i=0; i<temp; i++)
    {
        tableau_zombi[i]->afficher_data();
    }
}

void ZombiManager::afficher(sf::RenderWindow* App)
{
    int temp = tableau_zombi.size();
    for(int i=0; i<temp; i++)
    {
        if(tableau_zombi[i]->getCible())
        {
            sprite_cible.SetPosition(tableau_zombi[i]->getSprite().GetPosition().x-(tableau_zombi[i]->getImage().GetWidth()/2),tableau_zombi[i]->getSprite().GetPosition().y-tableau_zombi[i]->getImage().GetHeight()/2);
             App->Draw(sprite_cible);
        }
    }
    for(int i=0; i<temp; i++)
    {
        App->Draw(tableau_zombi[i]->getSprite());
    }
}
