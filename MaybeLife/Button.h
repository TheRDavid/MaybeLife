#pragma once

#include "TextPanel.h"

#include <functional>

#include "Utilities.h"

using namespace gui;
namespace gui {
	class Button : public TextPanel
	{
	public:
		std::string m_name = "TextPanel";
		sf::Color m_normalColor;
		Button(sf::RenderWindow * window, sf::Vector2f position, sf::Vector2f size, std::string text, std::function<void(sf::Event)> action, sf::Color normalColor);

		virtual void drawSelf(sf::Vector2f relativePosition) override;
	private:
		std::function<void(sf::Event)> m_action;
		virtual void onMouseEnter(sf::Event event) override;
		virtual void onMouseExit(sf::Event event) override;
		virtual void onMousePressed(sf::Event event) override;
		virtual void onMouseReleased(sf::Event event) override;
	};

}