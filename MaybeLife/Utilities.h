#pragma once
#include <string>
#include <time.h>
#include <SFML/Graphics.hpp>
namespace ut {

	inline std::string to_string(sf::Vector2f vector) {
		return "[" + std::to_string(vector.x) + "," + std::to_string(vector.y) + "]";
	}

	inline std::string to_string(sf::Vector2i vector) {
		return "[" + std::to_string(vector.x) + "," + std::to_string(vector.y) + "]";
	}

	inline float randomNumber(float Min, float Max)
	{
		return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
	}
}