#pragma once
#include "Panel.h"

#include "Environment.h"
#include "TextPanel.h"
class StatusPanel : public gui::Panel
{
public:
	Environment* environment;
	gui::TextPanel* statusText;

	StatusPanel(Environment * environment, sf::RenderWindow * window);
	static const int fpsAvgSpan = 30, updateAvgSpan = 30;
	std::chrono::time_point<std::chrono::high_resolution_clock> fpsstart, upstart;
	int drawCallsSince = 0, stepCountLast = 0;
	std::string fpsString = "", utString = "";
	virtual void drawSelf(sf::Vector2f relativePosition);

};

