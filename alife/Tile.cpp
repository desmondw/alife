#include "Tile.h"

Tile::Tile(void)
{
	color = std::vector<int>(3);
	makeEmpty();
}

Tile::~Tile(void)
{
}

Tile::Tile(int x, int y)
{
	color = std::vector<int>(3);
	status.lastLocation[0] = x;
	status.lastLocation[1] = y;
	makeEmpty();
}

void Tile::initialize()
{
	isEmpty = false;
	isCreature = false;
	isVegetation = false;
}

void Tile::loadGenetics()
{
	status.hunger = genetics.initialHunger;
	status.reproduction = genetics.reproduction;
	color = genetics.color;
}

void Tile::makeEmpty()
{
	initialize();
	isEmpty = true;
	color[0] = 0;
	color[1] = 0;
	color[2] = 0;
}

void Tile::makeVegetation()
{
	initialize();
	isVegetation = true;
	color[0] = 125;
	color[1] = 125;
	color[2] = 125;
}

void Tile::makeCreature(int x, int y, Genetics genetics)
{
	initialize();
	isCreature = true;

	this->genetics = genetics;
	status = Status();

	loadGenetics();
	status.lastLocation[0] = x;
	status.lastLocation[1] = y;
}

void Tile::makeRandomCreature(int x, int y)
{
	Genetics genetics;
	Status status;
	
	//determines food type
	int foodType = rand() % 2;

	if (foodType == 0)
	{
		genetics.isCarnivore = true;
		genetics.isHerbivore = false;
	}
	else
	{
		genetics.isCarnivore = false;
		genetics.isHerbivore = true;
	}

	//determines color based on food type and random variation
	if (genetics.isCarnivore)
	{
		genetics.color[0] = (rand()%105)+150;
		genetics.color[1] = 0;
		genetics.color[2] = 0;
	}
	else
	{
		genetics.color[0] = 0;
		genetics.color[1] = (rand()%105)+150;
		genetics.color[2] = 0;
	}

	genetics.initialHunger = 150;
	genetics.reproduction = 3;

	makeCreature(x, y, genetics);
}