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
        void setColor(sf::Color color);
        void setName(std::string name);

        sf::Vector2f getPosition();
        sf::Color getColor();
        std::string getName();
    protected:
    private:
        std::string m_name;
        sf::Color m_color;
        sf::Vector2f relativePos;
        //SPlanet* planet;
};

sf::Packet& operator <<(sf::Packet& packet, SPlayer& player);

sf::Packet& operator >>(sf::Packet& packet, SPlayer& player);

#endif // SPLAYER_H
