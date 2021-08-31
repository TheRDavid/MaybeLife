#pragma once
#include "Panel.h"

using namespace gui;
namespace gui
{
	class Scrollbar : public Panel
	{
	public:
		sf::Color m_handleColor;
		float m_min, m_max, m_value, handleLength = 1;
		bool m_handleHover = false, m_handleMove = false;
		float m_barStartPixel = 0, m_barEndPixel = 0, m_handleDragOffset=0;
		sf::Vector2f lastCursorPosition;

		Scrollbar(sf::RenderWindow * window, sf::Vector2f position, sf::Vector2f size, float min, float max, float value);

		virtual void drawSelf(sf::Vector2f relativePosition) override;
		virtual void onMousePressed(sf::Event event) override;
		virtual void onMouseReleased(sf::Event event) override;
		virtual void onMouseMove(sf::Event event) override;
		virtual void onMouseExit(sf::Event event) override;

	private:

		sf::RectangleShape m_handleRect;
	};

}