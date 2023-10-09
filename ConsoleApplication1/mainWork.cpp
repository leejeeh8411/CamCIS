#include "stdafx.h"
#include "mainWork.h"


mainWork::mainWork()
{
	_logger = new gLogger("defaultlog", std::string("C:\\Glim\\") + std::string("defaultLog.txt"), false, 23, 59);
	_logger->setConsoleLevel(G_LOGGER_LEVEL_DEBUG);

	int camNum = 1;
	int moduleNum = 5;

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
		pMatrox->StartThreadCallBack();
	}

	_gPoolIns = new gThreadPool(2);

	//callback ����
	camCis.RegistCallBack(std::bind(&mainWork::CisCallBack, this, std::placeholders::_1));
}


mainWork::~mainWork()
{
	//camCis.ClearImageBuffer();
}

void mainWork::CisCallBack(int a)
{
	_logger->info("cisCallBack");
	//ī�޶� ������ �����´�
	//ī�޶� ������, �̹��� ��ġ ����
	ImageList imgData = camCis.GetQueue();

	int cellid = 0;
	AddIns(imgData.frame, cellid, imgData.pAddress);
}

void mainWork::AddIns(int frame, int cellid, unsigned char* pImg)
{
	_logger->info("add job");
	_gPoolIns->addJob([this](int _frame, int _cellid, unsigned char* _pImg) {
		this->_inspect.InspectWork(_frame, _cellid, _pImg); }, frame, cellid, pImg);
}

