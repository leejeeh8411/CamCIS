#pragma once

#include "InterfaceInspect.h"

class InspectB : public InterfaceInspect
{
public:
	InspectB();
	~InspectB();

	void InspectWork(int frame, int cellid, unsigned char* pImg, int width, int height);
};

