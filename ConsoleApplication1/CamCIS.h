#pragma once

#include <vector>
#include <string>
#include <glogger.h>
#include "gMatroxTest.h"
//#include <gCamMatrox.h>

using namespace std;

#define DCF_FILE_PATH	"C:\\GLIM\\"

const static int LOOP_NUM = 40;


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

struct BoardList
{
	int BoardNo;
	int PortNo;
	int ImgWidth;
	int ImgHeight;
	int CamNo;
	bool InitGrabber;

	BoardList()
	{
		BoardNo = 0;
		PortNo = 0;
		ImgWidth = 0;
		ImgHeight = 0;
		CamNo = 0;
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

	vector<CamList> _camList;
	vector<BoardList> _bdList;

	int _frameIdx = 0;

	//module ���� �̹��� Set Cnt
	bool _setImageCnt[LOOP_NUM];

	//image buffer
	vector<unsigned char*> _imageBufAddress;
	unsigned char* _grab_buf;

public:
	void CopyMil(unsigned char * pImg, void* userData);

	vector<CallBackUserData*> _callBackUserData;

	vector<gMatroxTest*> _gMatrox;

	//Ŭ���� �ʱ�ȭ
	//camCnt : ī�޶� �� ����
	//moduleCnt : ��� ī��Ʈ(ī�޶� ��)
	void InitClass(int CamNum, int ModuleNum);

	//ī�޶� ����
	int GetCamNum();

	//ī�޶� ��
	int GetCamWidth(int camNo);

	//ī�޶� �̹��� ����
	int GetCamHeight(int _camNo);

	//�׷��� �ʱ�ȭ �۾��� �Ѵ�
	//�־��� ������ ��������
	void InitGrabber();

	//�̹��� ���� FullGrabBuffer, dcf size y �������� ��frame�� ���۷� �������� ����
	void MakeImageBuffer(int width, int height);

	//�����Ҵ�� �̹��� ��ü �Ұ�
	void ClearImageBuffer();

	//ī�޶� �������� ���۵� ������
	unsigned char* GetImagePtr(int camNo);

	//�ݹ� ȣ��Ǹ� FullGrabBuffer�� ī��
	//�� module���� �� �̹����� �����Ѵ�
	void SetImage(unsigned char* buffer, int moduleIdx, int frameIdx);

	//�̹��� 1���� �ϼ��Ǵ°� ������� �ľ�����
	//return : SetImage Cnt
	int GetSetImageCnt(int frameIdx);
};

