#ifndef SPLAYER_H
#define SPLAYER_H

#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

/**
 SPlayer for server-side class player.
*/
class SPlayer
{
    public:
        SPlayer();
        virtual ~SPlayer();

        void setPosition(sf::Vector2f pos);
        void setPosition(int x, int y);
        sf::Vector2f getPosition();
        sf::Color getColor();
        void setColor(sf::Color color);
    protected:
    private:
        sf::Color m_color;
        sf::Vector2f relativePos;
        //SPlanet* planet;
};

#endif // SPLAYER_H
