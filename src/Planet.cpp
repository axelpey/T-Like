#include "../include/Planet.h"

using namespace std;

Planet::Planet()
{

}

Planet::Planet(int mainRadius, sf::Vector2f position) : SPlanet(mainRadius, position)
{
    centerShape = sf::CircleShape(m_centerRadius);
    centerShape.setOrigin(m_centerRadius,m_centerRadius);
    centerShape.setPosition(m_position);
}

void Planet::render(sf::RenderWindow* window)
{
    window->draw(getCenterShape());

    float alpha = (360/m_circonference);
    alpha = (alpha*PI/180)/2;
    float const sinalpha = sinf(alpha);
    float const cosalpha = cosf(alpha);
    float const sinalphadiv2 = sinf(alpha/2);

    for(int j = 0; j < m_height; j++)
    {
        //La forme
        float DC = 2*(m_centerRadius+j)*sinalphadiv2;
        float EC = DC * sinalpha;
        float ED = DC * cosalpha;

        float AB =  2*(m_centerRadius+1+j)*sinalphadiv2;
        float FB = AB * sinalpha;
        float FA = AB * cosalpha;

        sf::Vector2f p1,p2,p3,p4;

        p1.x = 0;
        p1.y = 0;
        p4.x = 0;
        p4.y = 1;
        p2.x = p1.x + 2*FA;
        p2.y = p1.y + 2*FB;
        p3.x = p4.x + 2*ED;
        p3.y = p4.y + 2*EC;

        /*float xA = 0;
        float yA = 0;
        float xD = 0;
        float yD = 1;
        float xB = xA + 2*FA;
        float yB = yA + 2*FB;
        float xC = xD + 2*ED;
        float yC = yD + 2*EC;*/

        for(int i = 0; i < m_blocks.size(); i++)
        {
            if(m_blocks[i][j] == 0)
                continue;
            //4 Choses à régler :
            sf::ConvexShape block;

            block.setPointCount(4);

            //Design du bloc en fonction de son type
            switch(m_blocks[i][j])
            {
            case 1:
                block.setFillColor(sf::Color(102,51,0));
                block.setOutlineThickness(-0.001);
                block.setOutlineColor(sf::Color::Black);
                break;

            case 2:
                block.setFillColor(sf::Color(192,192,192));
                block.setOutlineThickness(-0.001);
                block.setOutlineColor(sf::Color::Black);
                break;

            default:
                block.setFillColor(sf::Color::Red);
                //block.setOutlineThickness(-0.01); Pas de bordure pour l'air
                block.setOutlineColor(sf::Color::Black);
                break;
            }

            block.setPoint(0,p1);
            block.setPoint(1,p2);
            block.setPoint(2,p3);
            block.setPoint(3,p4);

            //La position
            //Le point 0 donc A est l'origine normalement
            float posx = m_position.x-cos((2*PI/m_blocks.size())*i+PI/2)*(m_centerRadius+j+1);
            float posy = m_position.y-sin((2*PI/m_blocks.size())*i+PI/2)*(m_centerRadius+j+1);

            block.setPosition(posx,posy);

            //La rotation
            float circonferencef = 2*PI*m_mainRadius;
            float angle = i*(360/circonferencef);

            block.setRotation(angle);

            window->draw(block);
        }
    }
}

sf::CircleShape Planet::getCenterShape()
{
    return centerShape;
}

Planet::~Planet()
{
    //dtor
}
