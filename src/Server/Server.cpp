#include "../../include/Server/Server.h"

using namespace std;

Server::Server(int port) : m_mainThread(&Server::main,this)
{
    m_port = port;
    m_run = 1;
    players.empty();
}

Server::~Server()
{
    //dtor
}

void Server::launch()
{
    cout << "Démarrage du serveur sur le port " << m_port << endl;

    m_mainThread.launch();

    command();
}

void Server::main()
{
    float x = 0;
    float y = 0;

    sf::SocketSelector selector;
    sf::TcpListener listener;
    listener.listen(m_port);
    selector.add(listener);

    while (isRunning())
    {
        if(selector.wait())
        {
            if(selector.isReady(listener)) ///Nouvelle connexion entrante!
            {
                sf::TcpSocket* client = new sf::TcpSocket;

                if(listener.accept(*client) == sf::Socket::Done)
                {
                    clients.push_back(client);

                    selector.add(*client);

                    cout << "Client connected! From " << client->getRemoteAddress() << ":" << client->getRemotePort() << endl;

                    //On crée directement un nouveau SPlayer
                    SPlayer* newPlayer = new SPlayer();

                    players.push_back(newPlayer);
                }
                else
                {
                    delete client;
                }
            }
            else ///On échange des paquets avec les clients déjà connectés
            {
                for(int i = 0; i < clients.size(); ++i)
                {
                    sf::TcpSocket& client = *clients[i];

                    if(selector.isReady(client))
                    {
                        sf::Packet posPacket;

                        if(client.receive(posPacket) == sf::Socket::Done)
                        {
                            sf::Color color;
                            posPacket >> x >> y >> color.r >> color.g >> color.b;

                            SPlayer& player = *players[i];

                            player.setPosition(x,y);
                            player.setColor(color);
                        }

                        //On prépare le packet à envoyer au client
                        sf::Packet packet;
                        packet << players.size()-1; //
                        for(int j = 0; j < players.size(); j++)
                        {
                            if(j!=i) // On envoie les infos des autres joueurs sauf celle du joueur qui va les recevoir.
                            {
                                SPlayer &player = *players[j];
                                float x = player.getPosition().x;
                                float y = player.getPosition().y;
                                sf::Color color = player.getColor();
                                packet << x << y << color.r << color.g << color.b;
                            }
                        }

                        //On envoie le packet
                        client.send(packet);
                    }
                }
            }
        }
    }
}

void Server::command()
{
    while(m_run)
    {
        string command = "";

        cin >> command;

        if(command == "stop")
        {
            cout << "Arret du serveur" << endl;
            stop();
        }
        else if(command == "infos")
        {
            cout << "Positions des joueurs : " << endl;

            for(int i = 0; i < players.size(); i++)
            {
                SPlayer& player = *players[i];
                cout << " - Player " << i+1 << " est à la position x=" << player.getPosition().x << " et y=" << player.getPosition().y
                << " avec la couleur " << player.getColor().r << "," << player.getColor().g << "," << player.getColor().b << endl;
            }
        }
        else
        {
            cout << "Commande non valide" << endl;
        }
    }
}

bool Server::receiveUDP(sf::Packet& packet, int const& clientid)
{

}

bool Server::receiveTCP(sf::Packet& packet, int const& clientid)
{
    sf::Uint32 id;
    packet >> id;

    switch(id)
    {
    case 0: //Packet qui... ?
        disconnect(clientid);
        break;
    }
}

void Server::stop()
{
    m_run = false;
}

bool Server::isRunning()
{
    return m_run;
}

void Server::disconnect(int const& clientid)
{
    //On déconnecte le client et on détruit le socket et le joueur (il gardera sa place dans la liste des ids néanmoins)
    sf::TcpSocket* socket = clients[clientid];
    socket->disconnect();
    delete socket;
    auto it = clients.begin() + clientid - 1; //Iterateur sur le client dont l'id est clientid.
    clients.erase(it);

    SPlayer* player = players[clientid];
    delete player;
    auto it2 = players.begin() + clientid - 1;
    players.erase(it2);
}
