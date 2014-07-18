#include "../include/Game.h"

using namespace std;

Game::Game() : m_window(sf::VideoMode(1280, 720), "Initialisation..."),
    m_zoom(20),
    m_visibleArea(0, 0, m_window.getSize().x/m_zoom, m_window.getSize().y/m_zoom),
    m_view(m_visibleArea),
    m_running(false),
    m_networkThread(&Game::networkLoop,this)
{
    string ip;
    string name;

    cout << "Adresse du serveur : ";
    cin >> ip;
    cout << endl;

    ifstream ifs("data/player.txt");
    if(ifs)
    {
        ifs >> name;
    }
    else
    {
        cout << "ifs failed" << endl;
        ofstream ofs("data/player.txt");
        if(ofs)
            ofs << "default";
        else
            cout << "ofs failed" << endl;
        ofs.close();
        name = "default";
    }
    ifs.close();

    m_planet = Planet(18, sf::Vector2f(0,0));
    m_player = Player(name, 1, &m_planet);

    m_window.setView(m_view);
    m_window.setFramerateLimit(60);

    m_serverAdress = sf::IpAddress(ip);
}

Game::~Game()
{
    //dtor
}

bool Game::start() ///Démarrage du jeu
{
    m_running = true;

    ///Eventuel menu?

    if(!connect())
        return false;

    m_networkThread.launch(); ///Dans un thread, ce qui permet de modifier les variables pendant l'exécution du jeu.

    gameLoop();

    m_networkThread.wait();

    return true;
}

bool Game::connect()
{
    m_window.setTitle("Connecting");

    if(m_tcpSocket.connect(m_serverAdress,54002,sf::seconds(5)) != sf::Socket::Done)
    {
        cout << "Serveur non existant. Le programme va se fermer...";
        sf::sleep(sf::seconds(3));
        return false;
    }

    m_window.setTitle("Connected to " + m_tcpSocket.getRemoteAddress().toString()) ;

    return true;
}

bool Game::gameLoop()
{
    bool rightPressed = false;
    bool leftPressed = false;
    int fps;
    sf::Clock clock;

    while (m_running)
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            switch(event.type)
            {
///----------------------------------------------Events fenetre------------------------------------------------------------------------------------------///
            case sf::Event::Closed:
                exit();
                break;

            case sf::Event::LostFocus:
                //Pause
                break;

            case sf::Event::GainedFocus:
                //Resume
                break;

///--------------------------------------------Events KeyPressed------------------------------------------------------------------------------------------///

            case sf::Event::KeyPressed:
                switch(event.key.code)
                {
                case sf::Keyboard::Escape:
                    exit();
                    break;

                case sf::Keyboard::Right:
                    rightPressed = true;
                    break;

                case sf::Keyboard::Left:
                    leftPressed = true;
                    break;
                }
                break;

///------------------------------------------Events KeyReleased-------------------------------------------------------------------------------------------///

            case sf::Event::KeyReleased:
                switch (event.key.code)
                {
                case sf::Keyboard::Right:
                    rightPressed = false;
                    break;

                case sf::Keyboard::Left:
                    leftPressed = false;
                    break;
                }
                break;

///----------------------------------------------Events souris-------------------------------------------------------------------------------------------///

            case sf::Event::MouseButtonPressed:
                if(event.mouseButton.button == sf::Mouse::Right)
                {
                    /*
                     Pour plus tard
                     Récupérer position dans le monde puis la planète concernée.
                    */

                    //On détermine l'emplacement du clic par rapport au centre de la planète
                    sf::Vector2f realPosition;
                    realPosition.x = event.mouseButton.x/m_zoom;
                    realPosition.y = event.mouseButton.y/m_zoom;
                    sf::Vector2f upperleftcorner;
                    upperleftcorner.x = m_view.getCenter().x - m_view.getSize().x/2;
                    upperleftcorner.y = m_view.getCenter().y - m_view.getSize().y/2;
                    // -upperleftcorner.x correspond à la coordonnée x de la planète sur l'écran puisque la planète est en 0,0

                    //Distance entre la position de clic de la souris et le centre de la planète
                    float distance = sqrt(pow(realPosition.x+upperleftcorner.x,2) + pow(realPosition.y+upperleftcorner.y,2));
                    float height = distance-(m_planet.getRadius()*2)/3;
                    float adjacent = realPosition.x + upperleftcorner.x;
                    float oppose = realPosition.y + upperleftcorner.y;
                    float angle;
                    if(adjacent >= 0)
                        angle = 90 + atan(oppose/adjacent)/PI*180;
                    else
                        angle = 270 + atan(oppose/adjacent)/PI*180;

                    int xint = (int)height;
                    int yint = (angle/360) * m_planet.getCirconference();

                    cout << "upperleft x=" << upperleftcorner.x << endl;
                    cout << "upperleft y=" << upperleftcorner.y << endl;
                    cout << "distance = " << distance << endl;
                    cout << "height = " << height << endl;
                    cout << "angle = " << angle << endl;
                    cout << "x=" << xint << endl;
                    cout << "y=" << yint << endl;

                    m_planet.setBlock(sf::Vector2i(yint,xint),0);

                    //Bouton droit cliqué.
                }
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    //Bouton gauche cliqué.

                    sf::Vector2f realPosition;
                    realPosition.x = event.mouseButton.x/m_zoom;
                    realPosition.y = event.mouseButton.y/m_zoom;

                    float distance = sqrt(pow(realPosition.x,2) + pow(realPosition.y,2));
                    float height = distance-(m_planet.getRadius()*2)/3;
                    float angle = 90 + atan(realPosition.y/realPosition.x)/PI*180;

                    int xint = (int)height;
                    int yint = (angle/360) * m_planet.getCirconference();

                    cout << "x=" << xint << endl;
                    cout << "y=" << yint << endl;

                    m_planet.setBlock(sf::Vector2i(yint,xint),2);
                }
                break;

///--------------------------------------Restes----------------------------------------------------------------------------------------------------------///
            case sf::Event::Resized:
                // on met à jour la vue, avec la nouvelle taille de la fenêtre

                break;
            }
        }

