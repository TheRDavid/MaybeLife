#include "RuntimeControlPanel.h"

RuntimeControlPanel::RuntimeControlPanel(sf::RenderWindow * window, Environment* environment) 
	:gui::Panel(window, sf::Vector2f(window->getSize().x/2-150, 0), sf::Vector2f(300, 60), sf::Color(15,15,30,200), sf::Color::Yellow, 1, false),
	m_environment(environment)
{
	m_timeText = new gui::TextPanel(
		window,
		sf::Vector2f(85,40),
		sf::Vector2f(0, 0),
		sf::Color::Transparent,
		sf::Color::Transparent,
		0,
		sf::Color(200, 200, 255, 255),
		"Uninitialized",
		13
	);
	addChild(m_timeText);

	action playPauseAction = [this](sf::Event event) {
		this->playPause(event);
	};

	action stepAction = [this](sf::Event event) {
		this->step(event);
	};

	action stopAction = [this](sf::Event event) {
		this->stop(event);
	};

	m_playPauseButton = new gui::Button(
		window, sf::Vector2f(25, 10), sf::Vector2f(70, 25), "Pause",
		playPauseAction, sf::Color::White
	);

	m_stepButton = new gui::Button(
		window, sf::Vector2f(110, 10), sf::Vector2f(70, 25), "Step",
		stepAction, sf::Color::White
	);

	m_stopButton = new gui::Button(
		window, sf::Vector2f(195, 10), sf::Vector2f(70, 25), "Stop",
		stopAction, sf::Color::White
	);

	m_powerRatioBar = new ProgressBar(window, sf::Vector2f(0, 60), sf::Vector2f(m_size.x, 30), 0.2f, sf::Color(100, 100, 255, 255), sf::Color(255, 100, 100, 255));

	addChild(m_playPauseButton);
	addChild(m_stepButton);
	addChild(m_stopButton);
	addChild(m_powerRatioBar);
}

void RuntimeControlPanel::drawSelf(sf::Vector2f relativePosition)
{
	m_powerRatioBar->m_value = Commander::getInstance().goodToBadRatio();
	Panel::drawSelf(relativePosition);
	m_timeText->setText(ut::timeDescription(m_environment->steps) + "\n");
}

void RuntimeControlPanel::playPause(sf::Event event)
{
	if (m_environment->m_paused)
	{
		m_playPauseButton->setText("Pause");
		Commander::getInstance().playSimulation();
	} else 
	{
		m_playPauseButton->setText("Play");
		Commander::getInstance().pauseSimulation();
	}
}
void RuntimeControlPanel::step(sf::Event event)
{
	m_playPauseButton->setText("Play");
	Commander::getInstance().stepSimulation();
}
void RuntimeControlPanel::stop(sf::Event event)
{
	Commander::getInstance().stopSimulation();
}
