#include "MouseInputManager.h"

#include "Utilities.h"
#include "Grid.h"
#include "Peasant.h"

MouseInputManager::MouseInputManager(Environment * environment, sf::RenderWindow * window, sf::View * sceneView, sf::View * uiView)
{
	this->m_environment = environment;
	this->m_window = window;
	this->m_sceneView = sceneView;
	this->m_guiView = uiView;
	Commander::getInstance().registerMouseInputManager(this);
}

void MouseInputManager::handle(sf::Event event)
{
	if (event.type == sf::Event::MouseMoved) {
		m_window->setView(*m_sceneView);
		m_mousePixelPos = sf::Mouse::getPosition(*m_window);
		m_mouseWorldPos = m_window->mapPixelToCoords(m_mousePixelPos);

		if (m_makingArealSelection)
		{
			m_arealSelectionStop = m_mouseWorldPos;
		}
	}

	if (event.type == sf::Event::Closed)
		Commander::getInstance().stopSimulation();

	if (!Commander::getInstance().isGUIBlockingCursor())
	{
		if (event.type == sf::Event::MouseMoved) {
			m_window->setView(*m_sceneView);
			sf::Vector2i pixelPos = sf::Mouse::getPosition(*m_window);

			// convert it to world coordinates
			sf::Vector2f worldPos = m_window->mapPixelToCoords(pixelPos);
			Commander::getInstance().selectEntityAt_hover(worldPos);
			Commander::getInstance().setSelectedZone_hover(m_environment->m_entityGrid->zoneAt(worldPos));

			if (m_dragging) {
				m_window->setView(*m_guiView);
				sf::Vector2i pixelPos = sf::Mouse::getPosition(*m_window);

				sf::Vector2f worldPos = m_window->mapPixelToCoords(pixelPos);
				m_endDragPos = worldPos;
				float currentZoom = (m_sceneView->getSize().x / m_environment->m_size.x + m_sceneView->getSize().y / m_environment->m_size.y) / 2;
				sf::Vector2f delta = sf::Vector2f((m_startDragPos.x - m_endDragPos.x) * currentZoom, (m_startDragPos.y - m_endDragPos.y) * currentZoom);
				//std::cout << "In drag, delta = " << ut::to_string(delta) << " at zoom " << currentZoom << std::endl;
				m_sceneView->move(delta);
				m_startDragPos = m_endDragPos;
				//std::cout << "VP: center=" << ut::to_string(m_sceneView->getCenter()) << ", rect=" << ut::to_string(m_sceneView->getSize()) << std::endl;
				m_environment->adjustRenderingRect();
			}

		}

		if (event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == sf::Mouse::Button::Right) {
				m_window->setView(*m_guiView);
				sf::Vector2i pixelPos = sf::Mouse::getPosition(*m_window);
				sf::Vector2f worldPos = m_window->mapPixelToCoords(pixelPos);
				m_startDragPos = worldPos;
				Commander::getInstance().selectEntityAt(worldPos);
				Commander::getInstance().setSelectedZone(m_environment->m_entityGrid->zoneAt(worldPos));
				m_dragging = true;
			}
			else if (event.mouseButton.button == sf::Mouse::Button::Middle) {
				m_window->setView(*m_sceneView);
				sf::Vector2i pixelPos = sf::Mouse::getPosition(*m_window);
				sf::Vector2f worldPos = m_window->mapPixelToCoords(pixelPos);
				//Commander::getInstance().addEntity(std::make_shared<Peasant>(m_environment, worldPos));
			}
			else if (event.mouseButton.button == sf::Mouse::Button::Left) {
				m_window->setView(*m_sceneView);
				sf::Vector2i pixelPos = sf::Mouse::getPosition(*m_window);
				sf::Vector2f worldPos = m_window->mapPixelToCoords(pixelPos);
				Commander::getInstance().selectZoneAt(worldPos);
				Commander::getInstance().selectEntityAt(worldPos);
				if (!m_makingArealSelection)
				{
					m_makingArealSelection = true;
					m_arealSelectionStart = worldPos;
					m_arealSelectionStop = worldPos;
				}
			}
		}
		if (event.type == sf::Event::MouseButtonReleased) {
			m_dragging = false;
			if (event.mouseButton.button == sf::Mouse::Button::Left) {
				m_makingArealSelection = false;
				m_activeArealSelection = true;
				sf::Vector2i pixelPos = sf::Mouse::getPosition(*m_window);
				sf::Vector2f worldPos = m_window->mapPixelToCoords(pixelPos);
			}
		}
	}
	//std::cout << "Dragging: " << m_dragging << " from " << ut::to_string(m_startDragPos) << std::endl;

}
