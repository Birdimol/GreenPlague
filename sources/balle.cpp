#include "balle.hpp"

using namespace std;

Balle::Balle(sf::Image image_a, Map *map_)
{
    sprite.SetImage(image_a);
    statut = 0;
    map = map_;
}

void Balle::set(float x_envoye, float y_envoye, float direction_x_envoye, float direction_y_envoye, float rotation_envoye)
{
    sprite.SetPosition(x_envoye, y_envoye);
    sprite.SetCenter(2.f, 1.f);

    direction_x = direction_x_envoye;
    direction_y = direction_y_envoye;

    statut = 1;

    rotation = rotation_envoye;
}

void Balle::avancer(float time)
{
    if(statut == 1)
    {
        //collision avec les blocs
        if(map->pointdansmur(sprite.GetPosition().x+(direction_x  * time * 400),sprite.GetPosition().y+(direction_y * time * 400)))
        {
            statut = 0;
        }
        else
        {
            sprite.Move(direction_x * time * 400, direction_y * time * 400);
        }

        //sortie de l'écran
        if(sprite.GetPosition().x < 0 || sprite.GetPosition().x > 800 || sprite.GetPosition().y < 0 || sprite.GetPosition().y > 600)
        {
            statut = 0;
        }
    }
}

sf::Sprite Balle::getSprite()
{
    return sprite;
}


