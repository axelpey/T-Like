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
 - une liste des plan�tes
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

         Plans : Diff�rentes m�thodes pour recevoir diff�rents packets. Doivent �tre les m�mes c�t� client;

         Ids des packets server->clients:
          - TCP 0 : Quand re�u, d�connecte le client qui l'a envoy� avec un message contenant la raison de la d�connexion.
          - TCP 1 : Packet contenant les informations sur les joueurs tels que leur nombre, leur nom, leur couleur, leur position.
          - TCP 2 : Packet contenant tous les blocs de la plan�te de base.
          - TCP 3 : Packet informant qu'un bloc a �t� modifi� et que les clients doivent prendre cela en compte. Suit la r�ception d'un TCP 3.

         Ids des packets client->server:
          - TCP 0 : Informe de la d�connexion du client.
          - TCP 1 : Packet donnant les informations sur le joueur. Envoy� constamment au serveur, il indique �galement que le client est toujours connect�.
          - TCP 3 : Packet informant que le client a modifi� un bloc et que le serveur doit prendre cela en compte. Est suivi de l'envoi d'un TCP 3.

         Les m�thodes receive sont associ�es aux diff�rents id de paquets, et renvoient true quand tout s'est bien pass�.
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
