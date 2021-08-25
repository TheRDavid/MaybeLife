#pragma once
#include "Panel.h"

#include "Commander.h"
#include "Environment.h"
#include "ProgressBar.h"
#include "TextPanel.h"
#include "Button.h"
#include "Base.h"
#include "GUI.h"

class RuntimeControlPanel : public gui::Panel
{
public:
	Environment* m_environment;
	gui::TextPanel* m_timeText;
	gui::Button* m_playPauseButton, *m_stepButton, *m_stopButton;
	gui::ProgressBar* m_powerRatioBar;
	RuntimeControlPanel(sf::RenderWindow * m_window, Environment* environment);
	virtual void drawSelf(sf::Vector2f relativePosition) override;
private:
	void playPause(sf::Event event);
	void step(sf::Event event);
	void stop(sf::Event event);
};
