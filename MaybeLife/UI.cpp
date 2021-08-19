#include "UI.h"

#include "Grid.h"
#include "AppConfig.h"

UI::UI(sf::RenderWindow* renderWindow, Environment* environment, sf::View* uiView)
{
	this->environment = environment;
	this->uiView = uiView;
	window = renderWindow;
	// set the color
	fpsText.setFont(AppConfig::getInstance().mainFont); // font is a sf::Font

			// set the character size
	fpsText.setCharacterSize(32); // in pixels, not points!
	fpsText.setPosition(35, 35);

	// set the color
	fpsText.setFillColor(sf::Color::Black);
	fpsstart = std::chrono::high_resolution_clock::now();
	upstart = std::chrono::high_resolution_clock::now();
	rectangle.setSize(sf::Vector2f(300, 300));
	rectangle.setOutlineColor(sf::Color(255, 255, 255, 205));
	rectangle.setOutlineThickness(4);
	rectangle.setFillColor(sf::Color(255, 255, 255, 145));
	rectangle.setPosition(25, 25);
}

void UI::refresh()
{
	window->setView(*uiView);
	window->draw(rectangle);
	if (drawCallsSince++ == fpsAvgSpan) {
		int ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - fpsstart).count();
		fpsString = std::to_string((float)(fpsAvgSpan * 100000 / ms) / 100);
		fpsstart = std::chrono::high_resolution_clock::now();
		drawCallsSince = 0;
	}
	if (environment->steps[0] - stepCountLast >= updateAvgSpan) {
		int ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - upstart).count();
		utString = std::to_string((float)(updateAvgSpan * 100000 / ms) / 100);
		upstart = std::chrono::high_resolution_clock::now();
		stepCountLast = environment->steps[0];
	}
	fpsString.erase(fpsString.find_last_not_of('0') + 1, std::string::npos);
	utString.erase(utString.find_last_not_of('0') + 1, std::string::npos);
	if (true || AppConfig::getInstance().showFPS) {
		fpsText.setString(std::to_string(environment->entities->size()) + " entities\n" + std::to_string(environment->entityGrid->numZones) + " zones\n" + std::to_string(environment->numThreads) + " threads\nFPS: " + fpsString + "\nUPS: " + utString);
		window->draw(fpsText);
	}
}
