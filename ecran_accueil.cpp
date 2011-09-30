#include "balle.hpp"
#include "zombiManager.hpp"
#include "player.hpp"
#include "map.hpp"

#include "ecran_accueil.hpp"
#include "jeu.hpp"
using namespace std;

EcranAccueil::EcranAccueil(sf::RenderWindow* App_)
{
    this->App = App_;
}

int EcranAccueil::lancer()
{
    sf::Image image_ecran_accueil;
    sf::Image image_pressEnter;

    sf::Sprite sprite_ecran_accueil;
    sf::Sprite sprite_pressEnter;

    image_ecran_accueil.LoadFromFile("pictures/ecran_accueil.png");
    image_pressEnter.LoadFromFile("pictures/pressEnter.png");

    sprite_ecran_accueil.SetImage(image_ecran_accueil);
    sprite_pressEnter.SetImage(image_pressEnter);

    sprite_pressEnter.SetPosition(200,300);

    int statut = 0;

    Jeu jeu(App);

    int resultat=0;
    // Boucle principale
    while (App->IsOpened())
    {
        // Detections events
        sf::Event Event;
        while (App->GetEvent(Event))
        {
            if (Event.Type == sf::Event::Closed)
                App->Close();
        }

        // Gestion du temps
        float ElapsedTime = App->GetFrameTime();

        // Mouvements
        if (App->GetInput().IsKeyDown(sf::Key::Return))
        {
            resultat = jeu.lancer();
        }

        if (App->GetInput().IsKeyDown(sf::Key::Escape) && resultat != 2)
        {
            return 0;
        }
        // Clear screen
        App->Clear();

        if(statut == 0 && sprite_pressEnter.GetPosition().y > 300)
        {
            statut = 1;
        }
        if(statut == 1 && sprite_pressEnter.GetPosition().y < 280)
        {
            statut = 0;
        }

        if(statut == 0)
        {
            sprite_pressEnter.SetPosition(200,sprite_pressEnter.GetPosition().y+ElapsedTime*50);
        }
        else
        {
            sprite_pressEnter.SetPosition(200,sprite_pressEnter.GetPosition().y-ElapsedTime*50);
        }


        App->Draw(sprite_ecran_accueil);
        App->Draw(sprite_pressEnter);

        // Display window contents on screen
        App->Display();
    }

    return 0;
}




