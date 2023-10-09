#include "stdafx.h"
#include "Inspect.h"


Inspect::Inspect()
{
	_logger = new gLogger("defaultlog", std::string("C:\\Glim\\") + std::string("defaultLog.txt"), false, 23, 59);
	_logger->setConsoleLevel(G_LOGGER_LEVEL_DEBUG);
}


Inspect::~Inspect()
{
}

void Inspect::InspectWork(int frame, int cellid, unsigned char* pImg)
{
	_logger->info("inspet work frame:{}, cellid:{}, pImg:{}", 
		frame, cellid, (void*)pImg);
}

