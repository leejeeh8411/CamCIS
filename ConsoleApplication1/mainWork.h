#pragma once
#include <glogger.h>
#include <gThreadpool.h>
#include <functional>
#include "CamCIS.h"
#include "InterfaceInspect.h"
#include "InspectA.h"
#include "InspectB.h"

class mainWork
{
public:
	mainWork();
	~mainWork();

	CamCIS camCis;

	void CisCallBack(int a);

	gLogger* _logger;

	gThreadPool* _gPoolIns;

	vector<InterfaceInspect*> _vtInspect;


	void AddIns(int camid, int frame, int cellid, unsigned char* pImg, int width, int height);
	
};

