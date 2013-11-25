#include "Status.h"

Status::Status(void)
{
	lastLocation.resize(2);
}

Status::~Status(void)
{
}

void Status::setLastLocation(int x, int y)
{
	lastLocation[0] = x;
	lastLocation[1] = y;
}