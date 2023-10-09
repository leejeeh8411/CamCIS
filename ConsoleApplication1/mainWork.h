#pragma once
#include <glogger.h>
#include <gThreadpool.h>
#include <functional>
#include "CamCIS.h"
#include "Inspect.h"

class mainWork
{
public:
	mainWork();
	~mainWork();

	CamCIS camCis;

	void CisCallBack(int a);

	gLogger* _logger;

	gThreadPool* _gPoolIns;

	Inspect _inspect;

	void AddIns(int frame, int cellid, unsigned char* pImg);

	
	
};

