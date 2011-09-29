#include "zombiManager.hpp"

using namespace std;

ZombiManager::ZombiManager(Player *player_envoye,Map *map_envoyee)
{
    player = player_envoye;
    map = map_envoyee;
    tableau_zombi.push_back(new Zombi(player,map,200,400));
    tableau_zombi.push_back(new Zombi(player,map,400,200));
    tableau_zombi.push_back(new Zombi(player,map,600,300));
    tableau_zombi.push_back(new Zombi(player,map,50,500));
}

void ZombiManager::iteration_vie_zombi(float ElapsedTime)
{
    int temp = tableau_zombi.size();
    for(int i=0; i<temp; i++)
    {
        tableau_zombi[i]->definir_cible();
        tableau_zombi[i]->tourner(ElapsedTime);
        tableau_zombi[i]->avancer(ElapsedTime);
        tableau_zombi[i]->attaquer();
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
        App->Draw(tableau_zombi[i]->getSprite());
    }
}
