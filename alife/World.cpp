#include "World.h"

World::World(void)
{
}

World::~World(void)
{
}

World::World(int soundOn)
{
	initializeWorld(soundOn);
}

void World::initializeWorld(int soundOn)
{
	emptyBoard = std::vector<std::vector<Tile>>(WORLD_SIZE, std::vector<Tile>(WORLD_SIZE));
	board = emptyBoard;
	newBoard = emptyBoard;

	//audio load
	sbFoodEaten.LoadFromFile("bin/sounds/sfx/foodEaten.wav");
	sbBirth.LoadFromFile("bin/sounds/sfx/birth.wav");
	sbDeath.LoadFromFile("bin/sounds/sfx/death.wav");
	sfxFoodEaten.SetBuffer(sbFoodEaten);
	sfxBirth.SetBuffer(sbBirth);
	sfxDeath.SetBuffer(sbDeath);
	
	if (!soundOn)
	{
		sfxFoodEaten.SetVolume(0);
		sfxBirth.SetVolume(0);
		sfxDeath.SetVolume(0);
	}

	seedWorld();
}

void World::seedWorld()
{
	for (int i=0; i<board.size(); i++)
	{
		for (int j=0; j<board[0].size(); j++)
		{
			if (rand() % (WORLD_SIZE * 3) < 1)
			{
				board[i][j] = Tile();
				board[i][j].makeRandomCreature(i, j);
			}
			else if (rand() % (PLANT_GROWTH_RATE / 10) < 1)
			{
				board[i][j] = Tile();
				board[i][j].makeVegetation();
			}
			else
			{
				board[i][j] = Tile();
				board[i][j].makeEmpty();
			}
		}
	}

	newBoard = board;
}

void World::progressWorld()
{
	int randomCalcDir = rand() % 8;

	//makes it so creaturs don't always act from top to bottom, left to right
	if (randomCalcDir == 0) //top-down, left-right
	{
		for (int i=0; i<board.size(); i++)
		{
			for (int j=0; j<board[0].size(); j++)
			{
				calculateTile(i, j);
			}
		}
	}
	else if (randomCalcDir == 1) //left-right, top-down
	{
		for (int i=0; i<board.size(); i++)
		{
			for (int j=0; j<board[0].size(); j++)
			{
				calculateTile(j, i);
			}
		}
	}
	else if (randomCalcDir == 2) //bottom-up, left-right
	{
		for (int i=0; i<board.size(); i++)
		{
			for (int j=board[0].size()-1; j>0; j--)
			{
				calculateTile(i, j);
			}
		}
	}
	else if (randomCalcDir == 3) //left-right, bottom-up
	{
		for (int i=0; i<board.size(); i++)
		{
			for (int j=board[0].size()-1; j>0; j--)
			{
				calculateTile(j, i);
			}
		}
	}
	else if (randomCalcDir == 4) //top-down, right-left
	{
		for (int i=board.size()-1; i>0; i--)
		{
			for (int j=0; j<board[0].size(); j++)
			{
				calculateTile(i, j);
			}
		}
	}
	else if (randomCalcDir == 5) //right-left, top-down
	{
		for (int i=board.size()-1; i>0; i--)
		{
			for (int j=0; j<board[0].size(); j++)
			{
				calculateTile(j, i);
			}
		}
	}
	else if (randomCalcDir == 6) //bottom-up, right-left
	{
		for (int i=board.size()-1; i>0; i--)
		{
			for (int j=board[0].size()-1; j>0; j--)
			{
				calculateTile(i, j);
			}
		}
	}
	else //right-left, bottom-up
	{
		for (int i=board.size()-1; i>0; i--)
		{
			for (int j=board[0].size()-1; j>0; j--)
			{
				calculateTile(j, i);
			}
		}
	}

	board = newBoard;
}

void World::calculateTile(int x, int y)
{
	if (board[x][y].isCreature)
		calculateAI(x, y);
	else if (board[x][y].isEmpty && rand() % PLANT_GROWTH_RATE < 1)
		newBoard[x][y].makeVegetation();
}

