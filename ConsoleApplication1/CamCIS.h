#pragma once

#include <vector>
#include <string>
#include <glogger.h>
//#include <gCamMatrox.h>

using namespace std;

#define DCF_FILE_PATH	"C:\\GLIM\\"

const static int LOOP_NUM = 10;
const static int MATROX_BUF_NUM = 10;

//extern gLogger logger;

//gLogger logger("defaultlog", std::string("C:\\Glim\\") + std::string("defaultLog.txt"), false, 23, 59);


struct BoardInfo
{
	int boardIdx;
	vector<string> pathDcf;

	BoardInfo()
	{
		boardIdx = 0;
		pathDcf.clear();
	}
};

struct CamInfo 
{
	int camIdx;
	int width;
	int height;

	vector<BoardInfo> boardInfo;
	CamInfo()
	{
		camIdx = 0;
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

	vector<CamInfo> _camInfo;

	int _frameIdx = 0;

	int _cntCam = 0;
	int _cntModule = 0;
	int _cntModulePerBoard = 0;

	//module 별로 이미지 Set Cnt
	int _setImageCnt[LOOP_NUM];

	//image buffer
	vector<unsigned char*> _imageBufAddress;
	unsigned char* _imgBuf;

public:
	//클래스 초기화
	//camCnt : 카메라 총 개수
	//moduleCnt : 모듈 카운트(카메라 당)
	//moduleCntPerBoard : 모듈 카운트(보드 당)
	void InitClass(int camCnt, int moduleCnt, int moduleCntPerBoard);

	//초기화 시점에 CAM_CIS 클래스를 통해 초기화에 필요한 정보를 넣어준다
	void SetPathDCF(int camIdx, int boardIdx, string pathDcf);

	//그래버 초기화 작업을 한다
	//넣어준 정보를 바탕으로
	void InitGrabber();

	//이미지 생성 FullGrabBuffer, dcf size y 기준으로 몇frame을 버퍼로 생성할지 결정
	void MakeImageBuffer(int width, int height);

	//동적할당된 이미지 전체 소거
	void ClearImageBuffer();

	//콜백 호출되면 FullGrabBuffer에 카피
	//각 module별로 원 이미지에 복사한다
	void SetImage(unsigned char* buffer, int moduleIdx, int frameIdx);

	//이미지 1장이 완성되는걸 어떤식으로 파악할지
	//return : SetImage Cnt
	int GetSetImageCnt(int frameIdx);
};

