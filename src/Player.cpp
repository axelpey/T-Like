#include "Player.h"

using namespace std;

constexpr float G = 0.01;

Player::Player()
{}

Player::Player(string name, int r_mass, Planet* r_planet) : Entity(name, r_mass, r_planet)
{
    m_speed = 2000;
}

Player::~Player()
{
    //dtor
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
        relativeSpeed.y = +2;
    }
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
