#include "balle.hpp"
#include "zombiManager.hpp"
#include "human_manager.hpp"
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

    im_interface.LoadFromFile("pictures/interface.png");
    im_interface_zombie.LoadFromFile("pictures/icone_zombie.png");
    im_interface_zombies.LoadFromFile("pictures/icone_zombies.png");
    im_icone_go.LoadFromFile("pictures/icone_go.png");
    im_icone_look.LoadFromFile("pictures/icone_look.png");
    im_icone_go_selected.LoadFromFile("pictures/icone_go_selected.png");
    im_icone_look_selected.LoadFromFile("pictures/icone_look_selected.png");
    sf::Sprite s_interface;
    sf::Sprite s_interface_zombie;
    sf::Sprite s_interface_zombies;
    sf::Sprite s_icone_go;
    sf::Sprite s_icone_go_selected;
    sf::Sprite s_icone_look;
    sf::Sprite s_icone_look_selected;
    s_interface.SetImage(im_interface);
    s_interface_zombie.SetImage(im_interface_zombie);
    s_interface_zombies.SetImage(im_interface_zombies);
    s_icone_go.SetImage(im_icone_go);
    s_icone_go_selected.SetImage(im_icone_go_selected);
    s_icone_look.SetImage(im_icone_look);
    s_icone_look_selected.SetImage(im_icone_look_selected);
    s_interface.SetPosition(0,0);
    s_interface_zombie.SetPosition(0,520);
    s_interface_zombies.SetPosition(0,520);

    s_icone_go.SetPosition(83,528);
    s_icone_go_selected.SetPosition(83,528);
    s_icone_look.SetPosition(83,563);
    s_icone_look_selected.SetPosition(83,563);

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

    bool go = 1;
    bool look = 0;

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

        map.afficher(App,View1.GetRect());

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

            //gestion du click dans le jeu
            if(yMouse < 520)
            {
                if (Event.Type == sf::Event::MouseButtonPressed)
                {
                    if(look)
                    {
                        //click gauche
                        if(Event.MouseButton.Button == 0)
                        {
                            go = true;
                            look=false;
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

                            zombiManager->set_direction_to_active_zombies(xMousePressed+View1.GetRect().Left,yMousePressed+View1.GetRect().Top);
                        }
                    }
                    else if(go)
                    {
                        //click dans l'interface
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
                }
                if (Event.Type == sf::Event::MouseButtonReleased && Event.MouseButton.Button == 0)
                {
                    mouse_selection = false;

                    xMouseReleased = Event.MouseButton.X;
                    yMouseReleased = Event.MouseButton.Y;
                }

            }
            else
            {
                if (Event.Type == sf::Event::MouseButtonPressed)
                {
                    //gestion du clic dans l'interface
                    if(xMouse >= 83 && xMouse <=113 && yMouse >= 528 && yMouse <= 558)
                    {
                        //click look
                        look = false;
                        go = true;
                    }
                    else if(xMouse >= 83 && xMouse <=113 && yMouse >= 563 && yMouse <= 593)
                    {
                        //click go
                        look = true;
                        go = false;
                    }
                }
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

        //interface
        App->SetView(App->GetDefaultView());

        App->Draw(s_interface);
        if(zombiManager->get_nbre_zombie_selectionne() == 1)
        {
            App->Draw(s_interface_zombie);
            if(go)
            {
                App->Draw(s_icone_go_selected);
            }
            else
            {
                App->Draw(s_icone_go);
            }

            App->Draw(s_icone_look);
            if(look)
            {
                App->Draw(s_icone_look_selected);
            }
            else
            {
                App->Draw(s_icone_look);
            }
        }
        else if(zombiManager->get_nbre_zombie_selectionne()> 1)
        {
            App->Draw(s_interface_zombies);
            if(go)
            {
                App->Draw(s_icone_go_selected);
            }
            else
            {
                App->Draw(s_icone_go);
            }

            App->Draw(s_icone_look);
            if(look)
            {
                App->Draw(s_icone_look_selected);
            }
            else
            {
                App->Draw(s_icone_look);
            }
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



