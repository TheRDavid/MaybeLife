#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "json.hpp"

class Zone;
class Environment;

enum EntityShape {
	TRIANGLE = 0, RECT = 1, CIRCLE = 2, DIAMOND = 3
};

class Entity : public std::enable_shared_from_this<Entity>
{
public:

	std::string m_name = "Entity";
	EntityShape m_shape = RECT;
	bool m_enabled = true;
	Environment* m_environment;
	sf::Vector2f m_position, m_size;
	bool m_collide;

	unsigned int m_id = ID();
	unsigned long long int m_age = 0;
	sf::Color m_color = sf::Color::White;
	Zone* m_zone;
	Entity(Environment* environment, sf::Vector2f position, sf::Vector2f size, bool collide, sf::Color color);
	Entity(Environment* environment, sf::Vector2f position, sf::Vector2f size, bool collide);
	Entity(Environment* environment, sf::Vector2f position, sf::Vector2f size);
	Entity(Environment* environment, sf::Vector2f position);
	Entity(Environment* environment);
	Entity();
	~Entity()
	{
		//std::cout << "Deleting " << m_name << std::endl;
	}
	virtual void update();
	bool legalPosition_strict(std::shared_ptr<Entity> entity, sf::Vector2f position, Zone* zone);
	std::string to_string();
	std::string to_bounds_string();

	sf::Vector2f bounceFromEdgeIfNecessary(sf::Vector2f dir);

	virtual void jsonify(nlohmann::json* data);

private:

	void updateCollision();
	bool colliding(std::shared_ptr<Entity> entity, sf::Vector2f position, Zone* zone);
	bool colliding(std::shared_ptr<Entity> entity, Zone* zone);
	static unsigned int ID()
	{
		static unsigned int ID = 0;
		return ID++;
	}
};

