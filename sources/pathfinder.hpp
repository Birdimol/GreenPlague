#ifndef DEF_PATHFINDER
#define DEF_PATHFINDER

#include <iostream>
#include <vector>
#include <math.h>
#include <SFML/Graphics.hpp>

#include "point.hpp"
#include "case.hpp"
#include "map.hpp"

using namespace std;

class Pathfinder
{
    private :
    vector<int> carte;
    int largeur;
    int hauteur;
    int taille_case;
    vector<Case> carte_distances;


    float distance_entre_2_point(Point a,Point b);
    bool existe_pas_dans_vector(int x, int y,vector<Point> points_deja_verifies);
    bool traverse_mur(float x1,float y1,float x2,float y2);

    public :
    Pathfinder(vector<int> carte, int largeur, int hauteur, int taille_case, Map* map);
    Map* map;
    //Envoyer les cases !
    vector<Point> calcul_path(Point depart,Point arrivee);
    bool contient(vector<Point> tableau,int x,int y);
    void afficher_parcourt(vector<int> carte_vector, int largeur_carte, int hauteur_carte, Point depart, Point arrivee, vector<Point> parcourt);

};

#endif
