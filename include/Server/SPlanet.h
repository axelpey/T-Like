#ifndef SPLANET_H
#define SPLANET_H

#define PI 3.14159265

#include <SFML/System.hpp>
#include <vector>
#include <cmath>

class SPlanet
{
    public:
        SPlanet();
        SPlanet(int mainRadius, sf::Vector2f position);
        virtual ~SPlanet();

        int getRadius();
        sf::Vector2f getCenterPoint();
        int getCirconference();

        void setBlock(sf::Vector2i position, int const& blockid);
        int getBlock(sf::Vector2i position);
    protected:
        //Méthodes protected
        void generatePlanet();

        //Attributs
        int m_circonference;
        int m_height;
        int m_centerRadius;
        int m_mainRadius;///Rayon de la planète en blocs

        sf::Vector2f m_position;
        std::vector < std::vector <int> > m_blocks;

    private:
};

#endif // SPLANET_H
