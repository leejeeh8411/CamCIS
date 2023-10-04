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
	[CIS ����� ���� ����]
	CISī�޶�� ���� ������ ������ �پ Ư�������� 1�� ����� �ȴ�.
	�� 1���� ����� ���ν�ĵó�� 1�� ī�޶��� �ǹ̰� �ǰ�,
	CISī�޶�� ���̿� ���� �� ����� �������� �����ȴ�.
	ex)WHEC 760mm : ���3�� , 1500mm : ���5��
	*******************************
	*/
private:
	gLogger* _logger;

	vector<CamInfo> _camInfo;

	int _frameIdx = 0;

	int _cntCam = 0;
	int _cntModule = 0;
	int _cntModulePerBoard = 0;

	//module ���� �̹��� Set Cnt
	int _setImageCnt[LOOP_NUM];

	//image buffer
	vector<unsigned char*> _imageBufAddress;
	unsigned char* _imgBuf;

public:
	//Ŭ���� �ʱ�ȭ
	//camCnt : ī�޶� �� ����
	//moduleCnt : ��� ī��Ʈ(ī�޶� ��)
	//moduleCntPerBoard : ��� ī��Ʈ(���� ��)
	void InitClass(int camCnt, int moduleCnt, int moduleCntPerBoard);

	//�ʱ�ȭ ������ CAM_CIS Ŭ������ ���� �ʱ�ȭ�� �ʿ��� ������ �־��ش�
	void SetPathDCF(int camIdx, int boardIdx, string pathDcf);

	//�׷��� �ʱ�ȭ �۾��� �Ѵ�
	//�־��� ������ ��������
	void InitGrabber();

	//�̹��� ���� FullGrabBuffer, dcf size y �������� ��frame�� ���۷� �������� ����
	void MakeImageBuffer(int width, int height);

	//�����Ҵ�� �̹��� ��ü �Ұ�
	void ClearImageBuffer();

	//�ݹ� ȣ��Ǹ� FullGrabBuffer�� ī��
	//�� module���� �� �̹����� �����Ѵ�
	void SetImage(unsigned char* buffer, int moduleIdx, int frameIdx);

	//�̹��� 1���� �ϼ��Ǵ°� ������� �ľ�����
	//return : SetImage Cnt
	int GetSetImageCnt(int frameIdx);
};

