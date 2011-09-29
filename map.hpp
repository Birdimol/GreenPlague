#ifndef DEF_MAP
#define DEF_MAP

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>

#include <vector>

using namespace std;

class Map
{
    public:
    Map(int largeur, int hauteur);
    void afficher(sf::RenderWindow* App);
    bool traverse_mur(float x1,float y1,float x2,float y2);
    int collision(float x_cercle,float y_cercle,int *x_bloc,int *y_bloc);
    bool pointdansmur(float x, float y);
    int getL();
    int getH();

    private:
    vector<int> tableau_cases;
    int largeur;
    int hauteur;
    sf::Image idalle;
    sf::Image imur;
    vector<sf::Sprite> sdalle;
};

#endif
