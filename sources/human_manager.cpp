#include "human_manager.hpp"

void HumanManager::iteration_vie_human(float ElapsedTime)
{

    int temp = tableau_human.size();
    for(int i=0; i<temp; i++)
    {
        //le zombie ne fait rien actuellement
        //tableau_human[i]->tourner(ElapsedTime);
        //tableau_human[i]->avancer(ElapsedTime);
    }
}

int HumanManager::get_nombre_human()
{
    return tableau_human.size();
}

void HumanManager::afficher_data()
{
    system("cls");
    int temp = tableau_human.size();
    for(int i=0; i<temp; i++)
    {
        //tableau_human[i]->afficher_data();
    }
}

void HumanManager::afficher(sf::RenderWindow* App)
{
    int temp = tableau_human.size();
    for(int i=0; i<temp; i++)
    {
        /*
        if(tableau_human[i]->getCible())
        {
            //sprite_cible.SetPosition(tableau_human[i]->getSprite().GetPosition().x-(tableau_human[i]->getImage().GetWidth()/2),tableau_human[i]->getSprite().GetPosition().y-tableau_human[i]->getImage().GetHeight()/2);
            //App->Draw(sprite_cible);
        }
        */
    }
    for(int i=0; i<temp; i++)
    {
        //App->Draw(tableau_human[i]->getSprite());
    }
}
