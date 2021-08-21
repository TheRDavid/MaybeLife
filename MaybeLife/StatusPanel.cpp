#include "StatusPanel.h"

#include "Grid.h"

StatusPanel::StatusPanel(Environment * environment, sf::RenderWindow * window)
	: Panel(
		window, 
		sf::Vector2f(40, 40), 
		sf::Vector2f(350, 210), 
		sf::Color(255, 255, 255, 220), 
		sf::Color(140, 140, 140, 255),
		4,
		true)
{
	this->environment = environment;
	statusText = new gui::TextPanel(
		window,
		sf::Vector2f(20, 15),
		sf::Vector2f(0, 0),
		sf::Color::Transparent,
		sf::Color::Transparent,
		0,
		sf::Color(20, 20, 40, 255),
		"Uninitialized",
		24
	);
	addChild(statusText);
	name = "StatusPanel";
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

	return (hours < 9 ? "0" : "") + std::to_string(hours + 1) + ":" + (minutes < 9 ? "0" : "") + std::to_string(minutes + 1) + ":" + (seconds < 9 ? "0" : "") + std::to_string(seconds + 1) + " " + (days < 9 ? "0" : "") + std::to_string(days + 1) + "/" + (months < 9 ? "0" : "") + std::to_string(months + 1) + "/" + std::to_string(years);
}

void StatusPanel::drawSelf(sf::Vector2f relativePosition)
{
	if (drawCallsSince++ == fpsAvgSpan) {
		int ms = std::max<long long>(1, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - fpsstart).count());
		fpsString = std::to_string((float)(fpsAvgSpan * 100000 / ms) / 100);
		fpsstart = std::chrono::high_resolution_clock::now();
		drawCallsSince = 0;
	}
	if (environment->steps[0] - stepCountLast >= updateAvgSpan) {
		int ms = std::max<long long>(1, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - upstart).count());
		utString = std::to_string((float)(updateAvgSpan * 100000 / ms) / 100);
		upstart = std::chrono::high_resolution_clock::now();
		stepCountLast = environment->steps[0];
	}
	fpsString.erase(fpsString.find_last_not_of('0') + 1, std::string::npos);
	utString.erase(utString.find_last_not_of('0') + 1, std::string::npos);
	if (true || AppConfig::getInstance().showFPS) {
		statusText->setText(
			"Entities: " + shorthand(environment->entities->size()) + "\n"
			+ "Zones: " + shorthand(environment->entityGrid->numZones) + "\n"
			+ "Threads: " + std::to_string(environment->numThreads) + "\n"
			+ "FPS: " + fpsString + "\n"
			+ "UPS: " + utString + "\n"
			+ "Date: " + timeDescription(environment->steps[0]));
	}
	Panel::drawSelf(relativePosition);
}
