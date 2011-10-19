
#ifndef DEF_HUMANMANAGER
#define DEF_HUMANMANAGER

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>

#include <string>
#include "human.hpp"
#include "pathfinder.hpp"
#include "map.hpp"

#include <vector>

using namespace std;

class HumanManager
{
    public:
    HumanManager(Map *map);
    void afficher(sf::RenderWindow* App);
    void afficher_data();
    void iteration_vie_human(float ElapsedTime);
    int  get_nombre_human();

    private:
    vector<Human*> tableau_human;
    Map* map;
    sf::Image image;
    sf::Sprite sprite;
};

#endif
