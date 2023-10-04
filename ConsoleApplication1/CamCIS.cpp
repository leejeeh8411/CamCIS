#include "stdafx.h"
#include "CamCIS.h"


CamCIS::CamCIS()
{
	_logger = new gLogger("defaultlog", std::string("C:\\Glim\\") + std::string("defaultLog.txt"), false, 23, 59);
	_logger->setConsoleLevel(G_LOGGER_LEVEL_DEBUG);
}


CamCIS::~CamCIS()
{
	ClearImageBuffer();
}

void CamCIS::InitClass(int CamNum, int ModuleNum)
{
	for (int camIdx = 0; camIdx < CamNum; camIdx++) {
		CamList camList;
		for (int nModuleIdx = 0; nModuleIdx < ModuleNum; nModuleIdx++) {
			string dcfPath(fmt::format("{}{}{}{}", DCF_FILE_PATH, "Glim_", nModuleIdx, ".dcf"));
			camList.CamNo = camIdx;
			camList.ModuleNo = nModuleIdx;
			camList.PathDCF = dcfPath;

			_camList.emplace_back(camList);
		}
	}
	
	//Ȯ��
	for (int i = 0; i < _camList.size(); i++) {
		CamList camList = _camList[i];
		int a = 10;
	}
}


void CamCIS::InitGrabber()
{
	//���弳�� �׽�Ʈ - ���߿� matrox���� �޾ƿ���
	int testBdNum = 3;
	int testPortNum = 2;
	for (int bdIdx = 0; bdIdx < testBdNum; bdIdx++) {
		BoardList bdList;
		
		for (int portIdx = 0; portIdx < testPortNum; portIdx++) {
			bdList.BoardNo = bdIdx;
			bdList.PortNo = portIdx;

			_bdList.emplace_back(bdList);
		}
	}
	
	int camListNum = _camList.size();
	int boardListNum = _bdList.size();
		
	for (int boardListIdx = 0; boardListIdx < boardListNum; boardListIdx++) {
		//���� port�� ī�޶� ��� �ϳ��� ��Ī
		if (boardListIdx >= camListNum) {
			break;
		}
		
		//gCamMatrox(boardIdx, i, _camList[boardListIdx].PathDCF, MATROX_BUF_NUM);

		_bdList[boardListIdx].CamNo = _camList[boardListIdx].CamNo;
		_bdList[boardListIdx].InitGrabber = true;

		//gCamMatrox->getWidth();
		//gCamMatrox->getHeight();
		//test
		_bdList[boardListIdx].ImgWidth = 7000;
		_bdList[boardListIdx].ImgHeight = 1536;

		//callback ����

		_logger->info("BoardListIdx:{}, BoardNo:{}, BoardPort:{}, CamNo:{}, ModuleNo:{}, Dcf:{}", 
			boardListIdx, _bdList[boardListIdx].BoardNo, _bdList[boardListIdx].PortNo,
			_camList[boardListIdx].CamNo, _camList[boardListIdx].ModuleNo, _camList[boardListIdx].PathDCF);
	}

	//ī�޶� ���� ��������
	int camNum = GetCamNum();

	//ī�޶�� �̹��� ũ�⸸ŭ ���� ����
	for (int camIdx = 0; camIdx < camNum; camIdx++) {
		int width_full = GetCamWidth(camIdx);
		int height_full = GetCamHeight(camIdx);
		MakeImageBuffer(width_full, height_full);
	}
}

//ī�޶� ���� ����
int CamCIS::GetCamNum()
{
	int camListNum = _camList.size();

	int camCnt = 0;

	vector<int> vtCamNo;

	for (int i = 0; i < camListNum; i++) {
		int camNo = _camList[i].CamNo;
		vtCamNo.emplace_back(camNo);
	}

	vtCamNo.erase(unique(vtCamNo.begin(), vtCamNo.end()), vtCamNo.end());

	return vtCamNo.size();
}

//ī�޶� �̹��� ��
int CamCIS::GetCamWidth(int _camNo)
{
	int bdListNum = _bdList.size();

	vector<int> vtCamNo;

	int width = 0;

	for (int i = 0; i < bdListNum; i++) {
		int camNo = _bdList[i].CamNo;

		if (camNo == _camNo) {
			width += _bdList[i].ImgWidth;
		}
	}

	return width;
}

//ī�޶� �̹��� ����
int CamCIS::GetCamHeight(int _camNo)
{
	int bdListNum = _bdList.size();

	vector<int> vtCamNo;

	int height = 0;

	for (int i = 0; i < bdListNum; i++) {
		int camNo = _bdList[i].CamNo;

		if (camNo == _camNo) {
			height += _bdList[i].ImgHeight;
			break;
		}
	}

	return height;
}

unsigned char* CamCIS::GetImagePtr(int camNo)
{
	int imageBufSize = _imageBufAddress.size();
	if (imageBufSize <= camNo) {
		return nullptr;
	}
	else {
		return _imageBufAddress[camNo];
	}
}

void CamCIS::MakeImageBuffer(int width, int height)
{
	int imageSize = width * height * LOOP_NUM;

	if (width <= 0) {
		_logger->info("MakeImageBuf Error : width:{}, height:{}", width, height);
		return;
	}
	if (height <= 0) {
		_logger->info("MakeImageBuf Error : width:{}, height:{}", width, height);
		return;
	}

	//�̹��� ���� ����
	unsigned char* pBuf = new unsigned char[imageSize];
	memset(pBuf, 0, sizeof(unsigned char) * imageSize);

	_imageBufAddress.emplace_back(pBuf);
	//_grab_buf = pBuf;

	_logger->info("MakeImageBuf:{}, width:{}, height:{}, loopNum:{}", (void*)pBuf, width, height, LOOP_NUM);

}

void CamCIS::ClearImageBuffer()
{
	for (int i = 0; i < _imageBufAddress.size(); i++) {
		delete[] _imageBufAddress[i];
		_logger->info("DeleteImageBuf:{}", (void*)_imageBufAddress[i]);
	}

	//delete[] _grab_buf;

	//_logger->info("DeleteImageBuf:{}", (void*)_grab_buf);
}