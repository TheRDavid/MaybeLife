#pragma once
#include <string>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;
namespace ut {
	inline string to_string(Vector2f vector) {
		return "[" + std::to_string(vector.x) + "," + std::to_string(vector.y) + "]";
	}
	inline string to_string(Vector2i vector) {
		return "[" + std::to_string(vector.x) + "," + std::to_string(vector.y) + "]";
	}
}