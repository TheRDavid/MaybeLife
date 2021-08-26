#pragma once
#include "Person.h"
class Fighter :
	public Person
{
public:
	float m_attack;
	Fighter(Environment* environment, sf::Vector2f position, sf::Vector2f size, float speed, bool good, float attack, float viewDistance, std::shared_ptr<Base> base);
	Fighter() {} // for json conversion

	void attack(Person* person);
	virtual void jsonify(nlohmann::json* data) override;
};

