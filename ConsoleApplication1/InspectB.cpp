#include "stdafx.h"
#include "InspectB.h"


InspectB::InspectB()
{
}

InspectB::~InspectB()
{
}

void InspectB::InspectWork(int frame, int cellid, unsigned char* pImg, int width, int height)
{
	if (width != _width) {
		_logger->info("inspet work Exception_width, frame:{}, cellid:{}, pImg:{}",
			frame, cellid, (void*)pImg);
	}
	if (height != _height) {
		_logger->info("inspet work Exception_height, frame:{}, cellid:{}, pImg:{}",
			frame, cellid, (void*)pImg);
	}

	//이미지 복사
	memcpy(_pImg, pImg, sizeof(unsigned char) * width * height);

	int ave = GetAve(0, 0, width, height, 2, 2);

	ave -= 10;

	_logger->info("inspet work frame:{}, cellid:{}, pImg:{}, ave:{}",
		frame, cellid, (void*)pImg, ave);
}

