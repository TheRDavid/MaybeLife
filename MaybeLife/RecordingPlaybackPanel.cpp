#include "RecordingPlaybackPanel.h"

#include "Commander.h"
RecordingPlaybackPanel::RecordingPlaybackPanel(sf::RenderWindow * window, sf::Vector2f position)
	: gui::Panel(window, position, sf::Vector2f(330, 140), sf::Color(30, 30, 50, 200), sf::Color::White, 1, true)
{
	m_titleDisplay = new TextPanel(window, sf::Vector2f(20, 20), sf::Vector2f(0, 0), sf::Color::Transparent, sf::Color::Transparent, 0, sf::Color::White, "Recording", 15);
	m_frameNumberDisplay = new TextPanel(window, sf::Vector2f(180, 20), sf::Vector2f(0, 0), sf::Color::Transparent, sf::Color::Transparent, 0, sf::Color::White, "0/0", 14);

	action goLiveAction = [this](sf::Event event) {
		this->goLive(event);
	};

	action nextFrameAction = [this](sf::Event event) {
		this->nextFrame(event);
	};

	action previousFrameAction = [this](sf::Event event) {
		this->previousFrame(event);
	};

	action goToFrameAction = [this](sf::Event event) {
		this->goToFrame(event);
	};

	m_frameScrubber = new gui::Slider(window, sf::Vector2f(20, 50), sf::Vector2f(290, 30), 0, 1, 0, sf::Color::White, goToFrameAction);
	previousFrameButton = new Button(window, sf::Vector2f(20, 80), sf::Vector2f(50, 30), "<", previousFrameAction, sf::Color::White);
	nextFrameButton = new Button(window, sf::Vector2f(90, 80), sf::Vector2f(50, 30), ">", nextFrameAction, sf::Color::White);
	goBackLiveButton = new Button(window, sf::Vector2f(170, 80), sf::Vector2f(140, 30), "Go back live", goLiveAction, sf::Color::White);
	m_recordCheckbox = new Checkbox(window, sf::Vector2f(20, 20), sf::Vector2f(140, 20), sf::Color::White, sf::Color::White, "Record", 14, Commander::getInstance().isRecording());

	addChild(m_recordCheckbox);
	addChild(m_frameNumberDisplay);
	addChild(m_frameScrubber);
	addChild(previousFrameButton);
	addChild(nextFrameButton);
	addChild(goBackLiveButton);
}

void RecordingPlaybackPanel::drawSelf(sf::Vector2f relativePosition)
{
	bool currentlyRecording = Commander::getInstance().isRecording();
	m_frameNumberDisplay->setText(std::to_string((int)m_frameScrubber->m_value) + " / " + std::to_string((int)m_frameScrubber->m_maxValue));
	if (currentlyRecording)
	{
		if (!m_frameScrubber->m_scrubbing)
		{
			m_frameScrubber->m_maxValue = (float) Commander::getInstance().getCurrentSimulationStep();
		}
		if (Commander::getInstance().displayIsLiveSimulation())
		{
			m_frameScrubber->m_value = m_frameScrubber->m_maxValue;
		}
	}
	Panel::drawSelf(relativePosition);
}

void RecordingPlaybackPanel::goLive(sf::Event event)
{
	Commander::getInstance().displayLiveSimulation();
}

void RecordingPlaybackPanel::nextFrame(sf::Event event)
{
	int newFrame = (int) m_frameScrubber->m_value + 1;
	if (newFrame >= 0 && newFrame <= m_frameScrubber->m_maxValue)
	{
		m_frameScrubber->m_value = newFrame;
	}
	goToFrame(event);
}

void RecordingPlaybackPanel::previousFrame(sf::Event event)
{
	int newFrame = (int) m_frameScrubber->m_value - 1;
	if (newFrame >= 0 && newFrame <= m_frameScrubber->m_maxValue)
	{
		m_frameScrubber->m_value = newFrame;
	}
	goToFrame(event);
}

void RecordingPlaybackPanel::goToFrame(sf::Event event)
{
	Commander::getInstance().displayRecordedFrame((int) m_frameScrubber->m_value);
}
