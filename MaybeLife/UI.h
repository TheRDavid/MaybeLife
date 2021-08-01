#pragma once
#include <SFML/Graphics.hpp>
#include "AppConfig.h"
#include "Environment.h"
using namespace sf;
class UI
{
public:
	UI(sf::RenderWindow* window, Environment* environment);
	sf::RenderWindow* window;
	void refresh();
private:
	static const int fpsAvgSpan = 60;
	Clock fpsClock;
	float fpsSamples[fpsAvgSpan];
	int fpsCount = 0;
	float avgFPS = 0;
	Environment* environment;
	Text fpsText;
};

