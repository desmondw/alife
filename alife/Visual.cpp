#include "Visual.h"

Visual::Visual(void)
{
	totalFPS = 0;
	frameCount = 0;
}

Visual::~Visual(void)
{
}

Visual::Visual(Settings& settings, World& world)
{
	font.LoadFromFile("bin/arial.ttf", 30);
	pixelSizeX = static_cast<float>(settings.WIN_X) / world.WORLD_SIZE;
	pixelSizeY = static_cast<float>(settings.WIN_Y) / world.WORLD_SIZE;
}

void Visual::drawGraphics(sf::RenderWindow& mainWindow, World& world)
{
	//mainWindow.Clear(sf::Color(50, 50, 50, 255)); //clear screen
	mainWindow.Clear(sf::Color::Black);

	drawWorld(mainWindow, world);
	textDisplay(mainWindow);

    mainWindow.Display(); //draw screen
}

void Visual::drawWorld(sf::RenderWindow& mainWindow, World& world)
{
	for (int i=0; i<world.WORLD_SIZE; i++)
	{
		for (int j=0; j<world.WORLD_SIZE; j++)
		{
			color = world.board[i][j].color;
			if (color[0] > 0 || color[1] > 0 || color[2] > 0)
				mainWindow.Draw(sf::Shape::Rectangle(i*pixelSizeX, j*pixelSizeY, (i+1)*pixelSizeX, (j+1)*pixelSizeY, sf::Color(color[0], color[1], color[2], 255)));
		}
	}
}

void Visual::textDisplay(sf::RenderWindow& mainWindow)
{
	//draw text
	sf::String text("", font, 15);
	text.SetPosition(5, 5);
	std::ostringstream sstream;

	int fps = static_cast<int>(1 / mainWindow.GetFrameTime());
	sstream << "FPS: ";
	sstream << fps;

	if (totalFPS > 4000000000 || frameCount > 700)
	{
		totalFPS = 0;
		frameCount = 0;
	}
	if (fps >= 0)
		totalFPS += fps;
	frameCount++;
	
	sstream << "\nAvg FPS: ";
	sstream << totalFPS / frameCount;

	text.SetText(sstream.str());
	sstream.end;
	
	text.SetColor(sf::Color::Green);
	mainWindow.Draw(text);
}

bool Visual::rowFilled(World& world, int x, int y)
{
	if (x < world.board.size())
	{
		if (world.board[x][y].color == world.board[x-1][y].color)
			return rowFilled(world, x+1, y);
		return false;
	}

	return true;
}