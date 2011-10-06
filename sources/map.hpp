#ifndef DEF_MAP
#define DEF_MAP

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

#include "point.hpp"

using namespace std;

class Map
{
    public:
    Map(string fichier);
    void afficher(sf::RenderWindow* App);
    bool traverse_mur(float x1,float y1,float x2,float y2);
    int collision(float x_cercle,float y_cercle,int *x_bloc,int *y_bloc);
    bool pointdansmur(float x, float y);
    int getL();
    int getH();
    int getTailleCase();
    bool getErrorHappened();
    vector<int> get_tableau_cases();
    int strToInt(string chaine);
    vector<Point> get_tableau_zombie();
    bool estMur(Point case_);
    Point getCaseLibreProche(Point point, vector<Point> liste_case_occupee);
    vector<Point> get_liste_case_occupee();
    void add_liste_case_occupee(Point a);
    void delete_liste_case_occupee(Point a);

    private:
    vector<int> tableau_cases;
    vector<Point> tableau_zombie;
    int largeur;
    int hauteur;
    sf::Image idalle;
    sf::Image imur;
    vector<sf::Sprite> sdalle;
    vector<Point> liste_case_occupee;
    int taille_case;
    bool error_happened;
};

#endif