void World::calculateAI(int x, int y)
{
	board[x][y].status.hunger--;

	if (starved(x, y)) //die if starved
		return;
	if (runAway(x, y)) //run from predators
		return;
	if (board[x][y].genetics.isCarnivore && eatCreature(x, y)) //eat creature if I'm a carnivore and a creature is nearby
		return;
	if (board[x][y].genetics.isHerbivore && eatVegetation(x, y)) //eat plant if I'm an herbivore and a plant is nearby
		return;
	if (wander(x, y)) //wander semi-aimlessly
		return;
}

bool World::starved(int x, int y)
{
	if (board[x][y].status.hunger <= 0)
	{
		newBoard[x][y].makeEmpty();
		sfxDeath.Play();
		return true;
	}
	return false;
}

//TODO: add different conditions for predator near if the thinking creature is a carnivore
bool World::runAway(int x, int y)
{
	std::vector<std::vector<int>> predatorList;
	std::vector<std::vector<int>> escapeList;

	if (testTileExists(x, y-1)) //N
		fillPredatorEscapeLists(x, y, x, y-1, predatorList, escapeList);
	if (testTileExists(x+1, y-1)) //NE
		fillPredatorEscapeLists(x, y, x+1, y-1, predatorList, escapeList);
	if (testTileExists(x+1, y)) //E
		fillPredatorEscapeLists(x, y, x+1, y, predatorList, escapeList);
	if (testTileExists(x+1, y+1)) //SE
		fillPredatorEscapeLists(x, y, x+1, y+1, predatorList, escapeList);
	if (testTileExists(x, y+1)) //S
		fillPredatorEscapeLists(x, y, x, y+1, predatorList, escapeList);
	if (testTileExists(x-1, y+1)) //SW
		fillPredatorEscapeLists(x, y, x-1, y+1, predatorList, escapeList);
	if (testTileExists(x-1, y)) //W
		fillPredatorEscapeLists(x, y, x-1, y, predatorList, escapeList);
	if (testTileExists(x-1, y-1)) //NW
		fillPredatorEscapeLists(x, y, x-1, y-1, predatorList, escapeList);

	if (predatorList.size() >= 1)
	{
		if (escapeList.size() >= 2)
		{
			std::vector<std::vector<int>> trueEscapeList;
			for (int i=0; i<escapeList.size(); i++)
			{
				if (!testPredatorClose(escapeList[i][0], escapeList[i][1]))
					trueEscapeList.push_back(escapeList[i]);
			}

			int randTile;
			if (trueEscapeList.size() > 0) //TODO: add preference to move to food tiles over empty ones
			{
				randTile = rand() % trueEscapeList.size();

				if (newBoard[trueEscapeList[randTile][0]][trueEscapeList[randTile][1]].isVegetation && board[x][y].genetics.isHerbivore)
					creatureEat(x, y, trueEscapeList[randTile][0], trueEscapeList[randTile][1]);
				else
					creatureMove(x, y, trueEscapeList[randTile][0], trueEscapeList[randTile][1]);
			}
			else
			{
				randTile = rand() % escapeList.size();
				
				if (newBoard[escapeList[randTile][0]][escapeList[randTile][1]].isVegetation && board[x][y].genetics.isHerbivore)
					creatureEat(x, y, escapeList[randTile][0], escapeList[randTile][1]);
				else
					creatureMove(x, y, escapeList[randTile][0], escapeList[randTile][1]);
			}
		}
		else if (escapeList.size() == 1)
		{
			creatureMove(x, y, escapeList[0][0], escapeList[0][1]);
		}

		return true;
	}

	return false;
}

