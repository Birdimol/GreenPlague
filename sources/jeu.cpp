#include "balle.hpp"
#include "zombiManager.hpp"
#include "player.hpp"
#include "map.hpp"
#include "jeu.hpp"
#include "pathfinder.hpp"

#include <limits>

#include <sstream>

using namespace std;

Jeu::Jeu(sf::RenderWindow* App_)
{
    this->App = App_;
}

int Jeu::lancer()
{
    float ElapsedTime;

    sf::Vector2f Center(400, 300);
    sf::Vector2f HalfSize(400, 300);
    sf::View View1(Center, HalfSize);

    sf::Color background(0,0,0,0);
    sf::Color border(0,200,0,255);

    //variables pour le cadre de selection
    sf::Shape rectangle_selection   = sf::Shape::Rectangle(
        0,
		0,
		200,
		200,
		background,
		1,
		border);

    int xMousePressed = -1;
    int yMousePressed = -1;

    int xMousePressed_temp = -1;
    int yMousePressed_temp = -1;

    int xMouseReleased = -1;
    int yMouseReleased = -1;

    int xMouse = -1;
    int yMouse = -1;

    bool mouse_selection = false;

    Map map("map01");
    if(map.getErrorHappened())
    {
        App->Close();
    }

    Pathfinder pathfinder(map.get_tableau_cases(), map.getL(), map.getH(), 40, &map);

    ZombiManager* zombiManager = new ZombiManager(&map);

    sf::Font MyFont;
    MyFont.LoadFromFile("fonts/verdana.ttf", 50);

     // Boucle principale
    while (App->IsOpened())
    {
        //zombiManager->afficher_data();
        App->Clear();

        App->SetView(View1);

        map.reset_visible();

        zombiManager->set_visibility();

        map.afficher(App);

        // Detections events
        sf::Event Event;
        while (App->GetEvent(Event))
        {
            if (Event.Type == sf::Event::Closed)
                App->Close();

            if (Event.Type == sf::Event::MouseMoved)
            {
                xMouse = Event.MouseMove.X;
                yMouse = Event.MouseMove.Y;
            }

            if (Event.Type == sf::Event::MouseButtonPressed)
            {
                if(Event.MouseButton.Button == 0)
                {
                    mouse_selection = true;

                    xMousePressed = xMouse;
                    yMousePressed = yMouse;

                    xMousePressed_temp = xMouse;
                    yMousePressed_temp = yMouse;
                }
                else if(Event.MouseButton.Button == 1)
                {
                    mouse_selection = false;

                    xMousePressed = xMouse;
                    yMousePressed = yMouse;

                    zombiManager->set_destination_to_active_zombies(xMousePressed+View1.GetRect().Left,yMousePressed+View1.GetRect().Top);

                }
            }
            if (Event.Type == sf::Event::MouseButtonReleased && Event.MouseButton.Button == 0)
            {
                mouse_selection = false;

                xMouseReleased = Event.MouseButton.X;
                yMouseReleased = Event.MouseButton.Y;
            }

        }

        if (App->GetInput().IsKeyDown(sf::Key::Escape))
        {
            return 2;
        }

        if (App->GetInput().IsKeyDown(sf::Key::Right))
        {
            if(View1.GetRect().Right+10 <= map.getL()*map.getTailleCase())
            {
                View1.Move(10.0f,0);
            }
        }

        if (App->GetInput().IsKeyDown(sf::Key::Left))
        {
            if(View1.GetRect().Left-10 >= 0)
            {
                View1.Move(-10.0f,0);
            }
        }
        if (App->GetInput().IsKeyDown(sf::Key::Up))
        {
            if(View1.GetRect().Top-10 >= 0)
            {
                View1.Move(0,-10.0f);
            }
        }

        if (App->GetInput().IsKeyDown(sf::Key::Down))
        {
            if(View1.GetRect().Bottom+10 <= map.getH()*map.getTailleCase())
            {
                View1.Move(0,10.0f);
            }
        }


        // Gestion du temps
         ElapsedTime = App->GetFrameTime();

        // Mouvements
        if (App->GetInput().IsKeyDown(sf::Key::Left))
        {

        }
        if (App->GetInput().IsKeyDown(sf::Key::Right))
        {

        }
        if (App->GetInput().IsKeyDown(sf::Key::Up))
        {

        }
        if (App->GetInput().IsKeyDown(sf::Key::Down))
        {

        }

        if(xMousePressed != -1 && xMouseReleased != -1)
        {
            zombiManager->detecte_selection(xMousePressed+View1.GetRect().Left,yMousePressed+View1.GetRect().Top,xMouseReleased+View1.GetRect().Left,yMouseReleased+View1.GetRect().Top);
            xMousePressed  = -1;
            yMousePressed  = -1;
            xMouseReleased = -1;
            yMouseReleased = -1;
        }

        zombiManager->iteration_vie_zombi(ElapsedTime);

        zombiManager->start_movement_for_zombies(&pathfinder);

        // Clear screen
        zombiManager->afficher(App);

        if(mouse_selection)
        {
            rectangle_selection = sf::Shape::Rectangle(xMousePressed_temp+View1.GetRect().Left,yMousePressed_temp+View1.GetRect().Top,xMouse+View1.GetRect().Left,yMouse+View1.GetRect().Top,background,2,border);
            App->Draw(rectangle_selection);
        }

        // Display window contents on screen
        App->Display();
    }

    // Gestion du temps
    float temoin = App->GetFrameTime();

    //On a gagné !
    if(zombiManager->get_nombre_zombis() == 0)
    {
        while(temoin + 2 > ElapsedTime)
        {
            sf::Event Event;
            while (App->GetEvent(Event))
            {
                if (Event.Type == sf::Event::Closed)
                    App->Close();
            }

            if (App->GetInput().IsKeyDown(sf::Key::Escape))
            {
                return 2;
            }

            ElapsedTime += App->GetFrameTime();

            sf::Color couleur(0,0,0);
            App->Clear(couleur);

            // Display window contents on screen
            App->Display();
        }
    }

    return 0;
}



