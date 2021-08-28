#pragma once

#define UT_PI 3.14159265359f
#define UT_MOUSE_SELECTION_NEGLIGIBLE_SPATIAL_DIFFERENCE 10.0f

#include <string>
#include <time.h>
#include <math.h>
#include <SFML/Graphics.hpp>

#include "json.hpp"

class Entity;
class Base;
class BadGuy;
class Fighter;
class FoodItem;
class FoodSource;
class GoodGuy;
class Peasant;
class Slave;
class Wall;
class Worker;
class Person;

namespace ut {

	std::shared_ptr<Entity> convertToEntity(nlohmann::json data);
	void writeIntoEntity(std::shared_ptr<Entity> entity, nlohmann::json data);
	void writeIntoBase(std::shared_ptr<Base> entity, nlohmann::json data);
	void writeIntoBadGuy(std::shared_ptr<BadGuy> entity, nlohmann::json data);
	void writeIntoFighter(std::shared_ptr<Fighter> entity, nlohmann::json data);
	void writeIntoFoodItem(std::shared_ptr<FoodItem> entity, nlohmann::json data);
	void writeIntoFoodSource(std::shared_ptr<FoodSource> entity, nlohmann::json data);
	void writeIntoGoodGuy(std::shared_ptr<GoodGuy> entity, nlohmann::json data);
	void writeIntoPeasant(std::shared_ptr<Peasant> entity, nlohmann::json data);
	void writeIntoPerson(std::shared_ptr<Person> entity, nlohmann::json data);
	void writeIntoSlave(std::shared_ptr<Slave> entity, nlohmann::json data);
	void writeIntoWall(std::shared_ptr<Wall> entity, nlohmann::json data);
	void writeIntoWorker(std::shared_ptr<Worker> entity, nlohmann::json data);

	struct rectf
	{
		float x0,y0,x1,y1;
	};

	inline std::string to_string(sf::Vector2f vector) {
		return "[" + std::to_string(vector.x) + "," + std::to_string(vector.y) + "]";
	}

	inline std::string to_string_integers(sf::Vector2f vector) {
		return "[" + std::to_string((int)vector.x) + "," + std::to_string((int)vector.y) + "]";
	}

	inline sf::Vector2f to_vector2f(std::string str) {
		std::string stripped = str.substr(1, str.length() - 2);
		size_t c0 = stripped.find(",");
		size_t c1 = stripped.find(",", c0 + 1);

		return sf::Vector2f(
			std::stof(stripped.substr(0, c0)),
			std::stof(stripped.substr(c0 + 1, c1 - c0 - 1))
		);
	}

	inline bool pointInRect(rectf rect, sf::Vector2f point)
	{
		return
			point.x >= rect.x0 && point.x <= rect.x1 &&
			point.y >= rect.y0 && point.y <= rect.y1;
	}

	inline sf::Color to_color(std::string str) {
		std::string stripped = str.substr(1, str.length() - 2);
		size_t c0 = stripped.find(",");
		size_t c1 = stripped.find(",", c0 + 1);
		size_t c2 = stripped.find(",", c1 + 1);

		return sf::Color(
			std::stoi(stripped.substr(0, c0)),
			std::stoi(stripped.substr(c0 + 1, c1 - c0 - 1)),
			std::stoi(stripped.substr(c1 + 1, c2 - c1 - 1))
		);
	}

	inline std::string to_string(sf::Vector2i vector) {
		return "[" + std::to_string(vector.x) + "," + std::to_string(vector.y) + "]";
	}

	inline std::string to_string(sf::Color color) {
		return "[" + std::to_string(color.r) + "," + std::to_string(color.g) + "," + std::to_string(color.b) + "," + std::to_string(color.a) + "]";
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