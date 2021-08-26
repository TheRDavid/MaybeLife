#pragma once

#include <SFML/Graphics.hpp>

#include <functional>

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

		Panel* m_mainPanel;
		Element* m_focus = nullptr;

	private:
		sf::RenderWindow* m_window;
		sf::View* m_guiView;
	};

}