bool World::eatVegetation(int x, int y)
{
	std::vector<std::vector<int>> tileList;
	std::vector<int> tile(2, 0);

	if (testTileExists(x, y-1) && newBoard[x][y-1].isVegetation) //N
	{
		tile[0] = x;
		tile[1] = y - 1;
		tileList.push_back(tile);
	}
	if (testTileExists(x+1, y-1) && newBoard[x+1][y-1].isVegetation) //NE
	{
		tile[0] = x + 1;
		tile[1] = y - 1;
		tileList.push_back(tile);
	}
	if (testTileExists(x+1, y) && newBoard[x+1][y].isVegetation) //E
	{
		tile[0] = x + 1;
		tile[1] = y;
		tileList.push_back(tile);
	}
	if (testTileExists(x+1, y+1) && newBoard[x+1][y+1].isVegetation) //SE
	{
		tile[0] = x + 1;
		tile[1] = y + 1;
		tileList.push_back(tile);
	}
	if (testTileExists(x, y+1) && newBoard[x][y+1].isVegetation) //S
	{
		tile[0] = x;
		tile[1] = y + 1;
		tileList.push_back(tile);
	}
	if (testTileExists(x-1, y+1) && newBoard[x-1][y+1].isVegetation) //SW
	{
		tile[0] = x - 1;
		tile[1] = y + 1;
		tileList.push_back(tile);
	}
	if (testTileExists(x-1, y) && newBoard[x-1][y].isVegetation) //W
	{
		tile[0] = x - 1;
		tile[1] = y;
		tileList.push_back(tile);
	}
	if (testTileExists(x-1, y-1) && newBoard[x-1][y-1].isVegetation) //NW
	{
		tile[0] = x - 1;
		tile[1] = y - 1;
		tileList.push_back(tile);
	}

	if (tileList.size() >= 1)
	{
		int randTile = rand() % tileList.size();
		creatureEat(x, y, tileList[randTile][0], tileList[randTile][1]);
		sfxFoodEaten.Play();
		return true;
	}
	else
		return false;
}

bool World::eatCreature(int x, int y) 
{
	std::vector<std::vector<int>> tileList;
	std::vector<int> tile(2, 0);

	if (testTileExists(x, y-1) && newBoard[x][y-1].isCreature && !board[x][y].genetics.testCloseGenetics(newBoard[x][y-1].genetics)) //N
	{
		tile[0] = x;
		tile[1] = y - 1;
		tileList.push_back(tile);
	}
	if (testTileExists(x+1, y-1) && newBoard[x+1][y-1].isCreature && !board[x][y].genetics.testCloseGenetics(newBoard[x+1][y-1].genetics)) //NE
	{
		tile[0] = x + 1;
		tile[1] = y - 1;
		tileList.push_back(tile);
	}
	if (testTileExists(x+1, y) && newBoard[x+1][y].isCreature && !board[x][y].genetics.testCloseGenetics(newBoard[x+1][y].genetics)) //E
	{
		tile[0] = x + 1;
		tile[1] = y;
		tileList.push_back(tile);
	}
	if (testTileExists(x+1, y+1) && newBoard[x+1][y+1].isCreature && !board[x][y].genetics.testCloseGenetics(newBoard[x+1][y+1].genetics)) //SE
	{
		tile[0] = x + 1;
		tile[1] = y + 1;
		tileList.push_back(tile);
	}
	if (testTileExists(x, y+1) && newBoard[x][y+1].isCreature && !board[x][y].genetics.testCloseGenetics(newBoard[x][y+1].genetics)) //S
	{
		tile[0] = x;
		tile[1] = y + 1;
		tileList.push_back(tile);
	}
	if (testTileExists(x-1, y+1) && newBoard[x-1][y+1].isCreature && !board[x][y].genetics.testCloseGenetics(newBoard[x-1][y+1].genetics)) //SW
	{
		tile[0] = x - 1;
		tile[1] = y + 1;
		tileList.push_back(tile);
	}
	if (testTileExists(x-1, y) && newBoard[x-1][y].isCreature && !board[x][y].genetics.testCloseGenetics(newBoard[x-1][y].genetics)) //W
	{
		tile[0] = x - 1;
		tile[1] = y;
		tileList.push_back(tile);
	}
	if (testTileExists(x-1, y-1) && newBoard[x-1][y-1].isCreature && !board[x][y].genetics.testCloseGenetics(newBoard[x-1][y-1].genetics)) //NW
	{
		tile[0] = x - 1;
		tile[1] = y - 1;
		tileList.push_back(tile);
	}

	if (tileList.size() >= 1)
	{
		int randTile = rand() % tileList.size();
		creatureEat(x, y, tileList[randTile][0], tileList[randTile][1]);
		sfxDeath.Play();
		return true;
	}
	else
		return false;
}

