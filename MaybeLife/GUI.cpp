#include "GUI.h"

#include "Utilities.h"
using namespace gui;
GUI::GUI(sf::RenderWindow * window, sf::View * guiView, Commander * commander)
{
	this->window = window;
	this->guiView = guiView;
	this->commander = commander;
	mainPanel = new Panel(window, sf::Vector2f(0,0), guiView->getSize(), sf::Color::Transparent, sf::Color::Transparent, 0, false);
	mainPanel->name = "MainPanel";
}

void GUI::update()
{
	window->setView(*guiView);
	mainPanel->Element::draw();
}

void GUI::handle(sf::Event event)
{
	window->setView(*guiView);
	mainPanel->Element::handle(event);
}
