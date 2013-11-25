#ifndef _TILE_H
#define _TILE_H

#include <vector>
#include "Genetics.h"
#include "Status.h"

class Tile
{
	public:
		std::vector<int> color;

		bool isEmpty;
		bool isCreature;
		bool isVegetation;

		Genetics genetics;
		Status status;

		Tile(void);
		~Tile(void);
		Tile(int x, int y);
		void initialize();
		void loadGenetics();
		void makeEmpty();
		void makeVegetation();
		void makeCreature(int x, int y, Genetics genetics);
		void makeRandomCreature(int x, int y);
};

#endif