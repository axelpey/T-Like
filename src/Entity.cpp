#include "Entity.h"

using namespace std;

constexpr float G = 0.01;

Entity::Entity()
{

}

Entity::Entity(std::string name, int r_mass, Planet* r_planet)
{
	m_name = name;
	onGround = true;
	rectangle = sf::RectangleShape(sf::Vector2f(1, 2));
	char r = rand() % 255;
	char g = rand() % 255;
	char b = rand() % 255;
	m_color = sf::Color(r, g, b);
	rectangle.setFillColor(m_color);

	m_mass = r_mass;
	planet = r_planet;
	relativePos = sf::Vector2f(0, planet->getRadius() / 3 + 1); ///Debug
}

Entity::~Entity()
{
	//dtor
}

sf::Vector2f Entity::relativePos2absolute(sf::Vector2f relCoord)
{
	sf::Vector2f absCoord;
	absCoord.y = (planet->getCenterPoint().y) - sinf((relCoord.x * PI / 180) + PI / 2) * (planet->getCenterShape().getRadius() + relCoord.y + rectangle.getSize().y);
	absCoord.x = (planet->getCenterPoint().x) + cosf((relCoord.x * PI / 180) - PI / 2) * (planet->getCenterShape().getRadius() + relCoord.y + rectangle.getSize().y);

	return absCoord;
}

void Entity::playPhysics(int fps)
{
	float dt = 1 / ((float)fps);

	// ---------- Compute new position

	sf::Vector2f newPosition = relativePos;

	/*
		If on the ground, we're working with relative speed.
		Indeed, no need to run particularily complicated physics.
	*/

	if (onGround)
	{
		newPosition.x += dt * relativeSpeed.x;
	}


	/*
		If in the air, we're working with real physics.
		Thus we should be working with absolute speed.
	*/

	else
	{
		float distToCenter = relativePos.y
			+ planet->getCenterShape().getRadius() + rectangle.getSize().y / 2;
		float gnorm = G * (planet->getMass()) * m_mass / (pow(distToCenter, 2));
		relativeSpeed.y -= dt * gnorm;
		newPosition.y += dt * relativeSpeed.y;
	}

	// ----------------- Manage collisions

	std::pair<sf::Vector2f, bool> res;

	res = planet->collidingBlocks(relativePos, newPosition);

	relativePos = res.first;
	if (relativePos.x > 360)
	{
		relativePos.x -= 360;
	}
	if (relativePos.x < 0)
	{
		relativePos.x += 360;
	}
	onGround = res.second;
}

void Entity::render(sf::RenderWindow* window)
{
	sf::Text nameText;
	float csize = 100;
	nameText.setCharacterSize(csize);
	nameText.setScale(1 / csize, 1 / csize);
	nameText.setString(m_name);
	nameText.setColor(sf::Color::White);
	sf::Font calibri;
	if (!calibri.loadFromFile("data/calibri.ttf"))
	{
		cout << "Erreur lors du chargement de la police calibri" << endl;
	}
	else
	{
		nameText.setFont(calibri);
	}

	sf::Vector2f namePosition;
	namePosition.x = relativePos.x;
	namePosition.y = relativePos.y + 2;
	namePosition = relativePos2absolute(namePosition);

	absolutePos = relativePos2absolute(relativePos);
	rectangle.setPosition(absolutePos.x, absolutePos.y);
	rectangle.setRotation(relativePos.x);
	nameText.setPosition(namePosition.x, namePosition.y);
	nameText.setRotation(relativePos.x);
	window->draw(rectangle);
	window->draw(nameText);
	//std::cout << "Player pos relative x = " << relativePos.x << " and relative y = " << relativePos.y << std::endl;
	//std::cout << "Player pos absolute x = " << absolutePos.x << " and absolute y = " << absolutePos.y << std::endl;
}

void Entity::setRelativePos(sf::Vector2f const& pos)
{
	relativePos = pos;
}

sf::Vector2f Entity::getRelativePosition()
{
	return relativePos;
}

sf::Vector2f Entity::getAbsolutePosition()
{
	return absolutePos;
}

float Entity::getAngle()
{
	return relativePos.x;
}

void Entity::setColor(sf::Color const& color)
{
	m_color = color;
	rectangle.setFillColor(color);
}

sf::Color Entity::getColor()
{
	return m_color;
}

string Entity::getName()
{
	return m_name;
}

void Entity::setName(string const& name)
{
	m_name = name;
}