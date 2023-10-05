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
	//카메라, 카메라에 대한 모듈 수량, dcf 설정
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
}


void CamCIS::InitGrabber()
{
	//보드설정(테스트) - 나중에 matrox에서 받아오게
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
	int widthAdd = 0;
	int moduleAdd = 0;
	int CamNo = 0;
	for (int boardListIdx = 0; boardListIdx < boardListNum; boardListIdx++) {
		//보드 port에 카메라 모듈 하나씩 매칭
		if (boardListIdx >= camListNum) {
			break;
		}
		
		//gCamMatrox(boardIdx, portIdx, _camList[boardListIdx].PathDCF, MATROX_BUF_NUM);
		int width = 7000;//gCamMatrox->getWidth();
		int height = 512;//gCamMatrox->getHeight();

		//test
		_bdList[boardListIdx].ImgWidth = width;
		_bdList[boardListIdx].ImgHeight = height;

		gMatroxTest* gMatrox = new gMatroxTest();
		gMatrox->SetWidth(width);
		gMatrox->SetHeight(height);
		gMatrox->MakeImageBuf();
		_gMatrox.emplace_back(gMatrox);
		
		if (CamNo != _camList[boardListIdx].CamNo) {
			widthAdd = 0;
			moduleAdd = 0;
			CamNo = _camList[boardListIdx].CamNo;
		}

		_bdList[boardListIdx].ModuleNo = moduleAdd;
		_bdList[boardListIdx].ImgPtrX = widthAdd;
		_bdList[boardListIdx].CamNo = _camList[boardListIdx].CamNo;
		_bdList[boardListIdx].InitGrabber = true;

		widthAdd += width;
		moduleAdd++;

		CallBackUserData* userData = new CallBackUserData();
		userData->CamNo = _bdList[boardListIdx].CamNo;
		userData->ModuleNo = _bdList[boardListIdx].ModuleNo;
		_callBackUserData.emplace_back(userData);

		//callback 연결
		gMatrox->RegistCallBack(std::bind(&CamCIS::CopyMil, this, std::placeholders::_1, std::placeholders::_2), (void*)userData);

		_logger->info("BoardListIdx:{}, BoardNo:{}, BoardPort:{}, CamNo:{}, ModuleNo:{}, Dcf:{}", 
			boardListIdx, _bdList[boardListIdx].BoardNo, _bdList[boardListIdx].PortNo,
			_camList[boardListIdx].CamNo, _camList[boardListIdx].ModuleNo, _camList[boardListIdx].PathDCF);
	}

	//카메라 개수 가져오기
	int camNum = GetCamNum();
	_frameIdx = new int[camNum];

	//카메라당 이미지 크기만큼 버퍼 생성
	for (int camIdx = 0; camIdx < camNum; camIdx++) {
		int width_full = GetCamWidth(camIdx);
		int height_full = GetCamHeight(camIdx);
		MakeImageBuffer(width_full, height_full);
		_frameIdx[camIdx] = 0;
	}
}

