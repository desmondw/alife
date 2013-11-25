#ifndef _STATUS_H
#define _STATUS_H

#include <vector>

class Status
{
	public:
		std::vector<int> lastLocation;
		int hunger;
		int reproduction;

		Status(void);
		~Status(void);
		void setLastLocation(int x, int y);
};

#endif