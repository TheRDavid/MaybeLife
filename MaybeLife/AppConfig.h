#pragma once
#include <SFML/Graphics.hpp>
class AppConfig {
public:
	bool showFPS = false;
	sf::Font mainFont;
	static AppConfig& getInstance() {
		static AppConfig theInstance;
		return theInstance;
	}
private:
	AppConfig() {
		mainFont.loadFromFile("media/fonts/main.otf");
	}

};