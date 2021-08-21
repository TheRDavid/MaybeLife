#pragma once
#include "Panel.h"

#include "Environment.h"
#include "TextPanel.h"
class StatusPanel : public gui::Panel
{
public:
	Environment* m_environment;
	gui::TextPanel* m_statusText;

	StatusPanel(Environment * environment, sf::RenderWindow * m_window);
	static const int m_fpsAvgSpan = 30, m_updateAvgSpan = 30;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_fpsstart, m_upstart;
	int m_drawCallsSince = 0, m_stepCountLast = 0;
	std::string m_fpsString = "", m_utString = "";
	virtual void drawSelf(sf::Vector2f relativePosition);

};

