#include "case.hpp"

Case::Case()
{

}

Case::Case(float h, float g)
{
    H=h;
    G=g;
    F = H+G;
}

void Case::initialise(float h, float g)
{
    H=h;
    G=g;
    F = H+G;
}

void Case::set_parent(int x, int y)
{
   parent.x = x;
   parent.y = y;
}
