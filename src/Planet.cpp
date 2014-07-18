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
    blocks = vector < vector < int > >(circonference, vector < int > (height, 0));

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

        blocks[i] = column;
    }
}

void Planet::render(sf::RenderWindow* window)
{
    window->draw(getCenterShape());

    for(int i = 0; i < blocks.size(); i++)
    {
        vector < int > const& column = blocks[i];

        for(int j = 0; j < height; j++)
        {
            if(column[j] == 0)
                break;
            //4 Choses à régler :
            sf::ConvexShape block;

            block.setPointCount(4);

            //Design du bloc en fonction de son type
            switch(column[j])
            {
            case 0:
                block.setFillColor(sf::Color::Transparent);
                //block.setOutlineThickness(-0.01); Pas de bordure pour l'air
                block.setOutlineColor(sf::Color::Black);
                break;

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
            }

            //La forme
            float alpha = (360/circonference);
            alpha = alpha*PI/180;
            alpha = alpha/2;
            float DC = 2*(centerRadius+j)*sinf(alpha/2);
            float EC = DC * sinf(alpha);
            float ED = DC * cosf(alpha);
            float sinff = sin(alpha);

            float AB =  2*(centerRadius+1+j)*sinf(alpha/2);
            float FB = AB * sinf(alpha);
            float FA = AB * cosf(alpha);

            float xA = 0;
            float yA = 0;
            float xD = 0;
            float yD = 1;
            float xB = xA + 2*FA;
            float yB = yA + 2*FB;
            float xC = xD + 2*ED;
            float yC = yD + 2*EC;

            block.setPoint(0,sf::Vector2f(xA,yA));
            block.setPoint(1,sf::Vector2f(xB,yB));
            block.setPoint(2,sf::Vector2f(xC,yC));
            block.setPoint(3,sf::Vector2f(xD,yD));

            //La position
            //Le point 0 donc A est l'origine normalement
            float posx = position.x-cos((2*PI/blocks.size())*i+PI/2)*(centerRadius+j+1);
            float posy = position.y-sin((2*PI/blocks.size())*i+PI/2)*(centerRadius+j+1);

            block.setPosition(posx,posy);

            //La rotation
            float circonferencef = 2*PI*mainRadius;
            float angle = i*(360/circonferencef);

            block.setRotation(angle);

            window->draw(block);
        }
    }
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
