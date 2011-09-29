#ifndef DEF_ZOMBIMANAGER
#define DEF_ZOMBIMANAGER

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>

#include <string>
#include "zombi.hpp"
#include "map.hpp"

#include <vector>

using namespace std;

class ZombiManager
{
    public:
    ZombiManager(Player *player,Map *map);
    void afficher(sf::RenderWindow* App);
    void afficher_data();
    void iteration_vie_zombi(float ElapsedTime);
    bool balle_touche_zombi(float x, float y);
    int get_nombre_zombis();

    private:
    Player *player;
    vector<Zombi*> tableau_zombi;
    Map* map;
};

#endif
