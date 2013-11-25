#ifndef _VISUAL_H
#define _VISUAL_H

#include <SFML/Graphics.hpp>
#include <sstream>
#include <vector>
#include "Settings.h"
#include "World.h"

class Visual
{
	private:
		unsigned int totalFPS;
		unsigned int frameCount;

	public:
		sf::Font font;
		std::vector<int> color;
		float pixelSizeX, pixelSizeY;

		Visual(void);
		~Visual(void);
		Visual(Settings& settings, World& world);
		void drawGraphics(sf::RenderWindow& mainWindow, World& world);
		void drawWorld(sf::RenderWindow& mainWindow, World& world);
		void textDisplay(sf::RenderWindow& mainWindow);
		bool rowFilled(World& world, int x, int y);
};

#endif