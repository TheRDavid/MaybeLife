#include "UI.h"

#include <math.h>

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
	fpsText.setCharacterSize(24); // in pixels, not points!
	fpsText.setPosition(35, 35);

	// set the color
	fpsText.setFillColor(sf::Color::Black);
	fpsstart = std::chrono::high_resolution_clock::now();
	upstart = std::chrono::high_resolution_clock::now();
	rectangle.setSize(sf::Vector2f(350, 200));
	rectangle.setOutlineColor(sf::Color(255, 255, 255, 205));
	rectangle.setOutlineThickness(4);
	rectangle.setFillColor(sf::Color(255, 255, 255, 145));
	rectangle.setPosition(25, 25);
}

inline std::string shorthand(int num) {
	if (num <= 1000) {
		return std::to_string(num);
	}
	float shorthandCount;
	std::string multiplierString = "";
	if (num > 1000 * 1000 * 1000) {
		shorthandCount = (float)num / (1000.0 * 1000.0 * 1000.0);
		multiplierString = " B";
	}
	else if (num > 1000 * 1000) {
		shorthandCount = (float)num / (1000.0 * 1000.0);
		multiplierString = " M";
	}
	else if (num > 1000) {
		shorthandCount = (float)num / (1000.0);
		multiplierString = " K";
	}
	std::string countText = std::to_string(shorthandCount).substr(0, 5);
	countText.erase(countText.find_last_not_of('0') + 1, std::string::npos);
	countText.erase(countText.find_last_not_of('.') + 1, std::string::npos);
	return countText + multiplierString;
}

inline std::string timeDescription(int timeSteps) {
	int year = 365 * 24 * 60 * 60;
	int month = year / 12;
	int day = month / 30;
	int hour = day / 24;
	int minute = hour / 60;
	int second = minute / 60;

	int years = timeSteps / year;
	timeSteps -= years * year;

	int months = timeSteps / month;
	timeSteps -= months * month;

	int days = timeSteps / day;
	timeSteps -= days * day;

	int hours = timeSteps / hour;
	timeSteps -= hours * hour;

	int minutes = timeSteps / minute;
	timeSteps -= minutes * minute;

	int seconds = timeSteps / second;
	timeSteps -= seconds * second;

	return (hours < 9 ? "0" : "") + std::to_string(hours+1) + ":" + (minutes < 9 ? "0" : "") + std::to_string(minutes+1) + ":" + (seconds < 9 ? "0" : "") + std::to_string(seconds+1) + " " + (days < 9 ? "0" : "") + std::to_string(days+1) + "/" + (months < 9 ? "0" : "") + std::to_string(months+1) + "/" + std::to_string(years);
}

void UI::refresh()
{
	window->setView(*uiView);
	window->draw(rectangle);
	if (drawCallsSince++ == fpsAvgSpan) {
		int ms = std::max<long long>(1,std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - fpsstart).count());
		fpsString = std::to_string((float)(fpsAvgSpan * 100000 / ms) / 100);
		fpsstart = std::chrono::high_resolution_clock::now();
		drawCallsSince = 0;
	}
	if (environment->steps[0] - stepCountLast >= updateAvgSpan) {
		int ms = std::max<long long>(1,std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - upstart).count());
		utString = std::to_string((float)(updateAvgSpan * 100000 / ms) / 100);
		upstart = std::chrono::high_resolution_clock::now();
		stepCountLast = environment->steps[0];
	}
	fpsString.erase(fpsString.find_last_not_of('0') + 1, std::string::npos);
	utString.erase(utString.find_last_not_of('0') + 1, std::string::npos);
	if (true || AppConfig::getInstance().showFPS) {
		fpsText.setString(
			"Entities: "+shorthand(environment->entities->size()) + "\n"
			+"Zones: "+ shorthand(environment->entityGrid->numZones) + "\n"
			+ "Threads: " + std::to_string(environment->numThreads) + "\n"
			+ "FPS: " + fpsString + "\n"
			+ "UPS: " + utString + "\n"
			+ "Date: " + timeDescription(environment->steps[0]));
		window->draw(fpsText);
	}
}
