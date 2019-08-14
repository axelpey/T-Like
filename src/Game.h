#ifndef GAME_H
#define GAME_H

#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <fstream>
#include <sstream>
#include <vector>

#include "Player.h"

class Game
{
    public:
        Game();
        virtual ~Game();

        bool start();
        bool connect();
        bool gameLoop(); ///Affichage du jeu, interaction avec le joueur

        void networkLoop(); ///Mise � jour des variables avec le serveur

		// Render
		void render();

		// Handle events
		void handleClick(sf::Event event);

		// Packet sending methods
		void sendDisconnectPacket(); // ID 0, means we're leaving.
		void sendPlayerPacket(); // ID 1, contains player
		void sendModifPacket(int x, int y, sf::Uint16 id); // ID 3, contains block modification info

		// Packet handling methods
		void handleDisconnectPacket(sf::Packet& packet); // ID 0, received when we need to disconnect
		void handlePlayersPacket(sf::Packet& packet); // ID 1, contains other players info
		void handlePlanetPacket(sf::Packet& packet); // ID 2, contains a whole planet
		void handleModifPacket(sf::Packet& packet); // ID 3, contains modif infos

        void exit();
    protected:
    private:
        bool m_running;

        ///Graphics
        sf::RenderWindow m_window;
        sf::FloatRect m_visibleArea;
        sf::View m_view;
        float m_zoom; ///Change avec la molette de la souris!

        ///Network
        sf::Thread m_networkThread;
        sf::TcpSocket m_tcpSocket;
        sf::IpAddress m_serverAdress;

        Planet m_planet;
        Player m_player;
        std::vector <Player*> m_otherPlayers;
};

#endif // GAME_H
