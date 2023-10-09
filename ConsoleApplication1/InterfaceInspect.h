#pragma once

#include <glogger.h>

class InterfaceInspect
{
public:
	InterfaceInspect();
	~InterfaceInspect();

	gLogger* _logger;

	int _width = 0;
	int _height = 0;
	unsigned char* _pImg = nullptr;

	void Init(int width, int height);
	void MakeImageBuf();
	int  GetAve(int l, int t, int r, int b, int skipX, int skipY);

	virtual void InspectWork(int frame, int cellid, unsigned char* pImg, int width, int height) = 0;

private:
	

};

