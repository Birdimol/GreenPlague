#ifndef DEF_PLAYER
#define DEF_PLAYER

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>

#include <string>

#include "map.hpp"

class Player
{
    public:
    Player(std::string nom, Map* map);

    void avancer(float distance);
    void reculer(float distance);
    void tourner(float angle);
    int shoot();
    sf::Sprite getSprite();
    sf::Sprite getCercle();
    float direction_x;
    float direction_y;
    int rotation;
    int get_vie();
    void get_arme();
    bool have_arme();
    int get_munition();
    void add_munition(int nombre);
    void subir_degat();
    //void recevoirDegats(int nbDegats);
    //void attaquer(Personnage &cible);
    //bool estVivant();

    private:
    int vie;
    bool arme;
    int munitions;
    std::string nom;
    sf::Image image;
    sf::Image image_shoot;
    sf::Image image_cercle;
    sf::Sprite sprite;
    sf::Sprite sprite_shoot;
    sf::Sprite sprite_cercle;
    sf::Clock clock;
    int statut;
    Map *map;
    float intervalle_coup;
    float temps_dernier_coup;
};

#endif