void CamCIS::CopyMil(unsigned char * pSrc, void* _userData)
{
	CallBackUserData* pUser = (CallBackUserData*)_userData;

	CallBackUserData userData;
	memcpy(&userData, pUser, sizeof(CallBackUserData));

	int camNo = pUser->CamNo;
	int moduleNo = pUser->ModuleNo;

	_mutex.lock();

	int frame = _frameIdx[camNo] % LOOP_NUM;

	_mutex.unlock();

	_logger->info("CopyMil : Frame{}, CamNo:{}, ModuleNo:{}", frame, camNo, moduleNo);

	//위치에 맞게 버퍼 카피
	int WidthFull = GetCamWidth(camNo);
	int HeightFull = GetCamHeight(camNo);
	int WidthCopy = GetCamData(camNo, moduleNo, CAM_DATA_TYPE::IMG_WIDTH);
	int sttX = GetCamData(camNo, moduleNo, CAM_DATA_TYPE::IMG_PTR_X);
	int sttY = frame * HeightFull * WidthFull;
	unsigned char* pDst = GetImagePtr(camNo);

	SetImage(pSrc, pDst + sttY + sttX, WidthCopy, WidthFull, WidthCopy, HeightFull);

	_mutex.lock();

	//copy callback increase
	_callBackInfo[frame].camInfo.emplace_back(userData);

	//해당 카메라에 모듈 done 확인
	int size = _callBackInfo[frame].camInfo.size();
	int doneCnt = 0;
	vector<int> vtModule;
	for (int i = 0; i < size; i++) {
		if (camNo == _callBackInfo[frame].camInfo[i].CamNo) {
			//doneCnt++;
			vtModule.emplace_back(_callBackInfo[frame].camInfo[i].ModuleNo);
		}
	}
	//중복제거
	sort(vtModule.begin(), vtModule.end());
	vtModule.erase(unique(vtModule.begin(), vtModule.end()), vtModule.end());
	doneCnt = vtModule.size();

	//카피수량 확인
	int moduleCnt = 0;
	for (int i = 0; i < _camList.size(); i++) {
		if (camNo == _camList[i].CamNo) {
			moduleCnt++;
		}
	}

	//완료되었으면 inspect로 이관하고 백터 리셋
	bool bInspect = false;
	if (doneCnt == moduleCnt) {
		_logger->info("CopyMil_CallBack Done : frame:{}, CamNo:{}, DoneCnt:{}, ModuleCnt:{}", 
			frame, camNo, doneCnt, moduleCnt);
		_callBackInfo[frame].camInfo.clear();
		_frameIdx[camNo]++;
		bInspect = true;
	}

	_mutex.unlock();

	//이미지 저장해보자
	if (bInspect == true) {
		CImage image;
		image.Create(WidthFull, -HeightFull, 8);
		unsigned char* pImageDst = (unsigned char*)image.GetBits();

		static RGBQUAD rgb[256];
		int	i;

		for (i = 0; i < 256; i++) {
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		}
		image.SetColorTable(0, 256, rgb);

		memcpy(pImageDst, pDst + sttY, sizeof(unsigned char) * WidthFull * HeightFull);

		CString strPath;
		strPath.Format("D:\\ImgTest\\frame%d_cam%d.jpg", frame, camNo);
		image.Save(strPath);
	}
}

void CamCIS::SetImage(unsigned char* pSrc, unsigned char* pDst, int widthSrc, int widthDst, int copyWidth, int copyHeight)
{
	for (int y = 0; y < copyHeight; y++) {
		memcpy(pDst + y * widthDst, pSrc + y * widthSrc, sizeof(unsigned char) * copyWidth);
	}
}

//카메라 개수 리턴
int CamCIS::GetCamNum()
{
	int camListNum = _camList.size();

	int camCnt = 0;

	vector<int> vtCamNo;

	for (int i = 0; i < camListNum; i++) {
		int camNo = _camList[i].CamNo;
		vtCamNo.emplace_back(camNo);
	}

	//중복제거
	sort(vtCamNo.begin(), vtCamNo.end());
	vtCamNo.erase(unique(vtCamNo.begin(), vtCamNo.end()), vtCamNo.end());

	return vtCamNo.size();
}

//카메라 이미지 폭
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

int CamCIS::GetCamData(int _camNo, int _moduleNo, int type)
{
	int bdListNum = _bdList.size();

	vector<int> vtCamNo;

	int ret = 0;

	for (int i = 0; i < bdListNum; i++) {
		int camNo = _bdList[i].CamNo;
		int moduleNo = _bdList[i].ModuleNo;

		if (camNo == _camNo && moduleNo == _moduleNo) {

			if (type == IMG_PTR_X) {
				ret = _bdList[i].ImgPtrX;
				break;
			}
			else if (type == IMG_WIDTH) {
				ret = _bdList[i].ImgWidth;
				break;
			}
			
		}
	}

	return ret;
}

//카메라 이미지 높이
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

	//이미지 버퍼 생성
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