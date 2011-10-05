#ifndef DEF_TOOLS
#define DEF_TOOLS

class Tools
{
    public :
    static bool contient_point(vector<Point> tableau,Point key)
    {
        int size = tableau.size();
        for(int i=0;i<size;i++)
        {
            cout << "comparaison " << tableau[i].x  << ", " << tableau[i].y <<" avec " << key.x  << ", " << key.y << endl;
            if(tableau[i].x == key.x && tableau[i].y == key.y)
            {
                return true;
            }
        }
        return false;
    }
};

#endif
