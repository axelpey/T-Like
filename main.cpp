#include <iostream>
#include <thread>
#include <string>

#include "include/Game.h"
#include "include/Block.h"
#include "include/Planet.h"
#include "include/Player.h"
#include "include/Server/Server.h"

/**
 0 - Client only
 1 - Client & Server
 2 - Server only
**/

#define SERVER 1


/**
 TODO :
  - Serveur qui contient les noms des joueurs.
  - R�cup�ration de l'adresse du serveur sur mon serveur perso ovh.
  - Changer le zoom sur la fen�tre gr�ce � la molette de la souris.
  - Afficher le nom du joueur au dessus de son "personnage".
  - Cr�er un simple chat en bas � gauche / Canal de packet : un nouveau TCP.
*/

using namespace std;

void server(int port);

int main()
{
    srand(time(NULL));

    sf::Thread serverThread(&server,54002);

    if(SERVER==2)
    {
        cout << "Seul le serveur a �t� allum�" << endl;
        serverThread.launch();
        serverThread.wait();
        return 0;
    }

    Game instance;

    if(SERVER==1)
    {
        serverThread.launch();
    }

    if(!instance.start())
        return EXIT_FAILURE;

    serverThread.wait();

    return 0;
}

void server(int port)
{
    Server server(port);

    server.launch();

    return;
}
