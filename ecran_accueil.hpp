#ifndef DEF_ECRANACCUEIL
#define DEF_ECRANACCUEIL

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>

#include <vector>

using namespace std;

class EcranAccueil
{
    public:
    EcranAccueil(sf::RenderWindow* App);
    int lancer();

    private:
    sf::RenderWindow* App;

};

#endif

