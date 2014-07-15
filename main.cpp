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
  - Packets avec int ID dans le server et dans le client.
  - Adresse du serveur et nom du personnage dans un fichier texte adresse.txt
  - Changer le zoom sur la fenêtre grâce à la molette de la souris.
  - Afficher le nom du joueur au dessus de son "personnage".
  - Créer un simple chat en bas à gauche / Canal de packet : un nouveau TCP.
*/

void server(int port);

int main()
{
    srand(time(NULL));

    sf::Thread serverThread(&server,54002);

    if(SERVER==2)
    {
        std::cout << "Seul le serveur a été allumé" << std::endl;
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

    return 0;
}

void server(int port)
{
    Server server(port);

    server.launch();

    return;
}
