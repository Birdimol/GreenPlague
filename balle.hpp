#ifndef DEF_BALLE
#define DEF_BALLE

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>

#include <string>
#include "map.hpp"

class Balle
{
    public:
    Balle(sf::Image image,Map *map);
    void set(float x_envoye, float y_envoye, float direction_x_envoye, float direction_y_envoye, float rotation_envoye);
    void avancer(float time);
    sf::Sprite getSprite();
    int statut;

    private:
    float direction_x;
    float direction_y;
    float rotation;
    float x;
    float y;
    Map *map;
    sf::Image image;
    sf::Sprite sprite;
};

#endif
