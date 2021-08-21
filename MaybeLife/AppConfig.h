#pragma once
#include <SFML/Graphics.hpp>
class AppConfig {
public:
	bool m_showFPS = false;
	sf::Font m_mainFont;
	static AppConfig& getInstance() {
		static AppConfig theInstance;
		return theInstance;
	}
private:
	AppConfig() {
		m_mainFont.loadFromFile("media/fonts/main.otf");
	}

};