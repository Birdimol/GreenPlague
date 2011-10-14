#ifndef DEF_JEU
#define DEF_JEU

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>

#include <vector>

using namespace std;

class Jeu
{
    public:
    Jeu(sf::RenderWindow* App);
    int lancer();

    private:
    sf::Image im_interface;
    sf::Image im_interface_zombie;
    sf::Image im_interface_zombies;
    sf::Image im_icone_go;
    sf::Image im_icone_look;
    sf::Image im_icone_go_selected;
    sf::Image im_icone_look_selected;
    sf::RenderWindow* App;

};

#endif
