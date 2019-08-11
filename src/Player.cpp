#include "Player.h"

using namespace std;

Player::Player()
{

}

Player::Player(string name, int r_mass, Planet* r_planet)
{
    m_speed = 0.5;
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

sf::Vector2f Player::relativeCoord2absolute(sf::Vector2f relCoord)
{
    //Trigo � faire
    sf::Vector2f absCoord;
    absCoord.y = (planet->getCenterPoint().y)-sinf((relCoord.x*PI/180)+PI/2)*(planet->getCenterShape().getRadius()+relCoord.y+rectangle.getSize().y);
    absCoord.x = (planet->getCenterPoint().x)+cosf((relCoord.x*PI/180)-PI/2)*(planet->getCenterShape().getRadius()+relCoord.y+rectangle.getSize().y);

    return absCoord;
}

sf::Vector2f Player::absoluteCoord2relative(sf::Vector2f absCoord)
{
    //Trigo � faire
    sf::Vector2f relCoord;
    relCoord.x = (PI/2 + acosf(absCoord.x - (planet->getCenterPoint().x))/(planet->getCenterShape().getRadius()+relCoord.y+rectangle.getSize().y))*180/PI;
    relCoord.y = (-PI/2 + asinf(-absCoord.x + (planet->getCenterPoint().x))/(planet->getCenterShape().getRadius()+relCoord.y+rectangle.getSize().y))*180/PI;
    
    return absCoord;
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
    namePosition = relativeCoord2absolute(namePosition);

    absolutePos = relativeCoord2absolute(relativePos);
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
    if (right && onGround)
    {
        relativeSpeed.x = (m_speed/relativePos.y)*(PI/180);
    }
    else if (left && onGround)
    {
        relativeSpeed.x = -(m_speed/relativePos.y)*(PI/180);
    }
    else if (onGround)
    {
        relativeSpeed.x = 0;
    }

    if (jump && onGround)
    {
        onGround = false;
        relativeSpeed.y = +1;
    }
}

void Player::playPhysics()
{
    /*
        If on the ground, we're working with relative speed.
        Indeed, no need to run particularily complicated physics.
    */

    if(onGround)
    {
       relativePos.x += relativeSpeed.x;
       absoluteSpeed = relativeCoord2absolute(relativeSpeed);
    }
    

    /*
        If in the air, we're working with real physics.
        Thus we should be working with absolute speed.
    */

    else
    {
        relativeSpeed.y += G*(planet->getMass())*m_mass/(relativePos.y*relativePos.y);
        absoluteSpeed = relativeCoord2absolute(relativeSpeed);
        relativePos.y += relativeSpeed.y;
        relativePos.x += relativeSpeed.x;
        //absoluteSpeed += G*(planet->getMass())*m_mass/(relativePos.y*relativePos.y);
        //relativeSpeed = absoluteCoord2relative(relativeSpeed);
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
