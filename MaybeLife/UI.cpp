#include "UI.h"

UI::UI(sf::RenderWindow* renderWindow, Environment* environment)
{
	this->environment = environment;
	window = renderWindow;
	// set the color
	fpsText.setFont(AppConfig::getInstance().mainFont); // font is a sf::Font

			// set the character size
	fpsText.setCharacterSize(128); // in pixels, not points!

	// set the color
	fpsText.setFillColor(sf::Color::Red);
}

void UI::refresh()
{
	float time = fpsClock.getElapsedTime().asSeconds();
	float fps = 1.f / time;
	if (fpsCount == fpsAvgSpan) { fpsCount = 0; }
	fpsSamples[fpsCount++] = fps;
	for (int i = 0; i < fpsAvgSpan; i++) {
		avgFPS += fpsSamples[i];
	}
	avgFPS /= fpsAvgSpan;
	fpsClock.restart().asSeconds();
	string fpsString = to_string((float)((int)(avgFPS * 100)) / 100);
	fpsString.erase(fpsString.find_last_not_of('0') + 1, std::string::npos);
	if (true || AppConfig::getInstance().showFPS) {
		fpsText.setString(fpsString + " FPS at " + std::to_string(environment->entities.size()));
		window->draw(fpsText);
		
	}
}
