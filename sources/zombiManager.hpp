#ifndef DEF_ZOMBIMANAGER
#define DEF_ZOMBIMANAGER

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>

#include <string>
#include "zombi.hpp"
#include "pathfinder.hpp"
#include "map.hpp"

#include <vector>

using namespace std;

class ZombiManager
{
    public:
    ZombiManager(Map *map);
    void detecte_selection(int xMousePressed,int yMousePressed,int xMouseReleased,int yMouseReleased);
    void afficher(sf::RenderWindow* App);
    void afficher_data();
    void iteration_vie_zombi(float ElapsedTime);
    bool balle_touche_zombi(float x, float y);
    int get_nombre_zombis();
    void set_destination_to_active_zombies(int xMousePressed,int yMousePressed);
    void start_movement_for_zombies(Pathfinder* pathfinder);

    private:
    vector<Zombi*> tableau_zombi;
    Map* map;
    sf::Image image_cible;
    sf::Sprite sprite_cible;
};

#endif
