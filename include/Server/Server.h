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

         Ids des packets server->client:
          - TCP 0 : Quand re�u, d�connecte le client qui l'a envoy� avec un message contenant la raison de la d�connexion.
          - TCP 1 : Packet contenant les informations sur les joueurs tels que leur nombre, leur nom, leur couleur, leur position.
          - TCP 2 : Packet contenant tous les blocs de la plan�te de base.

         Les m�thodes receive sont associ�es aux diff�rents id de paquets, et renvoient true quand tout s'est bien pass�.
        */

        bool receiveUDP(sf::Packet& packet, int const& clientid);
        bool receiveTCP(sf::Packet& packet, int const& clientid);

        void kick(int const& clientid);
        void disconnect(int const& clientid);
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
