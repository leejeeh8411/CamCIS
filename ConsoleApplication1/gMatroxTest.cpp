#include "stdafx.h"
#include "gMatroxTest.h"

void ThreadCallBack(void* _parent)
{
	gMatroxTest* parent = (gMatroxTest*)_parent;

	while (true) {
		parent->MakeVirtualImage();
		parent->OnCallBack();
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
	
}

gMatroxTest::gMatroxTest()
{

}


gMatroxTest::~gMatroxTest()
{

}

void gMatroxTest::StartThreadCallBack()
{
	std::thread t1(ThreadCallBack, this);
	t1.detach();
}

void gMatroxTest::SetWidth(int width)
{
	_width = width;
}

void gMatroxTest::SetHeight(int height)
{
	_height = height;
}

//이미지 버퍼 생성
void gMatroxTest::MakeImageBuf()
{
	if (_pImg != nullptr) {
		delete[] _pImg;
	}
	
	_pImg = new unsigned char[_width * _height];
	memset(_pImg, 0, sizeof(unsigned char) * _width * _height);
}

//가상 이미지 생성
void gMatroxTest::MakeVirtualImage()
{
	if (_pImg == nullptr) {
		return;
	}

	int val = 100 + _frame;
	int size = sizeof(unsigned char) * _width * _height;

	for (int i = 0; i < size; i++) {
		_pImg[i] = val;
	}

	_frame = _frame++ % LOOP_NUM_;
}

