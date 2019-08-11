#ifndef PLANET_H
#define PLANET_H

#include "Server/SPlanet.h"

#include <SFML/Graphics.hpp>

class Planet : public SPlanet
{
    public:
        Planet();
        Planet(int mainRadius, sf::Vector2f position);
        virtual ~Planet();

        void render(sf::RenderWindow*);

        sf::CircleShape getCenterShape();
        float getMass();
    protected:

    private:
        sf::CircleShape centerShape;
        float m_mass;
};

#endif // PLANET_H
