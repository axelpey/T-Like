#include "../include/Game.h"

using namespace std;

Game::Game() : m_window(sf::VideoMode(1280, 720), "Initialisation..."),
    m_zoom(10),
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
    cout << "Nom du joueur : ";
    cin >> name;
    cout << endl;

    m_planet = Planet(18, sf::Vector2f(50,30));
    m_player = Player(name, 1, &m_planet);

    m_window.setView(m_view);
    m_window.setVerticalSyncEnabled(true);

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
        sf::View view(visibleArea);
        view.setRotation(m_player.getAngle());
        view.setCenter(m_player.getAbsolutePosition().x, m_player.getAbsolutePosition().y);
        m_window.setView(view);

        //End display
        m_window.display();
    }
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
        int nbPlayers;
        if(m_tcpSocket.receive(packet) == sf::Socket::Done)
        {
            packet >> nbPlayers;
            m_players.clear();
            for(int i = 0; i < nbPlayers; i++)
            {
                Player player("n/a",0, &m_planet);
                packet >> player;
                m_players.push_back(player);
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
    m_window.close();
    m_running = false;
}
