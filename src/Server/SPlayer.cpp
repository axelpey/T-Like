#include "SPlayer.h"

using namespace std;

SPlayer::SPlayer()
{}

SPlayer::~SPlayer()
{}

void SPlayer::setPosition(int x, int y)
{
    setPosition(sf::Vector2f(x,y));
}

void SPlayer::setPosition(sf::Vector2f pos)
{
    relativePos = pos;
}

sf::Vector2f SPlayer::getPosition()
{
    return relativePos;
}

sf::Color SPlayer::getColor()
{
    return m_color;
}

void SPlayer::setColor(sf::Color color)
{
    m_color = color;
}

void SPlayer::setName(string name)
{
    m_name = name;
}

string SPlayer::getName()
{
    return m_name;
}

sf::Packet& operator <<(sf::Packet& packet, SPlayer& player)
{
    return packet << player.getName() << player.getPosition().x << player.getPosition().y << player.getColor().r << player.getColor().g << player.getColor().b;
}

sf::Packet& operator >>(sf::Packet& packet, SPlayer& player)
{
    string name;
    packet >> name;
    player.setName(name);
    float x;
    float y;
    packet >> x >> y;
    player.setPosition(x,y);
    sf::Uint8 r;
    sf::Uint8 g;
    sf::Uint8 b;
    packet >> r >> g >> b;
    sf::Color color = sf::Color(r,g,b);
    player.setColor(color);
    return packet;
}
