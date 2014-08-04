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
    cout << "Chargement de la planète..." << endl;

    m_planet = SPlanet(18,sf::Vector2f(0,0));

    cout << "Démarrage du serveur sur le port " << m_port << endl;

    m_mainThread.launch();

    command();
}

void Server::main()
{
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

                    ///Envoyer informations sur la planète
                    sf::Packet planetPacket;
                    planetPacket << sf::Uint32(2); // ID Packet
                    planetPacket << m_planet;
                    client->send(planetPacket);
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
                        sf::Packet packet;

                        if(client.receive(packet) == sf::Socket::Done)
                        {
                            receiveTCP(packet, i);
                        }
                    }
                }
            }
        }
    }
}

void Server::command()
{
    cin.ignore();
    while(m_run)
    {
        string input = "";
        string command;
        string arg;

        getline(cin, input);

        if(input.find(" ")!=string::npos)
        {
            command = input.substr(0,input.find((" ")));
            arg = input.substr(input.find(" ")+1);
        }
        else
        {
            command = input;
        }

        if(command == "stop")
        {
            cout << "Arret du serveur" << endl;
            stop();
        }
        else if(command == "infos")
        {
            cout << "Infos sur les joueurs : " << endl;

            for(int i = 0; i < players.size(); i++)
            {
                SPlayer& player = *players[i];
                cout << " - Player " << i+1 << " avec le nom " << player.getName() << " est à la position x=" << player.getPosition().x << " et y=" << player.getPosition().y
                << " avec la couleur " << player.getColor().r << "," << player.getColor().g << "," << player.getColor().b << endl;
            }
        }
        else if(command == "kick")
        {
            if(arg == "")
            {
                cout << "Aucun nom de joueur renseigné." << endl;
            }
            for(int i = 0; i < players.size(); i++)
            {
                SPlayer& player = *players[i];
                if(player.getName() == arg)
                {
                    kick(i);
                }
            }
        }
        else if(command == "help")
        {

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

    case 1:
        sf::TcpSocket& client = *clients[clientid];

        SPlayer& player = *players[clientid];

        packet >> player;

        //On prépare le packet à envoyer au client
        sf::Packet packet;
        packet << sf::Uint32(1); //ID Packet
        sf::Uint32 nbPlayers;
        nbPlayers = players.size()-1;
        packet << nbPlayers; //
        for(int j = 0; j < players.size(); j++)
        {
            if(j!=clientid) // On envoie les infos des autres joueurs sauf celle du joueur qui va les recevoir.
            {
                SPlayer &player = *players[j];
                packet << player;
            }
        }

        //On envoie le packet
        client.send(packet);
        break;
    }
}

void Server::stop()
{
    //Message d'arrêt sur le chat

    for(int i = 0; i < clients.size(); --i)
    {
        sf::TcpSocket& socket = *clients[0];
        sf::Packet srvSHUTDOWNPacket;
        srvSHUTDOWNPacket << sf::Uint32(0); //ID packet qui demande la déconnexion du client.
        srvSHUTDOWNPacket << string("Extinction du serveur.");
        socket.send(srvSHUTDOWNPacket);
    }

    m_run = false;
}

bool Server::isRunning()
{
    return m_run;
}

void Server::kick(int const& clientid)
{
    sf::TcpSocket* socket = clients[clientid];
    sf::Packet kickPacket;
    kickPacket << sf::Uint32(0);
    kickPacket << string("Vous avez été expulsé du serveur!");
    socket->send(kickPacket);
}

void Server::disconnect(int const& clientid)
{
    cout << "Player " << clientid << " is disconnecting!" << endl;

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
