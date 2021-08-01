#pragma once
#include <SFML/Graphics.hpp>
class AppConfig {
private:
	AppConfig() {
		mainFont.loadFromFile("media/fonts/main.ttf");
	}
public:
	bool showFPS = false;
	sf::Font mainFont;
	static AppConfig& getInstance() {
		static AppConfig theInstance;
		return theInstance;
	}

};