#include "zombi.hpp"


using namespace std;

Zombi::Zombi(Map *map_envoyee, int x, int y)
{
    map = map_envoyee;
    if (image.LoadFromFile("pictures/zombi.png"))
    {
        sprite.SetImage(image);

        sprite.SetPosition(x, y);
        sprite.SetCenter(20.f, 20.f);

        distance_attaque = 30;
        temps_dernier_coup = 0;
        intervalle_coup = 1;

        localisation_joueur = "";
        label_cible = "";
        vois_joueur = 0;

        clock.Reset();

        etat = 0;
        attaque=0;
        vitesse = 80;

        direction_x = 1;
        direction_y = 0;

        rotation = 0;

        cible = 0;

        wait_for_path = false;
    }
}

sf::Image Zombi::getImage()
{
    return image;
}

void Zombi::set_cible_du_zombie(int x, int y)
{
    cible_x.clear();
    cible_y.clear();
    cible_x.push_back(x);
    cible_y.push_back(y);
}

void Zombi::set_wait_for_path(Point destination)
{
    wait_for_path = true;
    destination_memorisee = destination;
}

void Zombi::stop_wait_for_path()
{
    wait_for_path = false;
}

Point Zombi::get_destination_memorisee()
{
    return destination_memorisee;
}

bool Zombi::is_waiting_for_path()
{
    return wait_for_path;
}

void Zombi::add_cible_du_zombie(int x, int y)
{
    cible_x.push_back(x);
    cible_y.push_back(y);
}

bool Zombi::est_dans_rectangle(int xMousePressed,int yMousePressed,int xMouseReleased,int yMouseReleased)
{
    int temp;

    if(xMousePressed > xMouseReleased)
    {
        temp = xMousePressed;
        xMousePressed = xMouseReleased;
        xMouseReleased = temp;
    }

    if(yMousePressed > yMouseReleased)
    {
        temp = yMousePressed;
        yMousePressed = yMouseReleased;
        yMouseReleased = temp;
    }

    if(((sprite.GetPosition().x-image.GetWidth()/2 > xMousePressed) && (sprite.GetPosition().x-image.GetWidth()/2 > xMouseReleased)) || ((sprite.GetPosition().x+image.GetWidth()/2 < xMousePressed) && (sprite.GetPosition().x+image.GetWidth()/2 < xMouseReleased)))
    {
        cible = false;
        return false;
    }
    else if(((sprite.GetPosition().y-image.GetHeight()/2 > yMousePressed) && (sprite.GetPosition().y-image.GetHeight()/2 > yMouseReleased)) || ((sprite.GetPosition().y+image.GetHeight()/2 < yMousePressed) && (sprite.GetPosition().y+image.GetHeight()/2 < yMouseReleased)))
    {
        cible = false;
        return false;
    }
    else
    {
        cible = true;
        return true;
    }
}

void Zombi::tourner(float ElapsedTime)
{
    //Si on a une cible
    if(cible_x.size() != 0)
    {
        //on calcule la distance en x et y avec la destination
        float difference_x = sprite.GetPosition().x - cible_x[cible_x.size()-1];
        float difference_y = sprite.GetPosition().y - cible_y[cible_y.size()-1];

        /*if((difference_x > 10 || difference_x < -10) || (difference_y > 10 || difference_y < -10))
        {*/
            //on calcule l'angle ideal à avoir pour avancer vers la cible.
            float angle_parfait = (atan(-(difference_y)/difference_x) / M_PI * 180);

            //on calcule la distance reelle avec la destination
            distance = sqrt((difference_x*difference_x) + (difference_y*difference_y));

            //ajustement de l'angle;
            while(angle_parfait < 360)
            {
                angle_parfait += 360;
            }

            //ajustement de l'angle;
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

            //on regarde la difference entre l'angle du zombie et l'angle que l'on veut
            difference_angle = (angle_parfait - sprite.GetRotation());

            //on ajuste cette donnee
            if(difference_angle < 0)
            {
                difference_angle = difference_angle * -1;
            }
            //on ajuste cette donnee
            if(difference_angle > 180)
            {
                difference_angle = 360-difference_angle;
            }

            //si on est à moins de 10 pixels de la cible, l'état passe à zero
            if(distance < 10)
            {
                etat = 0;
            }

            //si le zombie n'est pas encore dans la bonne direction
            if(difference_angle > 2)
            {
                //si l'on est trop loin pour attaquer ou que l'on ne voit pas le joueur
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
        //}
    }
}

sf::Sprite Zombi::getSprite()
{
    return sprite;
}

bool Zombi::getCible()
{
    return cible;
}

Point Zombi::get_final_destination()
{
    Point temp(-1,-1);

    //si il n'y a pas de destination
    if(cible_x.size() == 0)
    {
        return temp;
    }

    temp.x = floor(cible_x[0]/map->getTailleCase());
    temp.y = floor(cible_y[0]/map->getTailleCase());

    return temp;
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
    cout << "Nombre de cible : " << cible_x.size() << endl;
    if(cible_x.size() > 0)
    {
        cout << "Cible       : " << cible_x[cible_x.size()-1] << "," << cible_y[cible_y.size()-1] << endl;
    }
    cout << "temps_dernier_coup : " << temps_dernier_coup << endl << endl;
}

Point Zombi::get_position()
{
    Point position;
    position.x = floor(sprite.GetPosition().x/map->getTailleCase());
    position.y = floor(sprite.GetPosition().y/map->getTailleCase());
    return position;
}

void Zombi::avancer(float ElapsedTime)
{
    //si on a une destination
    if(cible_x.size() != 0)
    {
        float difference_x = sprite.GetPosition().x - cible_x[cible_x.size()-1];
        float difference_y = sprite.GetPosition().y - cible_y[cible_y.size()-1];

        //si on est a plus de 10 pixels de la cible en x et Y
        if((difference_x > 5 || difference_x < -5) || (difference_y > 5 || difference_y < -5))
        {

            float ancienne_position_x  = sprite.GetPosition().x;
            float ancienne_position_y  = sprite.GetPosition().y;

            //on va avancer meme si l'angle n'est pas parfait mais presque
            if(difference_angle < 20)
            {
                //si on ne voit pas le joueur ou qu'il est trop loin
                if(distance >= distance_attaque || vois_joueur == 0)
                {
                    //gestion des collisions
                    //int x_bloc = -1;
                    //int y_bloc = -1;

                    sprite.Move(direction_x * vitesse * ElapsedTime, 0);
                    sprite.Move(0, direction_y * vitesse * ElapsedTime);

                    /*
                    //deplacement en x
                    if(map->collision(sprite.GetPosition().x+(direction_x * vitesse * ElapsedTime),sprite.GetPosition().y, &x_bloc, &y_bloc))
                    {
                        cout << "collision en X avec "<< x_bloc << ", " << y_bloc << endl;
                    }
                    else
                    {

                    }

                    //deplacement en y
                    if(map->collision(sprite.GetPosition().x,sprite.GetPosition().y+(direction_y * vitesse * ElapsedTime), &x_bloc, &y_bloc))
                    {
                        cout << "collision en Y avec"<< x_bloc << ", " << y_bloc << endl;
                    }
                    else
                    {

                    }
                    */
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
        else
        {
            cible_x.pop_back();
            cible_y.pop_back();
        }
    }
}

float Zombi::getRotation()
{
    return rotation;
}


