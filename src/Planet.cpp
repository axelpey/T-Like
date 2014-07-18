#include "../include/Planet.h"

using namespace std;

Planet::Planet()
{

}

Planet::Planet(int r_mainRadius, sf::Vector2f r_position)
{
    mainRadius = r_mainRadius;
    centerRadius = (mainRadius/3)*2;

    position = r_position;
    centerShape = sf::CircleShape(centerRadius);
    centerShape.setOrigin(centerRadius,centerRadius);
    centerShape.setPosition(position);

    height = (mainRadius-centerRadius)*2;

    generatePlanet();
}

void Planet::generatePlanet()
{
    circonference = 2*PI*mainRadius;
    m_blocks = vector < vector < int > >(circonference, vector < int > (height, 0));

    for(int i = 0; i < circonference; i++)
    {
        vector < int > column;

        for(int j = 0; j < height; j++)
        {
            if(j<(height/2))
                column.push_back(2);
            if(j==(height/2))
                column.push_back(1);
            if(j>(height/2))
                column.push_back(0);
        }

        m_blocks[i] = column;
    }
}

void Planet::render(sf::RenderWindow* window)
{
    window->draw(getCenterShape());

    float alpha = (360/circonference);
    alpha = (alpha*PI/180)/2;
    float const sinalpha = sinf(alpha);
    float const cosalpha = cosf(alpha);
    float const sinalphadiv2 = sinf(alpha/2);

    for(int j = 0; j < height; j++)
    {
        //La forme
        float DC = 2*(centerRadius+j)*sinalphadiv2;
        float EC = DC * sinalpha;
        float ED = DC * cosalpha;

        float AB =  2*(centerRadius+1+j)*sinalphadiv2;
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
                block.setOutlineThickness(-0.01);
                block.setOutlineColor(sf::Color::Black);
                break;

            case 2:
                block.setFillColor(sf::Color(192,192,192));
                block.setOutlineThickness(-0.01);
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
            float posx = position.x-cos((2*PI/m_blocks.size())*i+PI/2)*(centerRadius+j+1);
            float posy = position.y-sin((2*PI/m_blocks.size())*i+PI/2)*(centerRadius+j+1);

            block.setPosition(posx,posy);

            //La rotation
            float circonferencef = 2*PI*mainRadius;
            float angle = i*(360/circonferencef);

            block.setRotation(angle);

            window->draw(block);
        }
    }
}

void Planet::setBlock(sf::Vector2i position, int const& blockid)
{
    m_blocks[position.x][position.y] = blockid;
}

int Planet::getBlock(sf::Vector2i position)
{
    return m_blocks[position.x][position.y];
}

int Planet::getRadius()
{
    return mainRadius;
}

sf::CircleShape Planet::getCenterShape()
{
    return centerShape;
}

sf::Vector2f Planet::getCenterPoint()
{
    return position;
}

Planet::~Planet()
{
    //dtor
}
