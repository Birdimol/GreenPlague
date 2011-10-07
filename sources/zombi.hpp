#ifndef DEF_ZOMBI
#define DEF_ZOMBI

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>

#include <string>
#include "player.hpp"
#include "map.hpp"

class Zombi
{
    public:
    Zombi(Map *map, int x, int y);
    void maj_donnees();
    void avancer(float distance);
    void tourner(float ElapsedTime);
    void reperer_cible();
    void attaquer();
    void afficher_data();
    sf::Sprite getSprite();
    Point get_position();
    sf::Image getImage();
    bool detecter_joueur();
    void definir_cible();
    bool est_dans_rectangle(int xMousePressed,int yMousePressed,int xMouseReleased,int yMouseReleased);
    bool getCible();
    void set_cible_du_zombie(int x, int y);
    void add_cible_du_zombie(int x, int y);
    Point get_final_destination();
    bool is_waiting_for_path();
    void stop_wait_for_path();
    void set_wait_for_path(Point destination);
    Point get_destination_memorisee();

    //void recevoirDegats(int nbDegats);
    //void attaquer(Personnage &cible);
    //bool estVivant();

    private:
    bool wait_for_path;
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
    Point destination_memorisee;

    bool cible;
    int etat;
    int vois_joueur;
    int attaque;
    vector<float> cible_x;
    vector<float> cible_y;
    std::string localisation_joueur;
    std::string label_cible;

    sf::Image image;
    sf::Sprite sprite;
    sf::Clock clock;

};

#endif

