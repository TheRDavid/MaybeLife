#include "RecordingPlaybackPanel.h"

#include "Commander.h"
RecordingPlaybackPanel::RecordingPlaybackPanel(sf::RenderWindow * window, sf::Vector2f position)
	: gui::Panel(window, position, sf::Vector2f(330, 140), sf::Color(30, 30, 50, 200), sf::Color::White, 1, true)
{
	m_titleDisplay = new TextPanel(window, sf::Vector2f(20, 20), sf::Vector2f(0, 0), sf::Color::Transparent, sf::Color::Transparent, 0, sf::Color::White, "Recording", 15);
	m_frameNumberDisplay = new TextPanel(window, sf::Vector2f(210, 20), sf::Vector2f(0, 0), sf::Color::Transparent, sf::Color::Transparent, 0, sf::Color::White, "0/0", 14);

	action goLiveAction = [this](sf::Event event) {
		this->goLive(event);
	};

	action nextFrameAction = [this](sf::Event event) {
		this->nextFrame(event);
	};

	action previousFrameAction = [this](sf::Event event) {
		this->previousFrame(event);
	};

	action travelForwardsAction = [this](sf::Event event) {
		this->travelForward(event);
	};

	action travelBackwardAction = [this](sf::Event event) {
		this->travelBackward(event);
	};

	action goToFrameAction = [this](sf::Event event) {
		this->goToFrame(event);
	};

	m_frameScrubber = new gui::Slider(window, sf::Vector2f(20, 50), sf::Vector2f(290, 30), 0, 1, 0, sf::Color::White, goToFrameAction);
	travelBackwardButton = new Button(window, sf::Vector2f(20, 80), sf::Vector2f(50, 30), "<<", travelBackwardAction, sf::Color::White);
	previousFrameButton = new Button(window, sf::Vector2f(80, 80), sf::Vector2f(50, 30), "<", previousFrameAction, sf::Color::White);
	nextFrameButton = new Button(window, sf::Vector2f(140, 80), sf::Vector2f(50, 30), ">", nextFrameAction, sf::Color::White);
	travelForwardButton = new Button(window, sf::Vector2f(200, 80), sf::Vector2f(50, 30), ">>", travelForwardAction, sf::Color::White);
	goBackLiveButton = new Button(window, sf::Vector2f(260, 80), sf::Vector2f(50, 30), "o", goLiveAction, sf::Color::White);

	addChild(m_titleDisplay);
	addChild(m_frameNumberDisplay);
	addChild(m_frameScrubber);
	addChild(travelBackwardButton);
	addChild(previousFrameButton);
	addChild(nextFrameButton);
	addChild(travelForwardButton);
	addChild(goBackLiveButton);
}

void RecordingPlaybackPanel::drawSelf(sf::Vector2f relativePosition)
{
	if (!m_frameScrubber->m_scrubbing)
	{
		m_frameScrubber->m_maxValue = Commander::getInstance().getCurrentSimulationStep();
	}
	Panel::drawSelf(relativePosition);
	m_frameNumberDisplay->setText(std::to_string((int)m_frameScrubber->m_value) + " / " + std::to_string((int)m_frameScrubber->m_maxValue));
}

void RecordingPlaybackPanel::goLive(sf::Event event)
{
	Commander::getInstance().displayLiveSimulation();
}

void RecordingPlaybackPanel::nextFrame(sf::Event event)
{
	int newFrame = m_frameScrubber->m_value + 1;
	if (newFrame >= 0 && newFrame <= m_frameScrubber->m_maxValue)
	{
		m_frameScrubber->m_value = newFrame;
	}
	goToFrame(event);
}

void RecordingPlaybackPanel::previousFrame(sf::Event event)
{
	int newFrame = m_frameScrubber->m_value - 1;
	if (newFrame >= 0 && newFrame <= m_frameScrubber->m_maxValue)
	{
		m_frameScrubber->m_value = newFrame;
	}
	goToFrame(event);
}

void RecordingPlaybackPanel::travelForward(sf::Event event)
{
}

void RecordingPlaybackPanel::travelBackward(sf::Event event)
{
}

void RecordingPlaybackPanel::goToFrame(sf::Event event)
{
	Commander::getInstance().displayRecordedFrame(m_frameScrubber->m_value);
}
