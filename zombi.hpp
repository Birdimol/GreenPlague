#ifndef DEF_ZOMBI
#define DEF_ZOMBI

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>

#include <string>
#include "Player.hpp"
#include "Map.hpp"

class Zombi
{
    public:
    Zombi(Player *player_envoye, Map *map, int x, int y);
    void maj_donnees();
    void avancer(float distance);
    void tourner(float ElapsedTime);
    void reperer_cible();
    void attaquer();
    void afficher_data();
    sf::Sprite getSprite();
    bool detecter_joueur();
    void definir_cible();
    //void recevoirDegats(int nbDegats);
    //void attaquer(Personnage &cible);
    //bool estVivant();

    private:
    Player *player;
    Map *map;
    int rotation;
    int vie;
    float direction_x;
    float direction_y;
    int distance_attaque;
    float temps_dernier_coup;
    float intervalle_coup;
    float difference_angle;
    float distance;
    float vitesse;

    int etat;
    int vois_joueur;
    int attaque;
    float cible_x;
    float cible_y;
    std::string localisation_joueur;
    std::string label_cible;

    sf::Image image;
    sf::Sprite sprite;
    sf::Clock clock;

};

#endif

