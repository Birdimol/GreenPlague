#ifndef DEF_CASE
#define DEF_CASE

#include "point.hpp"

class Case
{
    public :
    float F;
    float G;
    float H;
    Point parent;

    Case();
    Case(float h, float g);
    void initialise(float h, float g);
    void set_parent(int x, int y);

};

#endif

