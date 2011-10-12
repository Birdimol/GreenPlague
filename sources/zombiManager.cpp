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

void ZombiManager::start_movement_for_zombies(Pathfinder* pathfinder)
{

    int temp = tableau_zombi.size();

    vector<Point> parcourt;

    for(int i=0; i<temp; i++)
    {
        //si ce zombie est en attente, on lui calcule son path et on sort de la boucle
        if(tableau_zombi[i]->is_waiting_for_path())
        {
            Point arrivee_reelle = tableau_zombi[i]->get_destination_memorisee();

            bool accessible = true;
            //si l'arrivee est un mur
            if(map->estMur(arrivee_reelle))
            {
                arrivee_reelle = map->getCaseLibreProche(arrivee_reelle,map->get_liste_case_occupee());
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

            //on va regarder s'il y a un mur entre le zombie et sa destination car s'il n'y a pas de mur, il est inutile de se prendre la tete avec le path !

            //si on a trouve une destination acceptable
            if(accessible)
            {
                Point depart(tableau_zombi[i]->getSprite().GetPosition().x/map->getTailleCase(),tableau_zombi[i]->getSprite().GetPosition().y/map->getTailleCase());
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
                    arrivee_reelle = parcourt[0];
                    //cout << "Ajout du point : " << arrivee_reelle.x <<", "<<arrivee_reelle.y << " dans la liste des cases occuppees."<< endl;
                    map->add_liste_case_occupee(arrivee_reelle);
                }
            }

            //ce zombie n'a plus besoin de calculer son path
            tableau_zombi[i]->stop_wait_for_path();

            //on sort de la boucle
            i = temp*2;
        }
    }

}

void ZombiManager::set_destination_to_active_zombies(int xMousePressed,int yMousePressed)
{
    int temp = tableau_zombi.size();

    vector<int> liste_x;
    vector<int> liste_y;

    Point depart;
    Point arrivee;

    arrivee.x = xMousePressed/map->getTailleCase();
    arrivee.y = yMousePressed/map->getTailleCase();

    for(int i=0; i<temp; i++)
    {
        if(tableau_zombi[i]->getCible())
        {
            tableau_zombi[i]->set_wait_for_path(arrivee);
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

void ZombiManager::set_visibility()
{
    int temp = tableau_zombi.size();
    for(int i=0; i<temp; i++)
    {
        cout <<tableau_zombi[i]->getSprite().GetRotation() << endl;

        int posx = floor(tableau_zombi[i]->getSprite().GetPosition().x/map->getTailleCase());
        int posy = floor(tableau_zombi[i]->getSprite().GetPosition().y/map->getTailleCase());
        if(tableau_zombi[i]->getSprite().GetRotation() <= 22.5 || tableau_zombi[i]->getSprite().GetRotation() > 337.5)
        {
            map->set_visible(posx,posy);

            map->set_visible(posx+1,posy-1);
            map->set_visible(posx+1,posy);
            map->set_visible(posx+1,posy+1);

            map->set_visible(posx+2,posy-1);
            map->set_visible(posx+2,posy);
            map->set_visible(posx+2,posy+1);

            map->set_visible(posx+3,posy-2);
            map->set_visible(posx+3,posy-1);
            map->set_visible(posx+3,posy);
            map->set_visible(posx+3,posy+1);
            map->set_visible(posx+3,posy+2);

            map->set_visible(posx+4,posy-2);
            map->set_visible(posx+4,posy-1);
            map->set_visible(posx+4,posy);
            map->set_visible(posx+4,posy+1);
            map->set_visible(posx+4,posy+2);
        }
        else if(tableau_zombi[i]->getSprite().GetRotation() <= 67.5 && tableau_zombi[i]->getSprite().GetRotation() > 22.5)
        {
            map->set_visible(posx,posy);
            map->set_visible(posx+1,posy);
            map->set_visible(posx+2,posy);
            map->set_visible(posx,posy-1);
            map->set_visible(posx,posy-2);

            map->set_visible(posx+1,posy-1);
            map->set_visible(posx+1,posy-2);
            map->set_visible(posx+2,posy-1);

            map->set_visible(posx+1,posy-3);
            map->set_visible(posx+3,posy-1);

            map->set_visible(posx+2,posy-2);

            map->set_visible(posx+2,posy-3);
            map->set_visible(posx+3,posy-3);
            map->set_visible(posx+3,posy-2);
        }
        else if(tableau_zombi[i]->getSprite().GetRotation() <= 112.5 && tableau_zombi[i]->getSprite().GetRotation() > 67.5)
        {
            map->set_visible(posx,posy);

            map->set_visible(posx+1,posy-1);
            map->set_visible(posx,posy-1);
            map->set_visible(posx-1,posy-1);

            map->set_visible(posx+1,posy-2);
            map->set_visible(posx,posy-2);
            map->set_visible(posx-1,posy-2);

            map->set_visible(posx+2,posy-3);
            map->set_visible(posx+1,posy-3);
            map->set_visible(posx,posy-3);
            map->set_visible(posx-1,posy-3);
            map->set_visible(posx-2,posy-3);

            map->set_visible(posx-2,posy-4);
            map->set_visible(posx-1,posy-4);
            map->set_visible(posx,posy-4);
            map->set_visible(posx+1,posy-4);
            map->set_visible(posx+2,posy-4);
        }
        else if(tableau_zombi[i]->getSprite().GetRotation() <= 157.5 && tableau_zombi[i]->getSprite().GetRotation() > 112.5)
        {
            map->set_visible(posx,posy);
            map->set_visible(posx-1,posy-1);
            map->set_visible(posx-2,posy-2);
        }
        else if(tableau_zombi[i]->getSprite().GetRotation() <= 202.5 && tableau_zombi[i]->getSprite().GetRotation() > 157.5)
        {
            map->set_visible(posx,posy);
            map->set_visible(posx-1,posy);
            map->set_visible(posx-2,posy);
        }
        else if(tableau_zombi[i]->getSprite().GetRotation() <= 247.5 && tableau_zombi[i]->getSprite().GetRotation() > 202.5)
        {
            map->set_visible(posx,posy);
            map->set_visible(posx-1,posy+1);
            map->set_visible(posx-2,posy+2);
        }
        else if(tableau_zombi[i]->getSprite().GetRotation() <= 292.5 && tableau_zombi[i]->getSprite().GetRotation() > 247.5)
        {
            map->set_visible(posx,posy);
            map->set_visible(posx,posy+1);
            map->set_visible(posx,posy+2);
        }
        else
        {
            map->set_visible(posx,posy);
            map->set_visible(posx+1,posy+1);
            map->set_visible(posx+2,posy+2);
        }
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
