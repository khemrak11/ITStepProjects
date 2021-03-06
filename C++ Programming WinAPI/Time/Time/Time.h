#pragma once

#include "resource.h"

class Time
{
private:
	int hh, mm;
	bool isAM;
	bool isAmericanTime;
public:
	Time(int hh = 0, int mm = 0, bool isAmericanTime = false, bool isAM = true);

	void SetHour(int h);
	void SetMin(int m);
	void SetType(bool isAmericanTime = true);
	void SetAMOrPM(bool isAM);

	int GetHour();
	int GetMin();
	bool IsAmericanTime();
	bool IsAM();

	Time operator + (const Time &other);
	Time operator - (const Time &other);
	int operator > (const Time &other);
};

Time Convert(Time t, bool fromAmericanTimeToGMT);