#ifndef GAME_H
#define GAME_H

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <fstream>

#include "Player.h"

class Game
{
    public:
        Game();
        virtual ~Game();

        bool start();
        bool connect();
        bool gameLoop(); ///Affichage du jeu, interaction avec le joueur
        void networkLoop(); ///Mise à jour des variables avec le serveur

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
        std::vector <Player> m_players;
};

#endif // GAME_H
