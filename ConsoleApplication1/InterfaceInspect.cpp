#include "stdafx.h"
#include "InterfaceInspect.h"


InterfaceInspect::InterfaceInspect()
{
	_logger = new gLogger("defaultlog", std::string("C:\\Glim\\") + std::string("defaultLog.txt"), false, 23, 59);
	_logger->setConsoleLevel(G_LOGGER_LEVEL_DEBUG);
}


InterfaceInspect::~InterfaceInspect()
{
}

void InterfaceInspect::Init(int width, int height)
{
	_width = width;
	_height = height;
}

void InterfaceInspect::MakeImageBuf()
{
	if (_pImg != nullptr) {
		delete[] _pImg;
	}
	else {
		_pImg = new unsigned char[_width * _height];
	}
}

int InterfaceInspect::GetAve(int l, int t, int r, int b, int skipX, int skipY)
{
	int sum = 0;
	int cnt = 0;

	for (int y = t; y < b; y += skipY) {
		for (int x = l; x < r; x += skipX) {
			sum += _pImg[y * _width + x];
			cnt++;
		}
	}

	if (cnt < 1) {
		cnt = 1;
	}

	return sum /= cnt;
}

