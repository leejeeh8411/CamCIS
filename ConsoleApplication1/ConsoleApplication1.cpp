// ConsoleApplication1.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "CamCIS.h"

int main()
{
	int camNum = 4;
	int moduleNum = 1;
	int moduleNumPerBoard = 1;
	CamCIS camCis;
	camCis.InitClass(camNum, moduleNum, moduleNumPerBoard);
	camCis.InitGrabber();

	system("pause");
    return 0;
}

