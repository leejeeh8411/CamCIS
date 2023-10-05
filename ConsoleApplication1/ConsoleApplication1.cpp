// ConsoleApplication1.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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

	//�ʱ�ȭ�� ���ι��� ��������
	unsigned char* pImg1 = camCis.GetImagePtr(0);
	unsigned char* pImg2 = camCis.GetImagePtr(1);
	unsigned char* pImg3 = camCis.GetImagePtr(2);
	unsigned char* pImg4 = camCis.GetImagePtr(3);

	//������ Ȯ��
	_logger->info("getImagePtr1:{}", (void*)pImg1);
	_logger->info("getImagePtr2:{}", (void*)pImg2);
	_logger->info("getImagePtr3:{}", (void*)pImg3);
	_logger->info("getImagePtr4:{}", (void*)pImg4);

	//callback �Լ� ��� üũ
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

