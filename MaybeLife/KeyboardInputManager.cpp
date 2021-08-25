#include "KeyboardInputManager.h"

#include <iostream>

#include "Utilities.h"

KeyboardInputManager::KeyboardInputManager(Environment * environment, sf::RenderWindow * window, sf::View * sceneView, sf::View * guiView)
{
	this->m_environment = environment;
	this->m_window = window;
	this->m_sceneView = sceneView;
	this->m_guiView = guiView;
}

void KeyboardInputManager::handle(sf::Event event)
{
	if (event.type == sf::Event::KeyPressed) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
			m_window->setView(*m_sceneView);
			sf::Vector2i pixelPos = sf::Mouse::getPosition(*m_window);
			sf::Vector2f worldPos = m_window->mapPixelToCoords(pixelPos);

			float relX = 1 - (m_sceneView->getCenter().x + m_sceneView->getSize().x / 2 - worldPos.x) / m_sceneView->getSize().x;
			float relY = 1 - (m_sceneView->getCenter().y + m_sceneView->getSize().y / 2 - worldPos.y) / m_sceneView->getSize().y;
			sf::Vector2f relativePos = sf::Vector2f(relX, relY);
			m_sceneView->zoom(0.9f);

			pixelPos = sf::Mouse::getPosition(*m_window);
			worldPos = m_window->mapPixelToCoords(pixelPos);

			float relX2 = 1 - (m_sceneView->getCenter().x + m_sceneView->getSize().x / 2 - worldPos.x) / m_sceneView->getSize().x;
			float relY2 = 1 - (m_sceneView->getCenter().y + m_sceneView->getSize().y / 2 - worldPos.y) / m_sceneView->getSize().y;
			sf::Vector2f relativePos2 = sf::Vector2f(relX2, relY2);

			sf::Vector2f relDelta = (relativePos2 - relativePos);
			m_sceneView->setCenter(m_sceneView->getCenter() + sf::Vector2f(relDelta.x * m_sceneView->getSize().x, relDelta.y * m_sceneView->getSize().y));
			m_environment->adjustRenderingRect();
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
			m_window->setView(*m_sceneView);
			sf::Vector2i pixelPos = sf::Mouse::getPosition(*m_window);
			sf::Vector2f worldPos = m_window->mapPixelToCoords(pixelPos);

			float relX = 1 - (m_sceneView->getCenter().x + m_sceneView->getSize().x / 2 - worldPos.x) / m_sceneView->getSize().x;
			float relY = 1 - (m_sceneView->getCenter().y + m_sceneView->getSize().y / 2 - worldPos.y) / m_sceneView->getSize().y;
			sf::Vector2f relativePos = sf::Vector2f(relX, relY);
			m_sceneView->zoom(1.1f);
			pixelPos = sf::Mouse::getPosition(*m_window);
			worldPos = m_window->mapPixelToCoords(pixelPos);

			float relX2 = 1 - (m_sceneView->getCenter().x + m_sceneView->getSize().x / 2 - worldPos.x) / m_sceneView->getSize().x;
			float relY2 = 1 - (m_sceneView->getCenter().y + m_sceneView->getSize().y / 2 - worldPos.y) / m_sceneView->getSize().y;
			sf::Vector2f relativePos2 = sf::Vector2f(relX2, relY2);

			sf::Vector2f relDelta = (relativePos2 - relativePos);
			m_sceneView->setCenter(m_sceneView->getCenter() + sf::Vector2f(relDelta.x * m_sceneView->getSize().x, relDelta.y * m_sceneView->getSize().y));
			m_environment->adjustRenderingRect();
		}
	}
}