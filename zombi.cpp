#include "zombi.hpp"


using namespace std;

Zombi::Zombi(Player *player_envoye, Map *map_envoyee, int x, int y)
{
    player = player_envoye;
    map = map_envoyee;
    if (image.LoadFromFile("pictures/zombi.png"))
    {
        sprite.SetImage(image);

        sprite.SetPosition(x, y);
        sprite.SetCenter(5.f, 20.f);

        distance_attaque = 30;
        temps_dernier_coup = 0;
        intervalle_coup = 1;

        localisation_joueur = "";
        label_cible = "";
        vois_joueur = 0;

        clock.Reset();

        etat = 0;
        attaque=0;
        vitesse = 20;

        direction_x = 1;
        direction_y = 0;

        cible_x = sprite.GetPosition().x;
        cible_y = sprite.GetPosition().y;

        rotation = 0;

        cout << "Zombi cree." << endl;
    }
}

bool Zombi::detecter_joueur()
{

    if(map->traverse_mur(sprite.GetPosition().x,sprite.GetPosition().y,player->getSprite().GetPosition().x,player->getSprite().GetPosition().y))
    {
        //cout << "mur" << endl;
        return 0;
    }

    //repere le joueur
    float difference_x = sprite.GetPosition().x - player->getSprite().GetPosition().x;
    float difference_y = sprite.GetPosition().y - player->getSprite().GetPosition().y;

    //angle parfait que le zombie doit avoir pour faire fasse au joueur
    float angle_parfait = (atan(-(difference_y)/difference_x) / M_PI * 180);

    //on s'assure que cette mesure sera entre 0 et 259
    while(angle_parfait < 0)
    {
        angle_parfait += 360;
    }

    while(angle_parfait >= 360)
    {
        angle_parfait -= 360;
    }

    //définir la direction
    if(difference_x >= 0 && difference_y >= 0)
    {
        localisation_joueur = "en haut a gauche";
        //cout << "joueur en haut a gauche du mob" << endl;
        angle_parfait -= 180;
    }
    else if(difference_x < 0 && difference_y >= 0)
    {
        localisation_joueur = "en haut a droite";
        //cout << "joueur en haut a droite du mob" << endl;
    }
    else if(difference_x < 0 && difference_y < 0)
    {
        localisation_joueur = "en bas a droite";
        //cout << "joueur en bas a droite du mob" << endl;
    }
    else if(difference_x >= 0 && difference_y < 0)
    {
        localisation_joueur = "en bas a gauche";
        //cout << "joueur en bas a gauche du mob" << endl;
        angle_parfait += 180;
    }

    difference_angle = (angle_parfait - sprite.GetRotation());

    if(difference_angle < 0)
    {
        difference_angle = difference_angle * -1;
    }

    if(difference_angle > 180)
    {
        difference_angle = 360-difference_angle;
    }

    distance = sqrt((difference_x*difference_x) + (difference_y*difference_y));

    if(difference_angle < 80 || distance < 100)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void Zombi::definir_cible()
{
    vois_joueur = 0;
    if(detecter_joueur())
    {
        //la cible est le joueur
        cible_y = player->getSprite().GetPosition().y;
        cible_x = player->getSprite().GetPosition().x;
        label_cible = "joueur";
        vois_joueur = 1;
        etat = 1;
        vitesse = 50;
    }
    else
    {
        if(etat == 0)
        {
            vitesse = 20;
            do
            {
                cible_y = sprite.GetPosition().y - 100 + sf::Randomizer::Random(0, 200);
                cible_x = sprite.GetPosition().x - 100 + sf::Randomizer::Random(0, 200);
            }while(cible_y < 0 || cible_y > (map->getH()-1)*20 || cible_x < 0 || cible_x > (map->getL()-1)*20);

            etat = 1;
            label_cible = "aleatoire";
        }
    }
}

void Zombi::tourner(float ElapsedTime)
{

    //ici la cible est soit le joueur, soit une destination aléatoire

    float difference_x = sprite.GetPosition().x - cible_x;
    float difference_y = sprite.GetPosition().y - cible_y;
    float angle_parfait = (atan(-(difference_y)/difference_x) / M_PI * 180);

    distance = sqrt((difference_x*difference_x) + (difference_y*difference_y));

    while(angle_parfait < 360)
    {
        angle_parfait += 360;
    }

    while(angle_parfait > 360)
    {
        angle_parfait -= 360;
    }

    //définir la direction
    if(difference_x >= 0 && difference_y >= 0)
    {
        //cout << "joueur en haut a gauche du mob" << endl;
        angle_parfait -= 180;
    }
    else if(difference_x < 0 && difference_y >= 0)
    {
        //cout << "joueur en haut a droite du mob" << endl;
    }
    else if(difference_x < 0 && difference_y < 0)
    {
        //cout << "joueur en bas a droite du mob" << endl;
    }
    else if(difference_x >= 0 && difference_y < 0)
    {
        //cout << "joueur en bas a gauche du mob" << endl;
        angle_parfait += 180;
    }

    difference_angle = (angle_parfait - sprite.GetRotation());

    if(difference_angle < 0)
    {
        difference_angle = difference_angle * -1;
    }

    if(difference_angle > 180)
    {
        difference_angle = 360-difference_angle;
    }

    if(distance < 10)
    {
        etat = 0;
    }

    //si le zombie n'est pas encore dans la bonne direction
    if(difference_angle > 2)
    {

        if(distance > distance_attaque || vois_joueur==0)
        {
            float temp;
            temp = sprite.GetRotation() + 180;

            if((angle_parfait > sprite.GetRotation() && angle_parfait < temp) || ((temp > 360) && (angle_parfait > 0 && angle_parfait < temp-360)))
            {
                if(temp >= 360)
                {
                    if((angle_parfait < 360) || (angle_parfait > 0 && angle_parfait < temp-360))
                    {
                        sprite.Rotate(2);
                        direction_x = cos(sprite.GetRotation() * M_PI / 180);
                        direction_y = -sin(sprite.GetRotation() * M_PI / 180);
                    }
                    else
                    {
                        sprite.Rotate(-2);
                        direction_x = cos(sprite.GetRotation() * M_PI / 180);
                        direction_y = -sin(sprite.GetRotation() * M_PI / 180);
                    }
                }
                else
                {
                    sprite.Rotate(2);
                    direction_x = cos(sprite.GetRotation() * M_PI / 180);
                    direction_y = -sin(sprite.GetRotation() * M_PI / 180);
                }
            }
            else
            {
                sprite.Rotate(-2);
                direction_x = cos(sprite.GetRotation() * M_PI / 180);
                direction_y = -sin(sprite.GetRotation() * M_PI / 180);
            }
        }
    }
}

sf::Sprite Zombi::getSprite()
{
    return sprite;
}

void Zombi::afficher_data()
{
    cout << "Etat        : "<<etat<<endl;
    cout << "Vois joueur : "<<vois_joueur<<endl;
    cout << "localisation: "<<localisation_joueur<<endl;
    cout << "label_cible : "<<label_cible<<endl;
    cout << "Distance    : "<<distance<<endl;
    cout << "Dist attak  : "<<distance_attaque<<endl;
    cout << "Diff_angle  : "<<difference_angle<<endl;
    cout << "Position    : "<< sprite.GetPosition().x << "," << sprite.GetPosition().y << endl;
    cout << "Cible       : " << cible_x << "," << cible_y << endl;
    cout << "temps_dernier_coup : " << temps_dernier_coup << endl << endl;
}


void Zombi::avancer(float ElapsedTime)
{
    if(etat)
    {
        float ancienne_position_x  = sprite.GetPosition().x;
        float ancienne_position_y  = sprite.GetPosition().y;

        if(difference_angle < 20)
        {
            if(distance >= distance_attaque || vois_joueur == 0)
            {
                //sprite.Move(direction_x * vitesse * ElapsedTime, direction_y * vitesse * ElapsedTime);
                //gestion des collisions
                int x_bloc = -1;
                int y_bloc = -1;

                //deplacement en x
                if(map->collision(sprite.GetPosition().x+(direction_x * vitesse * ElapsedTime),sprite.GetPosition().y, &x_bloc, &y_bloc))
                {

                }
                else
                {
                    sprite.Move(direction_x * vitesse * ElapsedTime, 0);
                }

                //deplacement en y
                if(map->collision(sprite.GetPosition().x,sprite.GetPosition().y+(direction_y * vitesse * ElapsedTime), &x_bloc, &y_bloc))
                {

                }
                else
                {
                    sprite.Move(0, direction_y * vitesse * ElapsedTime);
                }
            }
            else
            {
                etat = 0;
            }
        }
        //si le zombie est bloqué
        if(ancienne_position_x == sprite.GetPosition().x && ancienne_position_y == sprite.GetPosition().y)
        {
            etat = 0;
        }
    }
}

void Zombi::attaquer()
{
    if(vois_joueur)
    {
        if(difference_angle < 10)
        {
            if((clock.GetElapsedTime() - temps_dernier_coup) > intervalle_coup)
            {
                if(distance <= distance_attaque+1)
                {
                    temps_dernier_coup = clock.GetElapsedTime();
                    player->subir_degat();
                }
            }
        }
    }
}


