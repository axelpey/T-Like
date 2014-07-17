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
  - R�cup�ration de l'adresse du serveur sur mon serveur perso ovh.
  - Changer le zoom sur la fen�tre gr�ce � la molette de la souris.
  - Cr�er un simple chat en bas � gauche / Canal de packet : un nouveau TCP.
  - Cliquer sur un endroit pour cr�er une nouvelle brique :
   - R�cup�rer position absolue de la souris sur l'�cran.
   - La transformer en position relative par rapport au centre de la plan�te (inverse de relativePos2absolute())
   - D�terminer quel bloc est concern� par cette position relative via : sf::Vector2f getBlock(relativepos) o� le vecteur repr�sente sa position dans le tableau des blocs de la plan�te.
   - Agir en cons�quence : Destruction ou placement de bloc.
  - Optimiser le rendu graphique (notammment des plan�tes).
  - Gestion des collisions.
  - Capacit� de sauter.
  - Informations sur la plan�te gard�e par le serveur. Le serveur renvoie un paquet d�s que la plan�te est modifi�e indiquant quel bloc a �t� modifi�.
  - Ecran de chargement au d�but d� au fait qu'il faut charger depuis le serveur les infos sur la plan�te.
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
