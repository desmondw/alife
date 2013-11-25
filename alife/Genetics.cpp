#include "Genetics.h"

Genetics::Genetics(void)
{
	color = std::vector<int>(3);
}

Genetics::~Genetics(void)
{
}

bool Genetics::testCloseGenetics(Genetics& comparison) //TODO: change each genetic comparison to be flexible (i.e. similar if slightly different colored shades, but dissimilar if different colors)
{
	int matchingGenetics = 0;

	if (color == comparison.color)
		matchingGenetics++;
	if (isCarnivore == comparison.isCarnivore)
		matchingGenetics++;
	if (isHerbivore == comparison.isHerbivore)
		matchingGenetics++;
	if (reproduction == comparison.reproduction)
		matchingGenetics++;
	if (initialHunger == comparison.initialHunger)
		matchingGenetics++;

	if (static_cast<float>(matchingGenetics)/NUM_OF_GENETICS >= .8) //share 80% of genetic makeup
		return true;
	return false;
}