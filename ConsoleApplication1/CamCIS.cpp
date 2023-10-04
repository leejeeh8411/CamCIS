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

void CamCIS::InitClass(int camCnt, int moduleCnt, int moduleCntPerBoard)
{
	//초기 설정값
	_cntCam = camCnt;
	_cntModule = moduleCnt;
	_cntModulePerBoard = moduleCntPerBoard;

	int boardIdx = 0;

	for (int camIdx = 0; camIdx < _cntCam; camIdx++) {
		int nModuleCnt = 0;
		for (int nModuleIdx = 0; nModuleIdx < _cntModule; nModuleIdx++) {
			if (nModuleCnt >= _cntModulePerBoard) {
				nModuleCnt = 0;
				boardIdx++;
			}

			string dcfPath(fmt::format("{}{}{}{}", DCF_FILE_PATH, "Glim_", nModuleIdx, ".dcf"));
			SetPathDCF(camIdx, boardIdx, dcfPath);

			nModuleCnt++;
		}
	}

	//확인
	for (int i = 0; i < _camInfo.size(); i++) {
		CamInfo bdInfo = _camInfo[i];
		int a = 10;
	}
}

void CamCIS::SetPathDCF(int camIdx, int boardIdx, string pathDcf)
{
	int camInfoSize = _camInfo.size();
	bool bIs = false;
	for (int i = 0; i < camInfoSize; i++) {
		if (_camInfo[i].camIdx == camIdx) {
			bIs = true;
			break;
		}
	}

	if (bIs == true) {
		int boardSize = _camInfo[camIdx].boardInfo.size();
		bool bIs = false;
		for (int i = 0; i < boardSize; i++) {
			if (_camInfo[camIdx].boardInfo[i].boardIdx == boardIdx) {
				bIs = true;
				break;
			}
		}
		if (bIs == true) {
			_camInfo[camIdx].boardInfo[boardIdx].pathDcf.emplace_back(pathDcf);
		}
		else {
			BoardInfo bdInfo;
			bdInfo.boardIdx = boardIdx;
			bdInfo.pathDcf.emplace_back(pathDcf);
			_camInfo[camIdx].boardInfo.emplace_back(bdInfo);
		}
	}
	else {
		CamInfo camInfo;
		camInfo.camIdx = camIdx;

		BoardInfo bdInfo;
		bdInfo.boardIdx = boardIdx;
		bdInfo.pathDcf.emplace_back(pathDcf);

		camInfo.boardInfo.emplace_back(bdInfo);
	
		_camInfo.emplace_back(camInfo);
	}
}

void CamCIS::InitGrabber()
{
	int camInfoSize = _camInfo.size();

	for (int camIdx = 0; camIdx < camInfoSize; camIdx++) {
		int width_full = 0;
		int height_full = 0;
		int boardSize = _camInfo[camIdx].boardInfo.size();
		
		for (int boardIdx = 0; boardIdx < boardSize; boardIdx++) {
			int moduleSize = _camInfo[camIdx].boardInfo[boardIdx].pathDcf.size();

			for (int moduleIdx = 0; moduleIdx < moduleSize; moduleIdx++) {
				string pathDcf = _camInfo[camIdx].boardInfo[boardIdx].pathDcf[moduleIdx];

				_logger->info("camIdx:{}, boardIdx:{}, dcf:{}", camIdx, boardIdx, pathDcf);
				
				//메트록스 초기화
				//gCamMatrox(boardIdx, i, dcfPath, MATROX_BUF_NUM);

				//image width 결합
				int width_module = 7000;
				width_full += width_module;
				//image height
				int height_module = 3072;
				height_full = height_module;
				//callback 함수 연결


			}
		}
		_camInfo[camIdx].width = width_full;
		_camInfo[camIdx].height = height_full;
	}
	
	for (int camIdx = 0; camIdx < camInfoSize; camIdx++) {
		int width_full = _camInfo[camIdx].width;
		int height_full = _camInfo[camIdx].height;

		//이미지를 보관할 버퍼 생성
		MakeImageBuffer(width_full, height_full);
		
	}

}

void CamCIS::MakeImageBuffer(int width, int height)
{
	int imageSize = width * height;

	//이미지 버퍼 생성
	unsigned char* pBuf = _imgBuf = new unsigned char[imageSize];
	memset(_imgBuf, 0, sizeof(unsigned char) * imageSize);

	_imageBufAddress.emplace_back(pBuf);

	_logger->info("MakeImageBuf:{}, width:{}, height:{}", (void*)pBuf, width, height);

}

void CamCIS::ClearImageBuffer()
{
	for (int i = 0; i < _imageBufAddress.size(); i++) {
		delete[] _imageBufAddress[i];
		_logger->info("DeleteImageBuf:{}", (void*)_imageBufAddress[i]);
	}
}