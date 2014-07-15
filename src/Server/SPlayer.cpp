#include "../../include/Server/SPlayer.h"

SPlayer::SPlayer()
{

}

SPlayer::~SPlayer()
{
    //dtor
}

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
