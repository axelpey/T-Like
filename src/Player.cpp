#include "../include/Player.h"

using namespace std;

Player::Player()
{

}

Player::Player(string name, int r_weight, Planet* r_planet)
{
    m_name = name;
    rectangle = sf::RectangleShape(sf::Vector2f(1,2));
    char r = rand()%255;
    char g = rand()%255;
    char b = rand()%255;
    m_color = sf::Color(r,g,b);
    rectangle.setFillColor(m_color);

    weight = r_weight;
    planet = r_planet;
    relativePos = sf::Vector2f(0,planet->getRadius()/3+1); ///Debug
}

Player::~Player()
{
    //dtor
}

sf::Vector2f Player::relativePos2absolute(sf::Vector2f relativePosition)
{
    //Trigo à faire
    sf::Vector2f absolutePosition;
    absolutePosition.y = (planet->getCenterPoint().y)-sinf((relativePosition.x*PI/180)+PI/2)*(planet->getCenterShape().getRadius()+relativePosition.y+rectangle.getSize().y);
    absolutePosition.x = (planet->getCenterPoint().x)+cosf((relativePosition.x*PI/180)-PI/2)*(planet->getCenterShape().getRadius()+relativePosition.y+rectangle.getSize().y);

    return absolutePosition;
}

void Player::render(sf::RenderWindow* window)
{
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

void Player::goLeft()
{
    relativePos.x--;
}

void Player::goRight()
{
    relativePos.x++;
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

int Player::getAngle()
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
