#pragma once
#include <SFML/Graphics.hpp>
#include <chrono>

#include "Environment.h"

class UI
{
public:
	UI(sf::RenderWindow* window, Environment* environment, sf::View* uiView);
	void refresh();
private:
	sf::RenderWindow* window;
	sf::View* uiView;
	static const int fpsAvgSpan = 30, updateAvgSpan = 30;
	std::chrono::time_point<std::chrono::high_resolution_clock> fpsstart, upstart;
	int drawCallsSince = 0, stepCountLast = 0;
	Environment* environment;
	sf::Text fpsText;
	std::string fpsString = "", utString = "";
	sf::RectangleShape rectangle;
};

