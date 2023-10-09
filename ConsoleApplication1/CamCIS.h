#pragma once


//#include <afxwin.h>
#include <atlimage.h>
#include <vector>
#include <string>
#include <glogger.h>
#include "gMatroxTest.h"
#include <functional>
#include <queue>
//#include <gCamMatrox.h>


using namespace std;

#define DCF_FILE_PATH	"C:\\GLIM\\"

const static int LOOP_NUM = 40;


enum CAM_DATA_TYPE {
	IMG_PTR_X = 0,
	IMG_WIDTH = 1
};

struct CallBackUserData
{
	int CamNo;
	int ModuleNo;
	CallBackUserData()
	{
		CamNo = 0;
		ModuleNo = 0;
	}
};

struct CallBackInfo
{
	vector<CallBackUserData> camInfo;

	CallBackInfo()
	{
	}
};

struct BoardList
{
	int BoardNo;
	int PortNo;
	int CamNo;
	int ModuleNo;
	int ImgWidth;
	int ImgHeight;
	int ImgPtrX;
	bool InitGrabber;

	BoardList()
	{
		BoardNo = 0;
		PortNo = 0;
		CamNo = 0;
		ModuleNo = 0;
		ImgWidth = 0;
		ImgHeight = 0;
		ImgPtrX = 0;
		InitGrabber = false;
	}
};

struct CamList 
{
	int CamNo;
	int ModuleNo;
	string PathDCF;
	
	CamList()
	{
		CamNo = 0;
		ModuleNo = 0;
	}
};

struct ImageList
{
	int frame;
	unsigned char* pAddress;
	ImageList()
	{
		frame = 0;
		pAddress = nullptr;
	}
};

class CamCIS
{
public:
	CamCIS();
	~CamCIS();

	/*
	*******************************
	[CIS 모듈의 대한 설명]
	CIS카메라는 작은 센서가 여러개 붙어서 특정단위의 1개 모듈이 된다.
	이 1개의 모듈이 라인스캔처럼 1개 카메라의 의미가 되고,
	CIS카메라는 길이에 따라 이 모듈이 여러개로 구성된다.
	ex)WHEC 760mm : 모듈3개 , 1500mm : 모듈5개
	*******************************
	*/
private:
	gLogger* _logger;

	vector<CamList> _camList;
	vector<BoardList> _bdList;

	queue<ImageList> qImageList;

	int* _frameIdx = nullptr;

	//module 별로 이미지 Set Cnt
	CallBackInfo _callBackInfo[LOOP_NUM];

	//image buffer
	vector<unsigned char*> _imageBufAddress;
	//unsigned char* _grab_buf;


	std::function<void(int)> _callBack;

	void OnCallBack()
	{
		if (_callBack != nullptr)
			_callBack(0);
	}

public:
	
	void RegistCallBack(std::function<void(int)> pCallBack)
	{
		_callBack = std::move(pCallBack);
	}

	void CopyMil(unsigned char * pSrc, void* userData);

	vector<CallBackUserData*> _callBackUserData;

	vector<gMatroxTest*> _gMatrox;

	mutex _mutex;

	ImageList CamCIS::GetQueue();

	//클래스 초기화
	//camCnt : 카메라 총 개수
	//moduleCnt : 모듈 카운트(카메라 당)
	void InitClass(int CamNum, int ModuleNum);

	//카메라 개수
	int GetCamNum();

	//카메라 폭
	int GetCamWidth(int camNo);

	//카메라 이미지 높이
	int GetCamHeight(int _camNo);

	int GetCamData(int _camNo, int _moduleNo, int type);

	//그래버 초기화 작업을 한다
	//넣어준 정보를 바탕으로
	void InitGrabber();

	//이미지 생성 FullGrabBuffer, dcf size y 기준으로 몇frame을 버퍼로 생성할지 결정
	void MakeImageBuffer(int width, int height);

	//동적할당된 이미지 전체 소거
	void ClearImageBuffer();

	//카메라가 여러개면 버퍼도 여러개
	unsigned char* GetImagePtr(int camNo);

	//콜백 호출되면 FullGrabBuffer에 카피
	//각 module별로 원 이미지에 복사한다
	void SetImage(unsigned char* pSrc, unsigned char* pDst, int widthSrc, int widthDst, int copyWidth, int copyHeight);

};

