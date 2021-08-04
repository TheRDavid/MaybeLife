#pragma once
#include <SFML/Graphics.hpp>
#include "AppConfig.h"
#include "Environment.h"
#include <chrono>
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
using namespace sf;
using namespace std;
class UI
{
public:
	UI(sf::RenderWindow* window, Environment* environment);
	sf::RenderWindow* window;
	void refresh();
private:
	static const int fpsAvgSpan = 30, updateAvgSpan = 30;
	std::chrono::time_point<std::chrono::high_resolution_clock> fpsstart, upstart;
	int drawCallsSince = 0, stepCountLast = 0;
	Environment* environment;
	Text fpsText;
	string fpsString = "", utString = "";
	sf::RectangleShape rectangle;
};

