#include "../../include/Server/SPlanet.h"

using namespace std;

SPlanet::SPlanet()
{
    //ctor
}

SPlanet::~SPlanet()
{
    //dtor
}

SPlanet::SPlanet(int mainRadius, sf::Vector2f position)
{
    m_mainRadius = mainRadius;
    m_position = position;
    m_circonference = 2*PI*m_mainRadius;
    m_centerRadius = (m_mainRadius/3)*2;
    m_height = (m_mainRadius-m_centerRadius)*2;

    m_blocks.clear();
    generatePlanet();
}

void SPlanet::generatePlanet()
{
    m_blocks = vector < vector < int > >(m_circonference, vector < int > (m_height, 0));

    for(int i = 0; i < m_circonference; i++)
    {
        vector < int > column;

        for(int j = 0; j < m_height; j++)
        {
            if(j<(m_height/2))
                column.push_back(2);
            if(j==(m_height/2))
                column.push_back(1);
            if(j>(m_height/2))
                column.push_back(0);
        }

        m_blocks[i] = column;
    }
}

void SPlanet::setBlock(sf::Vector2i position, int const& blockid)
{
    m_blocks[position.x][position.y] = blockid;
}

int SPlanet::getBlock(sf::Vector2i position)
{
    return m_blocks[position.x][position.y];
}

/**
Donne la position du centre de la planète
*/
sf::Vector2f SPlanet::getCenterPoint()
{
    return m_position;
}

int SPlanet::getCirconference()
{
    return m_circonference;
}

int SPlanet::getRadius()
{
    return m_mainRadius;
}
