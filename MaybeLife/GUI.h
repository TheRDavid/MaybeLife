#pragma once

#include <SFML/Graphics.hpp>

#include <functional>

#include "Commander.h"
#include "Panel.h"
#include "Element.h"

namespace gui
{
	using namespace gui;
	typedef std::function<void(sf::Event)> action;
	class GUI
	{
	public:
		GUI(sf::RenderWindow* window, sf::View* guiView);
		void update();
		void handle(sf::Event event);

		gui::Panel* m_mainPanel;

	private:
		sf::RenderWindow* m_window;
		sf::View* m_guiView;
	};

}