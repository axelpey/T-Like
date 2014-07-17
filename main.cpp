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
  - Récupération de l'adresse du serveur sur mon serveur perso ovh.
  - Changer le zoom sur la fenêtre grâce à la molette de la souris.
  - Créer un simple chat en bas à gauche / Canal de packet : un nouveau TCP.
  - Cliquer sur un endroit pour créer une nouvelle brique :
   - Récupérer position absolue de la souris sur l'écran.
   - La transformer en position relative par rapport au centre de la planète (inverse de relativePos2absolute())
   - Déterminer quel bloc est concerné par cette position relative via : sf::Vector2f getBlock(relativepos) où le vecteur représente sa position dans le tableau des blocs de la planète.
   - Agir en conséquence : Destruction ou placement de bloc.
  - Optimiser le rendu graphique (notammment des planètes).
  - Gestion des collisions.
  - Capacité de sauter.
  - Informations sur la planète gardée par le serveur. Le serveur renvoie un paquet dès que la planète est modifiée indiquant quel bloc a été modifié.
  - Ecran de chargement au début dû au fait qu'il faut charger depuis le serveur les infos sur la planète.
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
