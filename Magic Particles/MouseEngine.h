#pragma once
#include <SDL_image.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "ColorCursor.h"

namespace Core {
	enum CoreRank {
		Black,
		DarkRed,
		SolidRed,
		LightRed,
		DarkOrange,
		SolidOrange,
		LightOrange,
		DarkYellow,
		SolidYellow,
		LightYellow,
		DarkSilver,
		SolidSilver,
		LightSilver,
		White,
		Purple,
	};

	static const sf::Color CORECOLORS[] = {
	sf::Color(0, 0, 0),
	sf::Color(158, 0, 0),
	sf::Color(255, 0, 0),
	sf::Color(255, 137, 137),
	sf::Color(145, 90, 0),
	sf::Color(255, 132, 0),
	sf::Color(255, 180, 60),
	sf::Color(202, 202, 0),
	sf::Color(255, 255, 0),
	sf::Color(255, 255, 129),
	sf::Color(114, 144, 114),
	sf::Color(185, 185, 185),
	sf::Color(246, 246, 246),
	sf::Color(254, 254, 254),
	sf::Color(323, 100, 58),
	};
}
const static sf::Color HEARTCOLOR(255, 0, 0);

class MouseEngine {

public:
	MouseEngine();


	sf::Color getCoreColor() { return getCoreColorFromCoreRank(coreRank); }

	void setCoreRank(Core::CoreRank newRank);

	void update();

	unsigned int getPoints() { return points; }
	void setPoints(unsigned int newPoints) { points = newPoints; };
	void addPoints(unsigned int toAdd) { points += toAdd; }
	void removePoints(unsigned int toRemove) { points -= toRemove; }
	
private:

	bool shouldUpdate;

	void updateRankFromPoints();

	void setCursorFromSFMLImage(sf::Image* image);

	sf::Image* baseCursor;
	sf::Image* modifiedCursor;

	Core::CoreRank coreRank;

	sf::Color getCoreColorFromCoreRank(Core::CoreRank rank) { return Core::CORECOLORS[rank]; }
	void processImage();


	unsigned int points;
};

