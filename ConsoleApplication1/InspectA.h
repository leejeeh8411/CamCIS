#pragma once

#include "InterfaceInspect.h"


class InspectA : public InterfaceInspect
{
public:
	InspectA();
	~InspectA();

	void InspectWork(int frame, int cellid, unsigned char* pImg, int width, int height);
	
};

