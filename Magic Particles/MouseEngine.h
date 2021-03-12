#pragma once
#include <SDL_image.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "ColorCursor.h"

class MouseEngine {

public:
	MouseEngine();
	
	void setCursorFromSFMLImage(sf::Image& image);
private:

	sf::Image baseCursor;

	
};

