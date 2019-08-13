#ifndef SERVER_H
#define SERVER_H

#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include <iostream>
#include <vector>
#include <list>

#include "SPlayer.h"
#include "SPlanet.h"

/**
Dans le serveur :
 - une liste des joueurs
 - une liste des planètes
 - une liste des clients
**/

class Server
{
    public:
        Server(int port);
        virtual ~Server();

        void launch();
        void main();
        void stop();
        bool isRunning();
        void command();

        /**

         Plans : Différentes méthodes pour recevoir différents packets. Doivent être les mêmes côté client;

         Ids des packets server->clients:
          - TCP 0 : Quand reçu, déconnecte le client qui l'a envoyé avec un message contenant la raison de la déconnexion.
          - TCP 1 : Packet contenant les informations sur les joueurs tels que leur nombre, leur nom, leur couleur, leur position.
          - TCP 2 : Packet contenant tous les blocs de la planète de base.
          - TCP 3 : Packet informant qu'un bloc a été modifié et que les clients doivent prendre cela en compte. Suit la réception d'un TCP 3.

         Ids des packets client->server:
          - TCP 0 : Informe de la déconnexion du client.
          - TCP 1 : Packet donnant les informations sur le joueur. Envoyé constamment au serveur, il indique également que le client est toujours connecté.
          - TCP 3 : Packet informant que le client a modifié un bloc et que le serveur doit prendre cela en compte. Est suivi de l'envoi d'un TCP 3.

         Les méthodes receive sont associées aux différents id de paquets, et renvoient true quand tout s'est bien passé.
        */

        bool receiveUDP(sf::Packet& packet, int const& clientid);
        bool receiveTCP(sf::Packet& packet, int const& clientid);

		void handleDisconnectionPacket(sf::Packet& packet, int const& clientid);
		void handlePlayerPacket(sf::Packet& packet, int const& clientid);
		void handleModifPacket(sf::Packet& packet, int const& clientid);

		void sendDisconnectionPacket(int const& clientid, std::string reason);
		void sendPlayersPacket(int const& clientid);
		void sendPlanetPacket(int const& clientid);
		void broadcastModifPacket(int const& clientid, int const& x, int const& y, sf::Uint16 blockID);

        void kick(int const& clientid);
    protected:
    private:
        SPlanet m_planet;
        sf::Thread m_mainThread;
        bool m_run;
        std::vector < sf::TcpSocket* > clients;
        std::vector < SPlayer* > players;
        int m_port;
};

#endif // SERVER_H
