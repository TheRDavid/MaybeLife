#pragma once

#define PI 3.14159265359f
#include <string>
#include <time.h>
#include <math.h>
#include <SFML/Graphics.hpp>
namespace ut {
	//float pi = 3.14159265359f;
	inline std::string to_string(sf::Vector2f vector) {
		return "[" + std::to_string(vector.x) + "," + std::to_string(vector.y) + "]";
	}

	inline std::string to_string(sf::Vector2i vector) {
		return "[" + std::to_string(vector.x) + "," + std::to_string(vector.y) + "]";
	}

	inline std::string to_string(sf::Color color) {
		return "[r=" + std::to_string(color.r) + " g=" + std::to_string(color.g) + " b=" + std::to_string(color.b) + " a=" + std::to_string(color.a) + "]";
	}

	inline float randomNumber(float Min, float Max)
	{
		return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
	}

	inline float manhattenDistance(sf::Vector2f a, sf::Vector2f b)
	{
		return abs(a.x - b.x) + abs(a.y - b.y);
	}

	inline sf::Vector2f normalize(sf::Vector2f source)
	{
		float length = sqrt((source.x * source.x) + (source.y * source.y));
		if (length != 0)
			return sf::Vector2f(source.x / length, source.y / length);
		else
			return source;
	}

	inline sf::Vector2f rotate(sf::Vector2f source, float degree)
	{
		float degreeRotation = degree * PI / 180;
		return sf::Vector2f(source.x*cos(degreeRotation) - source.y*sin(degreeRotation), source.x*sin(degreeRotation) + source.y*cos(degreeRotation));
	}

}