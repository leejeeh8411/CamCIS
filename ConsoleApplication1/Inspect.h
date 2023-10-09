#pragma once
#include <glogger.h>


class Inspect
{
public:
	Inspect();
	~Inspect();

	gLogger* _logger;

	void InspectWork(int frame, int cellid, unsigned char* pImg);
};

