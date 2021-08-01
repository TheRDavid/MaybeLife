#pragma once
#define TAB_DOWN (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
#define SHIFT_DOWN (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
#define CONTROL_DOWN (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
#define KEY_DOWN(KEY) sf::Keyboard::isKeyPressed(sf::Keyboard::KEY)

#include <SFML/Graphics.hpp>

using namespace sf;