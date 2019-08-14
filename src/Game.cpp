#include "Game.h"

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
    m_player = Player(name, 60, &m_planet);

    m_window.setView(m_view);
    m_window.setFramerateLimit(60);

    m_serverAdress = sf::IpAddress(ip);
}

void emptyPlayerList(vector<Player*> playerList)
{
	for (int i = 0; i < playerList.size(); i++)
	{
		delete playerList[i];
	}
	playerList.clear();
}

Game::~Game()
{
	emptyPlayerList(m_otherPlayers);
}

bool Game::start() ///D�marrage du jeu
{
    m_running = true;

    ///Eventuel menu?

    if(!connect())
        return false;

    m_networkThread.launch(); ///Dans un thread, ce qui permet de modifier les variables pendant l'ex�cution du jeu.

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
    bool spacePressed = false;
    bool newSpacePressed = false;
    int fps = 0;
	int averagefps = 12;
    sf::Clock clock;

    sf::Music music;
    if(music.openFromFile("data/music.ogg"))
    {
        music.play();
    }
    else
    {
        cout << "Musique non trouv�e. Pas de lecture de musique." << endl;
    }



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
                
                case sf::Keyboard::Space:
                    if(!spacePressed) {
                        newSpacePressed = true;
                    }
                    spacePressed = true;
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
                
                case sf::Keyboard::Space:
                    spacePressed = false;
                    break;
                }
                break;

///----------------------------------------------Events souris-------------------------------------------------------------------------------------------///

            case sf::Event::MouseButtonPressed:
			{
				//Determine where we clicked in the game
				sf::Vector2f realPosition = m_window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

				//Determine relative position on the planet of the block we clicked
				float adjacent = realPosition.x;
				float oppose = realPosition.y;
				float distance = sqrt(pow(realPosition.x, 2) + pow(realPosition.y, 2));
				float height = distance - (m_planet.getRadius() * 2) / 3;
				float angle;
				if (adjacent >= 0)
					angle = 90 + atan(oppose / adjacent) / PI * 180;
				else
					angle = 270 + atan(oppose / adjacent) / PI * 180;

				int x = (angle / 360) * m_planet.getCirconference();
				int y = (int)height;

				if (event.mouseButton.button == sf::Mouse::Right)
				{
					m_planet.setBlock(sf::Vector2i(x, y), 0);
					sendModifPacket(x, y, 0);
				}

				if (event.mouseButton.button == sf::Mouse::Left)
				{
					m_planet.setBlock(sf::Vector2i(x, y), 2);
					sendModifPacket(x, y, 2);
				}

				break;
			}

///--------------------------------------Restes----------------------------------------------------------------------------------------------------------///
            case sf::Event::Resized:
                // on met � jour la vue, avec la nouvelle taille de la fen�tre

                break;

			default:
				break;
            }
        }

///--------------------------------------Keyboard--------------------------------------------------------------------------------------------------------///

        m_player.handleDirection(rightPressed,leftPressed,newSpacePressed);

///-------------------------------------Physics----------------------------------------------------------------------------------------------------------///

        m_player.playPhysics(averagefps);

		// No need to run physics for other players

///--------------------------------------Render----------------------------------------------------------------------------------------------------------///



        m_window.clear();

        //Render planet
        m_planet.render(&m_window);
		m_player.render(&m_window);

        for(int i = 0; i < m_otherPlayers.size(); i++)
        {
            Player* player = m_otherPlayers[i];
            player->render(&m_window);
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
			averagefps = fps;
			fps = 0;
            clock.restart();
        }

        //End display
        m_window.display();
        newSpacePressed = false;
    }

    m_window.close();

	return true;
}

void Game::networkLoop()
{
    while(m_running)
    {
        ///Envoi de la position
		sendPlayerPacket();

        ///R�ception des infos du serveur
        sf::Packet packet;
        if(m_tcpSocket.receive(packet) == sf::Socket::Done)
        {
            sf::Uint8 id;
            packet >> id;
            switch(id)
            {
            case 0:
				handleDisconnectPacket(packet);
                break;

            case 1:
				handlePlayersPacket(packet);
				break;

            case 2:
				handlePlanetPacket(packet);
                break;

            case 3:
				handleModifPacket(packet);
				break;

            default:
                cout << "Packet re�u non identifiable. Votre version du jeu n'est pas � jour avec celle du serveur." << endl;
                cout << "ID du paquet : " << id << endl;
                break;
            }
        }
        else
        {}
    }
    //On envoie au serveur qu'on se d�connecte

	sendDisconnectPacket();

    m_tcpSocket.disconnect();
}

void Game::exit()
{
    m_running = false;
	emptyPlayerList(m_otherPlayers);
}

// -------------------- Packets methods - SEND

void Game::sendDisconnectPacket()
{
	sf::Packet discoPacket;
	discoPacket << sf::Uint8(0);
	if (m_tcpSocket.send(discoPacket) != sf::Socket::Done)
	{
		cout << "Erreur lors de l'envoi du paquet de d�connexion" << endl;
	}
}

void Game::sendPlayerPacket()
{
	sf::Packet ownPacket;
	ownPacket << sf::Uint8(1); //Id du paquet
	ownPacket << m_player;

	if (m_tcpSocket.send(ownPacket) != sf::Socket::Done)
	{
		cout << "Erreur lors de l'envoi du paquet" << endl;
	}
}

void Game::sendModifPacket(int x, int y, sf::Uint16 id)
{
	sf::Packet modifPacket;
	modifPacket << sf::Uint8(3); //Id Packet
	modifPacket << sf::Uint32(x) << sf::Uint32(y) << sf::Uint16(id);
	m_tcpSocket.send(modifPacket);
}

 // ------------------ Packet methods - RECEIVE

void Game::handleDisconnectPacket(sf::Packet& packet)
{
	string reason;
	packet >> reason;
	//Serveur shutdown
	cout << "Vous avez �t� d�connect� du serveur. Raison : " << reason << endl;
	exit();
}

void Game::handlePlayersPacket(sf::Packet& packet)
{
	sf::Uint8 nbPlayers;
	packet >> nbPlayers;
	if (nbPlayers != m_otherPlayers.size())
	{
		emptyPlayerList(m_otherPlayers);
		for (int i = 0; i < nbPlayers; i++)
		{
			Player player("n/a", 0, &m_planet);
			packet >> player;
			m_otherPlayers.push_back(&player);
		}
	}
	else
	{
		for (int i = 0; i < nbPlayers; i++)
		{
			packet >> *m_otherPlayers[i];
		}
	}
}

void Game::handlePlanetPacket(sf::Packet& packet)
{
	packet >> m_planet;
}

void Game::handleModifPacket(sf::Packet& packet)
{
	sf::Vector2i position;
	packet >> position.x >> position.y;
	sf::Uint16 blockID;
	packet >> blockID;
	cout << "Paquet 3 : " << position.x << "," << position.y << " devient " << blockID << endl;
	m_planet.setBlock(position, blockID);
}

