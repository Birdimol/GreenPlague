#include "zombiManager.hpp"
#include "tools.cpp"
#include <limits>


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

    /*
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

    */

    int nombre_zombies = get_nombre_zombis();

    arrivee.x = xMousePressed/map->getTailleCase();
    arrivee.y = yMousePressed/map->getTailleCase();

    for(int i=0; i<nombre_zombies; i++)
    {
        Point arrivee_reelle = arrivee;

        if(tableau_zombi[i]->getCible())
        {

            bool accessible = true;
            //si l'arrivee est un mur
            if(map->estMur(arrivee))
            {
                arrivee_reelle = map->getCaseLibreProche(arrivee,map->get_liste_case_occupee());
                if(arrivee_reelle.x == -1)
                {
                    //pas de destination trouvee
                    accessible = false;
                }
            }

            //Si un zombie est déjà sur la case ou est en train de s'y rendre
            if(Tools::contient_point(map->get_liste_case_occupee(),arrivee_reelle))
            {
                arrivee_reelle = map->getCaseLibreProche(arrivee_reelle,map->get_liste_case_occupee());
                if(arrivee_reelle.x == -1)
                {
                    //pas de destination trouvee
                    accessible = false;
                }
            }

            //si on a trouve une destination acceptable
            if(accessible)
            {
                depart.x = tableau_zombi[i]->getSprite().GetPosition().x/map->getTailleCase();
                depart.y = tableau_zombi[i]->getSprite().GetPosition().y/map->getTailleCase();

                parcourt = pathfinder->calcul_path(depart,arrivee_reelle);

                if(((int)parcourt.size()) > 0)
                {
                    //on ne tient pas compte de la derniere case du parcourt car il s'agit de celle sur laquelle est le zombie actuellement
                    for(int j=0;j<((int)parcourt.size()-1); j++)
                    {
                        if(j==0)
                        {
                            //la premiere fois on supprime les anciennes destinations et on enleve la case occupee par le zombie des cases occupees
                            map->delete_liste_case_occupee(depart);

                            //si le zombie était deja en route pour une destination, il faut annuler cette destination dans la liste des cases occupées.
                            if(tableau_zombi[i]->get_final_destination().x != -1)
                            {
                                Point temporaire(tableau_zombi[i]->get_final_destination().x,tableau_zombi[i]->get_final_destination().y);
                                 map->delete_liste_case_occupee(temporaire);
                            }

                            tableau_zombi[i]->set_cible_du_zombie(parcourt[j].x*map->getTailleCase()+map->getTailleCase()/2,parcourt[j].y*map->getTailleCase()+map->getTailleCase()/2);
                        }
                        else
                        {
                            tableau_zombi[i]->add_cible_du_zombie(parcourt[j].x*map->getTailleCase()+map->getTailleCase()/2,parcourt[j].y*map->getTailleCase()+map->getTailleCase()/2);
                        }
                    }
                    //cout << "Ajout du point : " << arrivee_reelle.x <<", "<<arrivee_reelle.y << " dans la liste des cases occuppees."<< endl;
                    map->add_liste_case_occupee(arrivee_reelle);
                }
                else
                {
                    //on ne bouge pas
                }
            }
            else
            {
                //on ne bouge pas
            }
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
