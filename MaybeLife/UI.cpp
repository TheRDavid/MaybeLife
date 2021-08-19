#include "UI.h"
UI::UI(sf::RenderWindow* renderWindow, Environment* environment)
{
	this->environment = environment;
	window = renderWindow;
	// set the color
	fpsText.setFont(AppConfig::getInstance().mainFont); // font is a sf::Font

			// set the character size
	fpsText.setCharacterSize(32); // in pixels, not points!
	fpsText.setPosition(35, 35);

	// set the color
	fpsText.setFillColor(sf::Color::Black);
	fpsstart = high_resolution_clock::now();
	upstart = high_resolution_clock::now();
	rectangle.setSize(sf::Vector2f(300, 300));
	rectangle.setOutlineColor(Color(255, 255, 255, 205));
	rectangle.setOutlineThickness(4);
	rectangle.setFillColor(Color(255, 255, 255, 145));
	rectangle.setPosition(25, 25);
}

void UI::refresh()
{
	window->draw(rectangle);
	if (drawCallsSince++ == fpsAvgSpan) {
		int ms = duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - fpsstart).count();
		fpsString = to_string((float)(fpsAvgSpan * 100000 / ms) / 100);
		fpsstart = high_resolution_clock::now();
		drawCallsSince = 0;
	}
	if (environment->steps[0] - stepCountLast >= updateAvgSpan) {
		int ms = duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - upstart).count();
		utString = to_string((float)(updateAvgSpan * 100000 / ms) / 100);
		upstart = high_resolution_clock::now();
		stepCountLast = environment->steps[0];
	}
	fpsString.erase(fpsString.find_last_not_of('0') + 1, std::string::npos);
	utString.erase(utString.find_last_not_of('0') + 1, std::string::npos);
	if (true || AppConfig::getInstance().showFPS) {
		fpsText.setString(std::to_string(environment->entities->size()) + " entities\n" + to_string(environment->numZones) + " zones\n" + to_string(environment->numThreads) + " threads\nFPS: " + fpsString + "\nUPS: " + utString);
		window->draw(fpsText);
	}
}
