#include "SPlanet.h"

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
    cout << "G�n�ration de la plan�te..." << endl;

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

        cout << i*100/m_circonference << "%" << endl;
    }
}

void SPlanet::loadBlocksFromVector(vector < vector <int> > blocks)
{
    ///On v�rifie la taille du vector
    if(blocks.size()!=m_circonference || blocks[0].size()!=m_height)
    {
        cout << "Blocs re�us par paquet invalides!";
        return;
    }

    m_blocks = blocks;
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
Donne la position du centre de la plan�te
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

sf::Packet& operator <<(sf::Packet& packet, SPlanet& planet)
{
    sf::Uint32 mainRadius = planet.getRadius();
    packet << mainRadius;
    packet << planet.m_position.x << planet.m_position.y;
    for(int i = 0; i < planet.getCirconference(); i++)
    {
        for(int j = 0; j < planet.m_height; j++)
        {
            sf::Uint32 blockID = planet.m_blocks[i][j];
            packet << blockID;
        }
    }

	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, SPlanet& planet)
{
    sf::Uint32 mainRadius;
    packet >> mainRadius;
    planet.m_mainRadius = mainRadius;
    sf::Vector2f position;
    packet >> position.x >> position.y;
    planet.m_position = position;
    planet.m_circonference = 2*PI*planet.m_mainRadius;
    planet.m_centerRadius = (planet.m_mainRadius/3)*2;
    planet.m_height = (planet.m_mainRadius-planet.m_centerRadius)*2;

    std::vector < std::vector <int> > blocks;
    for(int i = 0; i < planet.m_circonference; i++)
    {
        std::vector <int> column;
        for(int j = 0; j < planet.m_height; j++)
        {
            sf::Uint32 blockID;
            packet >> blockID;
            column.push_back(blockID);
        }
        blocks.push_back(column);
    }
    cout << "Avant loadFromVector" << endl;
    planet.loadBlocksFromVector(blocks);
    return packet;
}
