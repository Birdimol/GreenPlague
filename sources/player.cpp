#include "player.hpp"

using namespace std;

Player::Player(string nom_envoye, Map* map_envoyee)
{
    nom = nom_envoye;
    map = map_envoyee;

    munitions = 0;
    arme = false;

    if (image.LoadFromFile("pictures/sprite.png"))
    {
        image_shoot.LoadFromFile("pictures/shoot.png");
        sprite_shoot.SetImage(image_shoot);

        image_cercle.LoadFromFile("pictures/cercle.png");
        sprite_cercle.SetImage(image_cercle);

        sprite.SetImage(image);

        vie = 5;

        sprite.SetPosition(100.f, 150.f);
        sprite.SetCenter(15.f, 20.f);

        sprite_cercle.SetCenter(20.f, 20.f);
        sprite_cercle.SetPosition(sprite.GetPosition().x,sprite.GetPosition().y);

        sprite_shoot.SetPosition(100.f, 150.f);
        sprite_shoot.SetCenter(15.f, 20.f);

        direction_x = 1;
        direction_y = 0;

        statut = 0;

        rotation = 0;

        cout << "Personnage " << nom << " cree." << endl;
    }
}

int Player::get_vie()
{
    return vie;
}

void Player::get_arme()
{
    arme = true;
    munitions += 6;
}

int Player::get_munition()
{
    return munitions;
}

void Player::add_munition(int nombre)
{
    munitions += nombre;
}

void Player::tourner(float angle)
{
    if(statut == 0)
    {
        sprite.Rotate(angle);
        direction_x = cos(sprite.GetRotation() * M_PI / 180);
        direction_y = -sin(sprite.GetRotation() * M_PI / 180);
    }
}

sf::Sprite Player::getSprite()
{
    if(statut == 1)
    {
        if((clock.GetElapsedTime() - temps_dernier_coup) < 0.5)
        {
            return sprite_shoot;
        }
        else
        {
            statut = 0;
            return sprite;
        }
    }
    else
    {
        return sprite;
    }
}

sf::Sprite Player::getCercle()
{
    return sprite_cercle;
}

void Player::subir_degat()
{
    vie = vie -1;
}

bool Player::have_arme()
{
    return arme;
}

int Player::shoot()
{
    if(statut == 0)
    {
        if(arme)
        {
            if(munitions > 0)
            {
                if((clock.GetElapsedTime() - temps_dernier_coup) > intervalle_coup)
                {
                    cout << "Feu, reste "<< munitions <<" balles !" << endl;
                    munitions--;
                    sprite_shoot.SetRotation(sprite.GetRotation());
                    sprite_shoot.SetPosition(sprite.GetPosition());
                    statut = 1;
                    temps_dernier_coup = clock.GetElapsedTime();
                    //ok
                    return 1;
                }
            }
            else
            {
                //pas de balles
                return 3;
            }
        }
        else
        {
            //pas d'arme
            return 2;
        }
    }
    else
    {
        //pas ready
        return 0;
    }
}

void Player::avancer(float distance)
{
    if(statut == 0)
    {
        //gestion des collisions
        int x_bloc = -1;
        int y_bloc = -1;

        //deplacement en x
        if(map->collision(sprite.GetPosition().x+(direction_x * distance),sprite.GetPosition().y, &x_bloc, &y_bloc))
        {
            //cout << "boum avec le bloc : " << x_bloc << "," << y_bloc << endl;
        }
        else
        {
            sprite.Move(direction_x * distance, 0);
        }

        //deplacement en y
        if(map->collision(sprite.GetPosition().x,sprite.GetPosition().y+(direction_y * distance), &x_bloc, &y_bloc))
        {
            //cout << "boum avec le bloc : " << x_bloc << "," << y_bloc << endl;
        }
        else
        {
            sprite.Move(0, direction_y * distance);
        }
    }
    sprite_cercle.SetPosition(sprite.GetPosition().x,sprite.GetPosition().y);
}

void Player::reculer(float distance)
{
    if(statut == 0)
    {
        sprite.Move(direction_x * -distance, direction_y * -distance);
    }
}
