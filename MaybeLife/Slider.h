#pragma once
#include "Element.h"

#include "GUI.h"
namespace gui {
	class Slider :
		public Element
	{
	public:
		action m_onValueChange;
		float m_minValue, m_maxValue, m_value;
		float m_gripRadius = 8;
		sf::Color m_color;
		bool m_scrubbing = false, m_gripHover = false;
		sf::Vector2f m_gripPosition;
		float m_sliderStartPixel, m_sliderEndPixel;
		Slider(sf::RenderWindow* window, sf::Vector2f position, sf::Vector2f size, float value, float max, float min, sf::Color color, action onValueChange);
		virtual void drawSelf(sf::Vector2f relativePosition);
	private:
		virtual void onMousePressed(sf::Event event) override;
		virtual void onMouseReleased(sf::Event event) override;
		virtual void onMouseMove(sf::Event event) override;
		virtual void onMouseExit(sf::Event event) override;
	};
}