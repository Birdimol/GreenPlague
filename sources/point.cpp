#include "point.hpp"

Point::Point(int a, int b)
{
    x=a;
    y=b;
}

void Point::afficher()
{
    cout << x << ", " << y << endl;
}

Point::Point()
{

}
