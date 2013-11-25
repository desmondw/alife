#ifndef _GENETICS_H
#define _GENETICS_H

#include <vector>

class Genetics
{
	private:
		static const int NUM_OF_GENETICS = 5;

	public:
		std::vector<int> color;
		bool isCarnivore;
		bool isHerbivore;
		int reproduction;
		int initialHunger;

		Genetics(void);
		~Genetics(void);
		bool testCloseGenetics(Genetics& comparison);
};

#endif