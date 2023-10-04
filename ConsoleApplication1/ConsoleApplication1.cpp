// ConsoleApplication1.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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

