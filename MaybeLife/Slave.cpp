#include "Slave.h"

Slave::Slave(Environment * environment, sf::Vector2f position, std::shared_ptr<Base> base)
	: Worker(environment, position, sf::Vector2f(2,2), 2, false, 20, base)
{
	m_name = "Slave";
	m_color = sf::Color(255, 100, 100, 255);
}

void Slave::jsonify(nlohmann::json * data)
{
	Worker::jsonify(data);
	(*data)["type"] = "Slave";
}
