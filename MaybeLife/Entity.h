#pragma once
#include <SFML/Graphics.hpp>
class Zone;
class Environment;
class Entity
{
public:

	Environment* m_environment;
	sf::Vector2f m_position, m_size;
	bool m_collide;

	unsigned long long int m_id = ID();
	sf::Color color = sf::Color::White;
	Zone* zone;
	Entity(Environment* environment, sf::Vector2f position, sf::Vector2f size, bool collide, sf::Color color);
	Entity(Environment* environment, sf::Vector2f position, sf::Vector2f size, bool collide);
	Entity(Environment* environment, sf::Vector2f position, sf::Vector2f size);
	Entity(Environment* environment, sf::Vector2f position);
	Entity(Environment* environment);
	void update();
	bool legalPosition_strict(Entity* entity, sf::Vector2f position, Zone* zone);
	std::string to_string();
	std::string to_bounds_string();

private:

	void updateCollision();
	bool colliding(Entity* entity, sf::Vector2f position, Zone* zone);
	bool colliding(Entity* entity, Zone* zone);
	static unsigned long long int ID()
	{
		static unsigned long long int ID = 0;
		return ID++;
	}
};

