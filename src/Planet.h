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
    protected:

    private:
        sf::CircleShape centerShape;
};

#endif // PLANET_H
