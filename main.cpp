

#define M_PI  3.14159265358979323846

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
/*
#include "Player.hpp"
#include "zombi.hpp"
#include "zombiManager.hpp"
#include "balle.hpp"
#include "map.hpp"
#include "jeu.hpp"
*/

#include "ecran_accueil.hpp"

using namespace std;

int main()
{
    // Create the main rendering window
    sf::RenderWindow App(sf::VideoMode(800, 600, 32), "SFML Graphics");
    App.SetFramerateLimit(60);

    EcranAccueil ecranAccueil(&App);

    ecranAccueil.lancer();

    return EXIT_SUCCESS;
}

