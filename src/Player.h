#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <iostream>

#include "Entity.h"
#include "Planet.h"

class Player : public Entity
{
    public:
		Player();
		Player(std::string name, int r_mass, Planet* r_planet);
        virtual ~Player();

        void handleDirection(bool right,bool left,bool jump);

    private:
		float m_speed;
};

sf::Packet& operator <<(sf::Packet& packet, Player& player);

sf::Packet& operator >>(sf::Packet& packet, Player& player);

#endif // PLAYER_H
