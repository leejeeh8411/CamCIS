// ConsoleApplication1.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "CamCIS.h"
#include <glogger.h>

int main()
{
	gLogger* _logger;
	_logger = new gLogger("defaultlog", std::string("C:\\Glim\\") + std::string("defaultLog.txt"), false, 23, 59);
	_logger->setConsoleLevel(G_LOGGER_LEVEL_DEBUG);

	int camNum = 1;
	int moduleNum = 5;

	CamCIS camCis;
	camCis.InitClass(camNum, moduleNum);
	camCis.InitGrabber();

	//초기화된 메인버퍼 가져오기
	unsigned char* pImg1 = camCis.GetImagePtr(0);
	unsigned char* pImg2 = camCis.GetImagePtr(1);
	unsigned char* pImg3 = camCis.GetImagePtr(2);
	unsigned char* pImg4 = camCis.GetImagePtr(3);

	//포인터 확인
	_logger->info("getImagePtr1:{}", (void*)pImg1);
	_logger->info("getImagePtr2:{}", (void*)pImg2);
	_logger->info("getImagePtr3:{}", (void*)pImg3);
	_logger->info("getImagePtr4:{}", (void*)pImg4);

	//callback 함수 등록 체크
	int matNum = camCis._gMatrox.size();

	for (int i = 0; i < matNum; i++) {
		gMatroxTest* pMatrox = camCis._gMatrox[i];
		//pMatrox->OnCallBack();
		pMatrox->StartThreadCallBack();
	}
	

	
	//camCis.ClearImageBuffer();
	system("pause");
    return 0;
}

