#include "Base.h"

Base::Base(Environment * environment, sf::Vector2f position, bool good, float startNutrition)
	: Entity(environment, position, sf::Vector2f(12,12)), m_nutrition(startNutrition)
{
	if (good)
	{
		m_color = sf::Color(0, 0, 255, 255);
	}
	else {
		m_color = sf::Color(255, 0, 0, 255);
	}
}