///--------------------------------------Keyboard--------------------------------------------------------------------------------------------------------///

        if(rightPressed)
        {
            m_player.goRight();
        }

        if(leftPressed)
        {
            m_player.goLeft();
        }

///--------------------------------------Render----------------------------------------------------------------------------------------------------------///



        m_window.clear();

        //Render planet
        m_planet.render(&m_window);
        m_player.render(&m_window);

        for(int i = 0; i < m_players.size(); i++)
        {
            Player &player = m_players[i];
            player.render(&m_window);
        }

        sf::FloatRect visibleArea(0, 0, m_window.getSize().x/m_zoom, m_window.getSize().y/m_zoom);
        m_view = sf::View(visibleArea);
        m_view.setRotation(m_player.getAngle());
        m_view.setCenter(m_player.getAbsolutePosition().x, m_player.getAbsolutePosition().y);
        m_window.setView(m_view);

///-------------------------------------------Affichage des FPS------------------------------------------------------------------------------------------///

        fps++;
        if(clock.getElapsedTime().asSeconds() >= 1)
        {
            stringstream ss;
            ss << fps;
            string str_fps = ss.str();
            m_window.setTitle("Connected to " + m_tcpSocket.getRemoteAddress().toString() + ". FPS = " + str_fps);
            fps = 0;
            clock.restart();
        }

        //End display
        m_window.display();
    }

    m_window.close();
}

void Game::networkLoop()
{
    while(m_running)
    {
        ///Envoi de la position
        sf::Packet ownPacket;
        ownPacket << sf::Uint32(1); //Id du paquet
        ownPacket << m_player;

        if(m_tcpSocket.send(ownPacket) != sf::Socket::Done)
        {
            cout << "Erreur lors de l'envoi du paquet" << endl;
        }

        ///Réception des infos du serveur
        sf::Packet packet;
        if(m_tcpSocket.receive(packet) == sf::Socket::Done)
        {
            sf::Uint32 id;
            packet >> id;
            string reason;
            switch(id)
            {
            case 0:
                packet >> reason;
                //Serveur shutdown
                cout << "Vous avez été déconnecté du serveur. Raison : " << reason << endl;
                exit();
                break;

            case 1:
                sf::Uint32 nbPlayers;
                packet >> nbPlayers;
                m_players.clear();
                for(int i = 0; i < nbPlayers; i++)
                {
                    Player player("n/a",0, &m_planet);
                    packet >> player;
                    m_players.push_back(player);
                }
                break;
            }
        }
        else
        {}
    }
    //On envoie au serveur qu'on se déconnecte

    sf::Packet discoPacket;
    discoPacket << sf::Uint32(0);
    if(m_tcpSocket.send(discoPacket) != sf::Socket::Done)
    {
        cout << "Erreur lors de l'envoi du paquet de déconnexion" << endl;
    }

    m_tcpSocket.disconnect();
}

void Game::exit()
{
    m_running = false;
}
