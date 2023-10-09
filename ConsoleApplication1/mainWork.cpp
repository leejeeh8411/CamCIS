#include "stdafx.h"
#include "mainWork.h"

UINT ThreadInspectTemplate(void* _parent, int camid, int frame, int cellid, unsigned char* pImg, int width, int height)
{
	mainWork* parent = (mainWork*)_parent;

	
	InterfaceInspect* inspectInstance = parent->_vtInspect[camid];
	inspectInstance->InspectWork(frame, cellid, pImg, width, height);
	
	return 0;
}

mainWork::mainWork()
{
	_logger = new gLogger("defaultlog", std::string("C:\\Glim\\") + std::string("defaultLog.txt"), false, 23, 59);
	_logger->setConsoleLevel(G_LOGGER_LEVEL_DEBUG);

	int camNum = 2;
	int moduleNum = 1;

	camCis.InitClass(camNum, moduleNum);
	camCis.InitGrabber();

	for (int camNum = 0; camNum < camCis.GetCamNum(); camNum++) {
		InterfaceInspect* inspectInstance;
		if (camNum == 0) {
			inspectInstance = (InterfaceInspect*)new InspectA();
		}
		else {
			inspectInstance = (InterfaceInspect*)new InspectB();
		}
		inspectInstance->Init(camCis.GetCamWidth(camNum), camCis.GetCamHeight(camNum));
		inspectInstance->MakeImageBuf();
		_vtInspect.emplace_back(inspectInstance);
	}
	

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
	camCis.ClearImageBuffer();
}

//ī�޶� Ŭ������ �̹����� �ϼ��Ǹ� ����� �̺�Ʈ
void mainWork::CisCallBack(int a)
{
	_logger->info("cisCallBack");
	//ī�޶� ������ �����´�
	//ī�޶� ������, �̹��� ��ġ ����
	ImageList imgData = camCis.GetQueue();

	int cellid = 0;
	AddIns(imgData.camId, imgData.frame, cellid, imgData.pAddress, imgData.width, imgData.height);
}

//������Ǯ�� �˻翡 �ʿ��� ������ �Լ� ����
void mainWork::AddIns(int camid, int frame, int cellid, unsigned char* pImg, int width, int height)
{
	_logger->info("add job camid:{}", camid);

	_gPoolIns->addJob([this](int _camid, int _frame, int _cellid, unsigned char* _pImg, int _width, int _height) {
		ThreadInspectTemplate(this, _camid, _frame, _cellid, _pImg, _width, _height); }, camid, frame, cellid, pImg, width, height);
}

