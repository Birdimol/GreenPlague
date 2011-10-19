
#ifndef DEF_HUMAN
#define DEF_HUMAN

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>

#include <string>

#include "map.hpp"

class Human
{
    public:
    Human(std::string nom, Map* map);

    void avancer(float distance);
    void reculer(float distance);
    void tourner(float angle);

    sf::Sprite getSprite();
    sf::Sprite getCercle();
    float direction_x;
    float direction_y;
    int rotation;


    private:
    int vie;
    sf::Image image;
    sf::Sprite sprite;

    int statut;
    Map *map;

};

#endif
