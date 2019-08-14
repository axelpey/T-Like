#ifndef PLANET_H
#define PLANET_H

#include "Server/SPlanet.h"

#include <SFML/Graphics.hpp>

class Planet : public SPlanet
{
    public:
        Planet(int mainRadius, sf::Vector2f position);
        virtual ~Planet();

		// Returns the next position of a moving entity, considering it is flying
		// if there is nothing to collide with when moving from pos1 to pos2, then returns pos2
		// if there is, returns the position the entity will have
		// The bool value will be true if the entity has touched ground
		// said entity is a player (2 units height, 1 unit width)
		std::pair<sf::Vector2f,bool> collidingBlocks(sf::Vector2f pos1, sf::Vector2f pos2);

        void render(sf::RenderWindow*);

        sf::CircleShape getCenterShape();
        float getMass();
    protected:

    private:
        sf::CircleShape centerShape;
        float m_mass;
};

#endif // PLANET_H
