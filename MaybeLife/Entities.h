#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;
class Zone;
class Entities
{
public:
	vector<Vector2f>* positions=new vector<Vector2f>(), *sizes = new vector<Vector2f>();
	vector<Color>* colors = new vector<Color>();
	vector<Zone*>* zones = new vector<Zone*>();
	Entities(int capacity);
	void add(Vector2f position, Vector2f size, Color color);
	std::string toString(int id);
};

