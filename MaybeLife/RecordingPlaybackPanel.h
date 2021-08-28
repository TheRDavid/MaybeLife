#pragma once
#include "Panel.h"

#include "Slider.h"
#include "TextPanel.h"
#include "GUI.h"
#include "Button.h"
#include "Checkbox.h"

class RecordingPlaybackPanel :
	public gui::Panel
{
public:
	gui::Slider* m_frameScrubber;
	gui::TextPanel* m_frameNumberDisplay, *m_titleDisplay;
	gui::Button* goBackLiveButton, *nextFrameButton, *previousFrameButton;
	gui::Checkbox* m_recordCheckbox;

	gui::action goLiveAction, nextFrameAction, previousFrameAction, goToFrameAction;

	RecordingPlaybackPanel(sf::RenderWindow* window, sf::Vector2f position);
	void drawSelf(sf::Vector2f relativePosition);

	void goLive(sf::Event event);
	void nextFrame(sf::Event event);
	void previousFrame(sf::Event event);
	void goToFrame(sf::Event event);
};