bool World::wander(int x, int y)
{
	const int STRAIGHT_MOVE_EMPHASIS = 20;
	const int DIVERGENT_MOVE_EMPHASIS = STRAIGHT_MOVE_EMPHASIS / 2;

	std::vector<std::vector<int>> tileList;
	std::vector<int> tile(2, 0);

	if (testTileExists(x, y-1) && newBoard[x][y-1].isEmpty) //N
	{
		tile[0] = x;
		tile[1] = y - 1;
		tileList.push_back(tile);

		//direction enforcement
		if (board[x][y].status.lastLocation[1] == y + 1)
		{
			for (int i=0; i<DIVERGENT_MOVE_EMPHASIS; i++)
				tileList.push_back(tile);

			if (board[x][y].status.lastLocation[0] == x)
			{
				for (int i=0; i<STRAIGHT_MOVE_EMPHASIS; i++)
					tileList.push_back(tile);
			}
		}
	}
	if (testTileExists(x+1, y-1) && newBoard[x+1][y-1].isEmpty) //NE
	{
		tile[0] = x + 1;
		tile[1] = y - 1;
		tileList.push_back(tile);

		//direction enforcement
		if (board[x][y].status.lastLocation[0] == x - 1 && board[x][y].status.lastLocation[1] == y + 1)
		{
			for (int i=0; i<STRAIGHT_MOVE_EMPHASIS; i++)
				tileList.push_back(tile);

		}
		else if (board[x][y].status.lastLocation[0] == x - 1 && board[x][y].status.lastLocation[1] == y)
		{
			for (int i=0; i<DIVERGENT_MOVE_EMPHASIS; i++)
				tileList.push_back(tile);
		}
		else if (board[x][y].status.lastLocation[0] == x && board[x][y].status.lastLocation[1] == y + 1)
		{
			for (int i=0; i<DIVERGENT_MOVE_EMPHASIS; i++)
				tileList.push_back(tile);
		}
	}
	if (testTileExists(x+1, y) && newBoard[x+1][y].isEmpty) //E
	{
		tile[0] = x + 1;
		tile[1] = y;
		tileList.push_back(tile);

		//direction enforcement
		if (board[x][y].status.lastLocation[0] == x - 1)
		{
			for (int i=0; i<DIVERGENT_MOVE_EMPHASIS; i++)
				tileList.push_back(tile);

			if (board[x][y].status.lastLocation[1] == y)
			{
				for (int i=0; i<STRAIGHT_MOVE_EMPHASIS; i++)
					tileList.push_back(tile);
			}
		}
	}
	if (testTileExists(x+1, y+1) && newBoard[x+1][y+1].isEmpty) //SE
	{
		tile[0] = x + 1;
		tile[1] = y + 1;
		tileList.push_back(tile);

		//direction enforcement
		if (board[x][y].status.lastLocation[0] == x - 1 && board[x][y].status.lastLocation[1] == y - 1)
		{
			for (int i=0; i<STRAIGHT_MOVE_EMPHASIS; i++)
				tileList.push_back(tile);
		}
		else if (board[x][y].status.lastLocation[0] == x - 1 && board[x][y].status.lastLocation[1] == y)
		{
			for (int i=0; i<DIVERGENT_MOVE_EMPHASIS; i++)
				tileList.push_back(tile);
		}
		else if (board[x][y].status.lastLocation[0] == x && board[x][y].status.lastLocation[1] == y - 1)
		{
			for (int i=0; i<DIVERGENT_MOVE_EMPHASIS; i++)
				tileList.push_back(tile);
		}
	}
	if (testTileExists(x, y+1) && newBoard[x][y+1].isEmpty) //S
	{
		tile[0] = x;
		tile[1] = y + 1;
		tileList.push_back(tile);

		//direction enforcement
		if (board[x][y].status.lastLocation[1] == y - 1)
		{
			for (int i=0; i<DIVERGENT_MOVE_EMPHASIS; i++)
				tileList.push_back(tile);

			if (board[x][y].status.lastLocation[0] == x)
			{
				for (int i=0; i<STRAIGHT_MOVE_EMPHASIS; i++)
					tileList.push_back(tile);
			}
		}
	}
	if (testTileExists(x-1, y+1) && newBoard[x-1][y+1].isEmpty) //SW
	{
		tile[0] = x - 1;
		tile[1] = y + 1;
		tileList.push_back(tile);

		//direction enforcement
		if (board[x][y].status.lastLocation[0] == x + 1 && board[x][y].status.lastLocation[1] == y - 1)
		{
			for (int i=0; i<STRAIGHT_MOVE_EMPHASIS; i++)
				tileList.push_back(tile);
		}
		else if (board[x][y].status.lastLocation[0] == x + 1 && board[x][y].status.lastLocation[1] == y)
		{
			for (int i=0; i<DIVERGENT_MOVE_EMPHASIS; i++)
				tileList.push_back(tile);
		}
		else if (board[x][y].status.lastLocation[0] == x && board[x][y].status.lastLocation[1] == y - 1)
		{
			for (int i=0; i<DIVERGENT_MOVE_EMPHASIS; i++)
				tileList.push_back(tile);
		}
	}
	if (testTileExists(x-1, y) && newBoard[x-1][y].isEmpty) //W
	{
		tile[0] = x - 1;
		tile[1] = y;
		tileList.push_back(tile);

		//direction enforcement
		if (board[x][y].status.lastLocation[0] == x + 1)
		{
			for (int i=0; i<DIVERGENT_MOVE_EMPHASIS; i++)
				tileList.push_back(tile);

			if (board[x][y].status.lastLocation[1] == y)
			{
				for (int i=0; i<STRAIGHT_MOVE_EMPHASIS; i++)
					tileList.push_back(tile);
			}
		}
	}
	if (testTileExists(x-1, y-1) && newBoard[x-1][y-1].isEmpty) //NW
	{
		tile[0] = x - 1;
		tile[1] = y - 1;
		tileList.push_back(tile);

		//direction enforcement
		if (board[x][y].status.lastLocation[0] == x + 1 && board[x][y].status.lastLocation[1] == y + 1)
		{
			for (int i=0; i<STRAIGHT_MOVE_EMPHASIS; i++)
				tileList.push_back(tile);
		}
		else if (board[x][y].status.lastLocation[0] == x + 1 && board[x][y].status.lastLocation[1] == y)
		{
			for (int i=0; i<DIVERGENT_MOVE_EMPHASIS; i++)
				tileList.push_back(tile);
		}
		else if (board[x][y].status.lastLocation[0] == x && board[x][y].status.lastLocation[1] == y + 1)
		{
			for (int i=0; i<DIVERGENT_MOVE_EMPHASIS; i++)
				tileList.push_back(tile);
		}
	}

	if (tileList.size() >= 1)
	{
		int randTile = rand() % tileList.size();
		creatureMove(x, y, tileList[randTile][0], tileList[randTile][1]);
		return true;
	}

	//don't move
	newBoard[x][y] = board[x][y];

	return false;
}

