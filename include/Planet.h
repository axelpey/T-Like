#ifndef PLANET_H
#define PLANET_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

#define PI 3.14159265

class Planet
{
    public:
        Planet();
        Planet(int r_mainRadius, sf::Vector2f r_position);
        virtual ~Planet();

        void render(sf::RenderWindow*);

        sf::CircleShape getCenterShape();
        int getRadius();
        sf::Vector2f getCenterPoint();

    protected:
        void generatePlanet();

    private:
        int mainRadius;///Rayon de la planète en blocs
        int centerRadius;
        int height;
        int circonference;

        sf::CircleShape centerShape;
        sf::Vector2f position;

        std::vector < std::vector <int> > blocks;
};

#endif // PLANET_H
