#include "Peasant.h"

Peasant::Peasant(Environment * environment, sf::Vector2f position, std::shared_ptr<Base> base)
	: Worker(environment, position, sf::Vector2f(2,2), 2, true, 30, base)
{
	m_name = "Peasant";
	m_color = sf::Color(100, 100, 255, 255);
}

void Peasant::jsonify(nlohmann::json * data)
{
	Worker::jsonify(data);
	(*data)["type"] = "Peasant";
}
