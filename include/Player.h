#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <iostream>

#include "Planet.h"

#define PI 3.14159265

class Player
{
    public:
        Player();
        Player(std::string name, int r_weight, Planet* r_planet);
        virtual ~Player();

        void render(sf::RenderWindow*);

        void goLeft();
        void goRight();

        void setRelativePos(sf::Vector2f const& pos);
        void setColor(sf::Color const& color);
        void setName(std::string const& name);

        sf::Vector2f getRelativePosition();
        sf::Vector2f getAbsolutePosition();
        int getAngle();
        sf::Color getColor();
        std::string getName();
    protected:
        sf::Vector2f relativePos2absolute();
    private:
        std::string m_name;
        Planet* planet;
        sf::Vector2f absolutePos;
        sf::Vector2f relativePos; //x = position autour de la planète (en degrés) y = hauteur
        int weight;
        sf::Color m_color;
        sf::RectangleShape rectangle; //Le perso est juste un rectangle
};

sf::Packet& operator <<(sf::Packet& packet, Player& player);

sf::Packet& operator >>(sf::Packet& packet, Player& player);

#endif // PLAYER_H
