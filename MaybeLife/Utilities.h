#pragma once

#define UT_PI 3.14159265359f
#define UT_MOUSE_SELECTION_NEGLIGIBLE_SPATIAL_DIFFERENCE 10.0f

#include <string>
#include <time.h>
#include <math.h>
#include <SFML/Graphics.hpp>
namespace ut {

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

	inline float euclideanDistance(sf::Vector2f a, sf::Vector2f b)
	{
		return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
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
		float degreeRotation = degree * UT_PI / 180;
		return sf::Vector2f(source.x*cos(degreeRotation) - source.y*sin(degreeRotation), source.x*sin(degreeRotation) + source.y*cos(degreeRotation));
	}

	inline std::string timeDescription(int timeSteps) {
		int year = 365 * 24 * 60 * 60;
		int month = year / 12;
		int day = month / 30;
		int hour = day / 24;
		int minute = hour / 60;
		int second = minute / 60;

		int years = timeSteps / year;
		timeSteps -= years * year;

		int months = timeSteps / month;
		timeSteps -= months * month;

		int days = timeSteps / day;
		timeSteps -= days * day;

		int hours = timeSteps / hour;
		timeSteps -= hours * hour;

		int minutes = timeSteps / minute;
		timeSteps -= minutes * minute;

		int seconds = timeSteps / second;
		timeSteps -= seconds * second;

		return (hours < 9 ? "0" : "") + std::to_string(hours + 1) + ":" + (minutes < 9 ? "0" : "") + std::to_string(minutes + 1) + ":" + (seconds < 9 ? "0" : "") + std::to_string(seconds + 1) + " " + (days < 9 ? "0" : "") + std::to_string(days + 1) + "/" + (months < 9 ? "0" : "") + std::to_string(months + 1) + "/" + std::to_string(years);
	}

}