void World::creatureMove(int x, int y, int newX, int newY)
{
	board[x][y].status.setLastLocation(x, y);
	newBoard[newX][newY] = board[x][y];
	newBoard[x][y].makeEmpty();
}

void World::creatureEat(int x, int y, int foodX, int foodY)
{
	if (newBoard[foodX][foodY].isVegetation)
		board[x][y].status.hunger += HUNGER_FILLED_PLANT;
	else
		board[x][y].status.hunger += HUNGER_FILLED_CREATURE;

	board[x][y].status.reproduction--;

	if (board[x][y].status.reproduction <= 0)
		reproduce(x, y, foodX, foodY);
	else
		creatureMove(x, y, foodX, foodY);
}

bool World::testTileExists(int x, int y)
{
	if (x >= 0 && x < WORLD_SIZE)
		if (y >= 0 && y < WORLD_SIZE)
			return true;
	return false;
}

void World::fillPredatorEscapeLists(int creatureX, int creatureY, int x, int y, std::vector<std::vector<int>>& predatorList, std::vector<std::vector<int>>& escapeList)
{
	std::vector<int> tile(2, 0);
	tile[0] = x;
	tile[1] = y;
	if (newBoard[x][y].isCreature && newBoard[x][y].genetics.isCarnivore && !board[creatureX][creatureY].genetics.testCloseGenetics(newBoard[x][y].genetics))
		predatorList.push_back(tile);
	else if (newBoard[x][y].isEmpty || newBoard[x][y].isVegetation)
		escapeList.push_back(tile);
}

