#include "balle.hpp"
#include "zombiManager.hpp"
#include "player.hpp"
#include "map.hpp"
#include "jeu.hpp"

#include <sstream>

using namespace std;

Jeu::Jeu(sf::RenderWindow* App_)
{
    this->App = App_;
}

//test de modif

int Jeu::lancer()
{
    Map map(40,30);
    Player* joueur = new Player("Tom",&map);
    ZombiManager* zombiManager = new ZombiManager(joueur, &map);

    vector<Balle*> tableau_balle;
    //Balle* balle = new Balle();

    sf::Font MyFont;
    if (!MyFont.LoadFromFile("verdana.ttf", 50))

    //??????????????????????????????????????????
    sf::String Texte;
    sf::String Texte;
    sf::String Parole;

    float duree_parole = 500;
    float timer_parole = 0;

    sf::Shape Polygon;
    Polygon.AddPoint(0, 540,  sf::Color(255, 0, 0),     sf::Color(0, 0, 0));
    Polygon.AddPoint(800, 540,   sf::Color(255, 0, 0),   sf::Color(0, 0, 0));
    Polygon.AddPoint(800, 600,  sf::Color(0, 0, 0), sf::Color(0, 0, 0));
    Polygon.AddPoint(0, 600,  sf::Color(0, 0, 0), sf::Color(0, 0, 0));


    Texte.SetFont(MyFont);
    Texte.SetSize(40);
    Texte.Move(520,540);

    Parole.SetFont(MyFont);
    Parole.SetSize(16);

    sf::Image image_arme;
    sf::Sprite sprite_arme;
    image_arme.LoadFromFile("pistolet.png");
    sprite_arme.SetImage(image_arme);

    sf::Image image_coeur_vide;
    sf::Image image_coeur_plein;

    sf::Sprite sprite_coeur_vide;
    sf::Sprite sprite_coeur_plein;

    sf::Image image_balle;
    image_balle.LoadFromFile("balle.png");

    image_coeur_vide.LoadFromFile("coeur_vide.png");
    image_coeur_plein.LoadFromFile("coeur_plein.png");

    sprite_coeur_vide.SetImage(image_coeur_vide);
    sprite_coeur_plein.SetImage(image_coeur_plein);

    sprite_coeur_plein.Resize(40,30);
    sprite_coeur_vide.Resize(40,30);


    float ElapsedTime;

     // Boucle principale
    while (App->IsOpened() && joueur->get_vie() > 0 && zombiManager->get_nombre_zombis() > 0)
    {
        //zombiManager->afficher_data();

        // Detections events
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

        // Gestion du temps
        ElapsedTime = App->GetFrameTime();

        // Mouvements
        if (App->GetInput().IsKeyDown(sf::Key::Left))
        {
            joueur->tourner(200 * ElapsedTime);
        }
        if (App->GetInput().IsKeyDown(sf::Key::Right))
        {
            joueur->tourner(-200 * ElapsedTime);
        }
        if (App->GetInput().IsKeyDown(sf::Key::Up))
        {
           joueur->avancer(100 * ElapsedTime);
        }
        if (App->GetInput().IsKeyDown(sf::Key::Down))
        {
            joueur->avancer(-1 * 100 * ElapsedTime);
        }
        if (App->GetInput().IsKeyDown(sf::Key::A))
        {
            int resultat_shoot = joueur->shoot();
            if(resultat_shoot == 1)
            {
                tableau_balle.push_back(new Balle(image_balle, &map));

                tableau_balle[tableau_balle.size()-1]->set(joueur->getSprite().GetPosition().x,
                          joueur->getSprite().GetPosition().y,
                          joueur->direction_x,
                          joueur->direction_y,
                          joueur->rotation);
            }
            else if(resultat_shoot == 2)
            {
                Parole.SetText("Je n'ai pas d'arme !");
                timer_parole = duree_parole;
            }
            else if(resultat_shoot == 3)
            {
                Parole.SetText("Je n'ai plus de balles !");
                timer_parole = duree_parole;
            }
        }

        zombiManager->iteration_vie_zombi(ElapsedTime);

        int temp = tableau_balle.size();
        for(int i=0; i<temp; i++)
        {
            tableau_balle[i]->avancer(ElapsedTime);
        }

        // Clear screen
        App->Clear();

        map.afficher(App);

        //gestion de l'arme
        if(!joueur->have_arme())
        {
            int difference_joueur_arme_x = (sprite_arme.GetPosition().x+25) - joueur->getSprite().GetPosition().x;
            int difference_joueur_arme_y = (sprite_arme.GetPosition().y+16) - joueur->getSprite().GetPosition().y;

            if(difference_joueur_arme_x < 0)
            {
                difference_joueur_arme_x = difference_joueur_arme_x * -1;
            }
            if(difference_joueur_arme_y < 0)
            {
                difference_joueur_arme_y = difference_joueur_arme_y * -1;
            }

            int difference_totale_joueur_arme = sqrt((difference_joueur_arme_y*difference_joueur_arme_y)+(difference_joueur_arme_x*difference_joueur_arme_x));

            if(difference_totale_joueur_arme < 20)
            {
                joueur->get_arme();
            }
             App->Draw(sprite_arme);
        }

        // Display sprite in our window
        App->Draw(joueur->getCercle());
        App->Draw(joueur->getSprite());

        zombiManager->afficher(App);

        //gestion des balles
        for(int i=0; i<temp; i++)
        {
            if(tableau_balle[i]->statut != 0)
            {
                App->Draw(tableau_balle[i]->getSprite());
                if(zombiManager->balle_touche_zombi(tableau_balle[i]->getSprite().GetPosition().x, tableau_balle[i]->getSprite().GetPosition().y))
                {
                    tableau_balle[i]->statut = 0;
                }
            }
        }
        for(int i=0; i<temp; i++)
        {
            if(tableau_balle[i]->statut == 0)
            {
               tableau_balle.erase(tableau_balle.begin()+i);
                i--;
                temp--;
            }
        }

        //affichage des paroles
        if(timer_parole > 0)
        {
            Parole.SetPosition((joueur->getSprite().GetPosition().x - (Parole.GetRect().GetWidth()/2)),joueur->getSprite().GetPosition().y - 45);

            //éviter que le texte parte trop à gauche
            if(Parole.GetPosition().x < 0)
            {Parole.SetPosition(0,Parole.GetPosition().y);}

            //éviter que le texte parte trop à droite
            else if((Parole.GetPosition().x+Parole.GetRect().GetWidth()) > 800)
            {Parole.SetPosition((800-Parole.GetRect().GetWidth()),Parole.GetPosition().y);}

            //éviter que le texte parte trop haut
            cout << Parole.GetPosition().y << endl;
            if(Parole.GetPosition().y < 0)
            {Parole.SetPosition(Parole.GetPosition().x,joueur->getSprite().GetPosition().y + 15);}

            App->Draw(Parole);
            timer_parole = timer_parole-4;
        }

        //rectangle de l'interface
        App->Draw(Polygon);

        //affichage des coeurs
        int i;
        for(i = 0; i < joueur->get_vie(); i++)
        {
            sprite_coeur_plein.SetPosition(20+(i*50),550);
            App->Draw(sprite_coeur_plein);
        }

        for(i = joueur->get_vie(); i < 5; i++)
        {
            sprite_coeur_vide.SetPosition(20+(i*50),550);
            App->Draw(sprite_coeur_vide);
        }

        if(joueur->have_arme())
        {
            std::stringstream out;
            out << joueur->get_munition() << " munitions";
            Texte.SetText(out.str());
            App->Draw(Texte);
        }

        // Display window contents on screen
        App->Display();
    }

    // Gestion du temps
    float temoin = App->GetFrameTime();

    //On a gagné !
    if(zombiManager->get_nombre_zombis() == 0)
    {
        int zooming = 1;

        Texte.SetText("VICTOIRE");
        Texte.SetSize(60);
        Texte.SetPosition(400,200);
        Texte.SetCenter(Texte.GetRect().GetWidth()/2,Texte.GetRect().GetHeight()/2);

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

            // Display sprite in our window
            App->Draw(joueur->getCercle());
            App->Draw(joueur->getSprite());

            if(zooming == 1)
            {
                Texte.SetScale(Texte.GetScale().x+0.05,Texte.GetScale().y+0.05);
                if(Texte.GetScale().x > 2)
                {
                    zooming = 0;
                }
            }
            else
            {
                Texte.SetScale(Texte.GetScale().x-0.05,Texte.GetScale().y-0.05);
                if(Texte.GetScale().x < 0.5)
                {
                    zooming = 1;
                }
            }

            App->Draw(Texte);

            // Display window contents on screen
            App->Display();
        }
    }

    return 0;
}



