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
    sf::RenderWindow* App;

};

#endif
