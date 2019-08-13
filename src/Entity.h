#ifndef ENTITY_H
#define ENTITY_H

#include "Planet.h"

#define PI 3.14159265

// For now, an entity is just a rectangle 1x2 that is supposed to move around the planet
// and follow physics rules.

class Entity
{
	public:
		Entity();
		Entity(std::string name, int r_mass, Planet* r_planet);
		virtual ~Entity();

		sf::Vector2f relativePos2absolute(sf::Vector2f relCoord);

		void playPhysics(int fps);

		void setRelativePos(sf::Vector2f const& pos);
		void setColor(sf::Color const& color);
		void setName(std::string const& name);

		sf::Vector2f getRelativePosition();
		sf::Vector2f getAbsolutePosition();
		float getAngle();
		sf::Color getColor();
		std::string getName();

		void render(sf::RenderWindow*);

	protected:
		std::string m_name;
		Planet* planet;
		bool onGround;
		sf::Vector2f absolutePos;
		sf::Vector2f relativePos; //x = position autour de la planete (en degres) y = hauteur
		sf::Vector2f relativeSpeed;
		float m_mass;
		sf::Color m_color;
		sf::RectangleShape rectangle; //Le perso est juste un rectangle

	private:
		
};

#endif ENTITY_H