#include "Player.h"

using namespace std;

constexpr float G = 0.005;

Player::Player()
{

}

Player::Player(string name, int r_mass, Planet* r_planet)
{
    m_speed = 300;
    m_name = name;
    onGround = true;
    rectangle = sf::RectangleShape(sf::Vector2f(1,2));
    char r = rand()%255;
    char g = rand()%255;
    char b = rand()%255;
    m_color = sf::Color(r,g,b);
    rectangle.setFillColor(m_color);

    m_mass = r_mass;
    planet = r_planet;
    relativePos = sf::Vector2f(0,planet->getRadius()/3+1); ///Debug
}

Player::~Player()
{
    //dtor
}

sf::Vector2f Player::relativePos2absolute(sf::Vector2f relCoord)
{
    //Trigo � faire
    sf::Vector2f absCoord;
    absCoord.y = (planet->getCenterPoint().y)-sinf((relCoord.x*PI/180)+PI/2)*(planet->getCenterShape().getRadius()+relCoord.y+rectangle.getSize().y);
    absCoord.x = (planet->getCenterPoint().x)+cosf((relCoord.x*PI/180)-PI/2)*(planet->getCenterShape().getRadius()+relCoord.y+rectangle.getSize().y);

    return absCoord;
}

//Useless
sf::Vector2f Player::relativeSpeed2absolute(sf::Vector2f relCoord)
{
	//Trigo � faire
	sf::Vector2f absCoord;
	float norm = sqrt(pow(relCoord.x * (planet->getCenterShape().getRadius() + relCoord.y + rectangle.getSize().y), 2) + pow(relCoord.y, 2));
	absCoord.y = - sinf((relCoord.x * PI / 180) + PI / 2) * norm;
	absCoord.x = cosf((relCoord.x * PI / 180) - PI / 2) * norm;

	return absCoord;
}

//Useless
sf::Vector2f Player::absolutePos2relative(sf::Vector2f absCoord)
{
    //Trigo � faire
    sf::Vector2f relCoord;
    sf::Vector2f planetCenter = planet->getCenterShape().getPosition();
    relCoord.y = sqrt(
		pow(absCoord.x - planetCenter.x,2)
		+ pow(absCoord.y - planetCenter.y,2))
		- planet->getRadius() - rectangle.getSize().y;
	relCoord.x = (180 / PI) * atanf((absCoord.x - planetCenter.x)
		/ (absCoord.y - planetCenter.y));

    return relCoord;
}

//Useless
sf::Vector2f Player::absoluteSpeed2relative(sf::Vector2f absCoord)
{
	//Trigo � faire
	sf::Vector2f relCoord;
	sf::Vector2f planetCenter = planet->getCenterShape().getPosition();
	float norm = sqrt(pow(absCoord.x, 2) + pow(absCoord.y, 2));
	relCoord.y = -sqrt(pow(absCoord.x - planetCenter.x, 2)
		+ pow(absCoord.y - planetCenter.y, 2));
	relCoord.x = (180 / PI) * atanf((absCoord.y - planetCenter.y)
		/ (absCoord.x - planetCenter.x));

	return relCoord;
}

void Player::render(sf::RenderWindow* window)
{
    if(relativePos.x > 360)
    {
        relativePos.x = relativePos.x - 360;
    }
    sf::Text nameText;
    float csize = 100;
    nameText.setCharacterSize(csize);
    nameText.setScale(1/csize,1/csize);
    nameText.setString(m_name);
    nameText.setColor(sf::Color::White);
    sf::Font calibri;
    if(!calibri.loadFromFile("data/calibri.ttf"))
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
    rectangle.setPosition(absolutePos.x,absolutePos.y);
    rectangle.setRotation(relativePos.x);
    nameText.setPosition(namePosition.x,namePosition.y);
    nameText.setRotation(relativePos.x);
    window->draw(rectangle);
    window->draw(nameText);
    //std::cout << "Player pos relative x = " << relativePos.x << " and relative y = " << relativePos.y << std::endl;
    //std::cout << "Player pos absolute x = " << absolutePos.x << " and absolute y = " << absolutePos.y << std::endl;
}

void Player::handleDirection(bool right, bool left, bool jump)
{
    if (onGround)
    {
        relativeSpeed.x = 0;
    }
    if (right && onGround)
    {
        relativeSpeed.x = (m_speed/relativePos.y)*(PI/180);
    }
    else if (left && onGround)
    {
        relativeSpeed.x = -(m_speed/relativePos.y)*(PI/180);
    }

    if (jump && onGround)
    {
        onGround = false;
        relativeSpeed.y = +1;
    }
}

void Player::playPhysics(int fps)
{
    float dt = 1/((float) fps);
    /*
        If on the ground, we're working with relative speed.
        Indeed, no need to run particularily complicated physics.
    */

    if(onGround)
    {
       relativePos.x += dt*relativeSpeed.x;
       absoluteSpeed = relativeSpeed2absolute(relativeSpeed);
    }
    

    /*
        If in the air, we're working with real physics.
        Thus we should be working with absolute speed.
    */

    else
    {
        float distToCenter = relativePos.y
			+ planet->getCenterShape().getRadius() + rectangle.getSize().y/2;
        float gnorm = G*(planet->getMass())*m_mass/(pow(distToCenter,2));
		relativeSpeed.y -= dt*gnorm;
		relativePos.y += dt*relativeSpeed.y;
    }
}

void Player::setRelativePos(sf::Vector2f const& pos)
{
    relativePos = pos;
}

sf::Vector2f Player::getRelativePosition()
{
    return relativePos;
}

sf::Vector2f Player::getAbsolutePosition()
{
    return absolutePos;
}

float Player::getAngle()
{
    return relativePos.x;
}

void Player::setColor(sf::Color const& color)
{
    m_color = color;
    rectangle.setFillColor(color);
}

sf::Color Player::getColor()
{
    return m_color;
}

string Player::getName()
{
    return m_name;
}

void Player::setName(string const& name)
{
    m_name = name;
}

sf::Packet& operator <<(sf::Packet& packet, Player& player)
{
    return packet << player.getName() << player.getRelativePosition().x << player.getRelativePosition().y << player.getColor().r << player.getColor().g << player.getColor().b;
}

sf::Packet& operator >>(sf::Packet& packet, Player& player)
{
    string name;
    packet >> name;
    player.setName(name);
    float x;
    float y;
    packet >> x >> y;
    player.setRelativePos(sf::Vector2f(x,y));
    sf::Uint8 r;
    sf::Uint8 g;
    sf::Uint8 b;
    packet >> r >> g >> b;
    sf::Color color = sf::Color(r,g,b);
    player.setColor(color);
    return packet;
}
