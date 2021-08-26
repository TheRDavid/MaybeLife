#include "Wall.h"

Wall::Wall(Environment * environment, sf::Vector2f position)
	: Entity(environment, position, sf::Vector2f(4, 4))
{
	m_name = "Wall";
	m_color = sf::Color(88, 255, 88, 120);
}

void Wall::jsonify(nlohmann::json * data)
{
	Entity::jsonify(data);
	(*data)["type"] = "Wall";
	(*data)["health"] = m_health;
}
