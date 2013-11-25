#ifndef _SETTINGS_H
#define _SETTINGS_H

class Settings
{
	public:
		static const int WIN_X = 1028, WIN_Y = 768;
		bool limitFPSOn;
		bool soundOn;

		Settings(void);
		~Settings(void);
};

#endif