#include "GUI.h"

#include "Utilities.h"
using namespace gui;
GUI::GUI(sf::RenderWindow * window, sf::View * guiView)
{
	this->m_window = window;
	this->m_guiView = guiView;
	m_mainPanel = new Panel(m_window, sf::Vector2f(0,0), m_guiView->getSize(), sf::Color::Transparent, sf::Color::Transparent, 0, false);
	m_mainPanel->m_name = "MainPanel";
}

void GUI::update()
{
	m_window->setView(*m_guiView);
	m_mainPanel->Element::draw();
}

void GUI::handle(sf::Event event)
{
	m_window->setView(*m_guiView);
	m_mainPanel->Element::handle(event);
}
