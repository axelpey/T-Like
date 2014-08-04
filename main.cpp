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

/**
 Block ids :
 - 0 : Air
 - 1 : Dirt
 - 2 : Stone
**/

#define SERVER 1


/**
 TODO :
  - Récupération de l'adresse du serveur sur mon serveur perso ovh.
  - Changer le zoom sur la fenêtre grâce à la molette de la souris.
  - Créer un simple chat en bas à gauche / Canal de packet : un nouveau TCP.
  - Gestion des collisions.
  - Capacité de sauter.
  - Informations sur la planète gardée par le serveur. Le serveur renvoie un paquet dès que la planète est modifiée indiquant quel bloc a été modifié.
  - Ecran de chargement au début dû au fait qu'il faut charger depuis le serveur les infos sur la planète.
  - Le déplacement du personnage doit être indépendant de sa position relative à planète.
  - Sauvegarde et chargement de la planète.
*/

using namespace std;

void server(int port);

int main()
{
    srand(time(NULL));

    sf::Thread serverThread(&server,54002);

    if(SERVER==2)
    {
        cout << "Seul le serveur a été allumé" << endl;
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
