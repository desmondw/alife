#include "World.h"
#include "Visual.h"
#include "Settings.h"

sf::RenderWindow mainWindow;
Settings settings;
World world(settings.soundOn);
Visual visual(settings, world);

void main()
{
	srand (time(NULL));
	mainWindow.Create(sf::VideoMode(settings.WIN_X, settings.WIN_Y, 32), "alife");
	mainWindow.SetFramerateLimit(30);

	while (mainWindow.IsOpened())
    {
		sf::Event input;
		while (mainWindow.GetEvent(input))
		{
			if (input.Type == sf::Event::Closed || (input.Type == sf::Event::KeyPressed && input.Key.Code == sf::Key::Escape)) //window closed
				mainWindow.Close();
			else if (input.Type == sf::Event::KeyPressed)
			{
				switch (input.Key.Code)
				{
					case sf::Key::N :
						world.initializeWorld(settings.soundOn);
						break;
					case sf::Key::Q : //FOR TESTING
						break;
					case sf::Key::S :
						world.toggleSound(settings);
						break;
					//case sf::Key::Num1 :
					//	if(settings.limitFPSOn)
					//	{
					//		mainWindow.SetFramerateLimit(0);
					//		settings.limitFPSOn = false;
					//	}
					//	else
					//	{
					//		mainWindow.SetFramerateLimit(30);
					//		settings.limitFPSOn = true;
					//	}
					//	break;
					case sf::Key::Num1 :
						mainWindow.SetFramerateLimit(1);
						break;
					case sf::Key::Num2 :
						mainWindow.SetFramerateLimit(30);
						break;
					case sf::Key::Num3 :
						mainWindow.SetFramerateLimit(0);
						break;
				}
			}
		}

		world.progressWorld();
		visual.drawGraphics(mainWindow, world);
    }
}