bool World::testPredatorClose(int x, int y)
{
	if (testTileExists(x, y-1) && newBoard[x][y-1].isCreature && newBoard[x][y-1].genetics.isCarnivore && !board[x][y].genetics.testCloseGenetics(newBoard[x][y-1].genetics)) //N
		return true;
	if (testTileExists(x+1, y-1) && newBoard[x+1][y-1].isCreature && newBoard[x+1][y-1].genetics.isCarnivore && !board[x][y].genetics.testCloseGenetics(newBoard[x+1][y-1].genetics)) //NE
		return true;
	if (testTileExists(x+1, y) && newBoard[x+1][y].isCreature && newBoard[x+1][y].genetics.isCarnivore && !board[x][y].genetics.testCloseGenetics(newBoard[x+1][y].genetics)) //E
		return true;
	if (testTileExists(x+1, y+1) && newBoard[x+1][y+1].isCreature && newBoard[x+1][y+1].genetics.isCarnivore && !board[x][y].genetics.testCloseGenetics(newBoard[x+1][y+1].genetics)) //SE
		return true;
	if (testTileExists(x, y+1) && newBoard[x][y+1].isCreature && newBoard[x][y+1].genetics.isCarnivore && !board[x][y].genetics.testCloseGenetics(newBoard[x][y+1].genetics)) //S
		return true;
	if (testTileExists(x-1, y+1) && newBoard[x-1][y+1].isCreature && newBoard[x-1][y+1].genetics.isCarnivore && !board[x][y].genetics.testCloseGenetics(newBoard[x-1][y+1].genetics)) //SW
		return true;
	if (testTileExists(x-1, y) && newBoard[x-1][y].isCreature && newBoard[x-1][y].genetics.isCarnivore && !board[x][y].genetics.testCloseGenetics(newBoard[x-1][y].genetics)) //W
		return true;
	if (testTileExists(x-1, y-1) && newBoard[x-1][y-1].isCreature && newBoard[x-1][y-1].genetics.isCarnivore && !board[x][y].genetics.testCloseGenetics(newBoard[x-1][y-1].genetics)) //NW
		return true;

	return false;
}

void World::reproduce(int parentX, int parentY, int childX, int childY)
{
	//reset reproduction meter
	board[parentX][parentY].status.reproduction = board[parentX][parentY].genetics.reproduction;
	
	//create offspring
	Tile baby;
	baby.makeCreature(childX, childY, board[parentX][parentY].genetics);
	baby.status.lastLocation[0] = childX;
	baby.status.lastLocation[1] = childY;

	newBoard[childX][childY] = baby;
	sfxBirth.Play();
}

void World::toggleSound(Settings& settings)
{
	if(settings.soundOn)
	{
		sfxFoodEaten.SetVolume(0);
		sfxBirth.SetVolume(0);
		sfxDeath.SetVolume(0);
		settings.soundOn = false;
	}
	else
	{
		sfxFoodEaten.SetVolume(100);
		sfxBirth.SetVolume(100);
		sfxDeath.SetVolume(100);
		settings.soundOn = true;
	}
}