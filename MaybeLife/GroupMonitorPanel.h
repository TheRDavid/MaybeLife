#pragma once
#include "Panel.h"

#include "Environment.h"
#include "TextPanel.h"
#include "Base.h"
class GroupMonitorPanel : public gui::Panel
{
public:
	Environment* m_environment;
	gui::TextPanel* m_statusText;
	std::shared_ptr<Base> m_base;
	bool m_good;
	GroupMonitorPanel(Environment * environment, sf::RenderWindow * m_window, bool good, std::shared_ptr<Base> base);
	virtual void drawSelf(sf::Vector2f relativePosition);

};

