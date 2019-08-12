#include "Planet.h"

using namespace std;

Planet::Planet()
{

}

Planet::Planet(int mainRadius, sf::Vector2f position) : SPlanet(mainRadius, position)
{
    centerShape = sf::CircleShape(m_centerRadius);
    centerShape.setOrigin(m_centerRadius,m_centerRadius);
    centerShape.setPosition(m_position);
    m_mass = mainRadius*3.14*mainRadius;
}

bool isInABlock(sf::Vector2f& pos, float blockWidth, std::vector < std::vector < int > >& blocks)
{
	return blocks[(int)(pos.x / blockWidth)][(int)pos.y] != 0 ||
		blocks[(int)(pos.x / blockWidth)][(int)pos.y + 1] != 0 ||
		blocks[(int)(pos.x / blockWidth)][(int)pos.y + 2] != 0 ||
		blocks[(int)(pos.x / blockWidth) + 1][(int)pos.y] != 0 ||
		blocks[(int)(pos.x / blockWidth) + 1][(int)pos.y + 1] != 0 ||
		blocks[(int)(pos.x / blockWidth) + 1][(int)pos.y + 2] != 0;
}

bool isOnTheGround(sf::Vector2f pos, float blockWidth, std::vector < std::vector < int > >& blocks, float epsmin)
{
	return blocks[(int)(pos.x / blockWidth)][(int)(pos.y - epsmin)] != 0 ||
		blocks[(int)(pos.x / blockWidth) + 1][(int)(pos.y - epsmin)] != 0;
}

sf::Vector2f half(sf::Vector2f in)
{
	return sf::Vector2f(in.x / 2, in.y / 2);
}

float norm(sf::Vector2f in)
{
	return sqrt(pow(in.x, 2) + pow(in.y, 2));
}

std::pair<sf::Vector2f,bool> Planet::collidingBlocks(sf::Vector2f pos1, sf::Vector2f pos2)
{
	std::vector<sf::Vector2f> potentialCollidingBlocks;
	bool touchedGround = false;

	float blockWidth = 360 / (float)m_circonference;

	float epsmin = 0.01 / m_circonference;

	// FRENCH : on va procéder par dichotomie!

	// Tout d'abord, on check la position d'arrivée!
	// Il y a au maximum six cases qui peuvent être touchées par l'entité.
	// On regarde si la position d'arrivée est convaincante, et si elle ne l'est pas,
	// on revient en arrière d'une demi-distance puis on avance d'une demi-demi-distance,
	// et ainsi de suite jusqu'à voir avec une bonne précision où on peut s'arrêter.
	// Si on s'aperçoie qu'on touche le sol (écart avec un des deux blocs dessous inférieur à epsilon)
	// on renvoie également le bool true.

	// Cet algo est assez original mais ne fonctionnera que pour l'entité player et sa taille.

	if (!isInABlock(pos2,blockWidth,m_blocks))
	{ 
		return std::pair<sf::Vector2f, bool> (pos2, isOnTheGround(pos2, blockWidth, m_blocks, epsmin));
	}
	else
	{	// Collision à l'arrivée pos2, l'entité sera dans un bloc si elle continue, il faut donc revenir en arrière
		// Début de la dichotomie
		sf::Vector2f newPos = pos1 + half(pos2 - pos1);
		sf::Vector2f lastValid = pos1;
		sf::Vector2f diff = newPos - lastValid;
		while (norm(diff) > epsmin)
		{
			diff = half(diff);
			if (isInABlock(newPos, blockWidth, m_blocks))
			{
				newPos -= diff;
			}
			else
			{
				lastValid = newPos;
				newPos += diff;
			}
		}
		return std::pair<sf::Vector2f, bool>(lastValid, isOnTheGround(lastValid,blockWidth,m_blocks,epsmin));
	}
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
            //4 Choses � r�gler :
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

float Planet::getMass()
{
    return m_mass;
}

Planet::~Planet()
{
    //dtor
}
