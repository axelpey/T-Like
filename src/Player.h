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
        Player(std::string name, int r_mass, Planet* r_planet);
        virtual ~Player();

        void render(sf::RenderWindow*);

        void handleDirection(bool right,bool left,bool jump);
        void playPhysics(int fps);

        void setRelativePos(sf::Vector2f const& pos);
        void setColor(sf::Color const& color);
        void setName(std::string const& name);

        sf::Vector2f getRelativePosition();
        sf::Vector2f getAbsolutePosition();
        float getAngle();
        sf::Color getColor();
        std::string getName();
    protected:
        sf::Vector2f relativePos2absolute(sf::Vector2f relCoord);
		sf::Vector2f relativeSpeed2absolute(sf::Vector2f relCoord);
        sf::Vector2f absolutePos2relative(sf::Vector2f absCoord);
		sf::Vector2f absoluteSpeed2relative(sf::Vector2f absCoord);
    private:
        std::string m_name;
        Planet* planet; 
        bool onGround;
        sf::Vector2f absolutePos;
        sf::Vector2f relativePos; //x = position autour de la planete (en degres) y = hauteur
        sf::Vector2f absoluteSpeed;
        sf::Vector2f relativeSpeed;
        float m_mass;
        float m_speed;
        sf::Color m_color;
        sf::RectangleShape rectangle; //Le perso est juste un rectangle
};

sf::Packet& operator <<(sf::Packet& packet, Player& player);

sf::Packet& operator >>(sf::Packet& packet, Player& player);

#endif // PLAYER_H
