#pragma once

#include <functional>
#include <thread>
#include <chrono>

const static int LOOP_NUM_ = 40;

class gMatroxTest
{
public:
	gMatroxTest();
	~gMatroxTest();

	void* _userData = nullptr;
	unsigned char* _pImg = nullptr;

	int _width = 0;
	int _height = 0;

	int _frame = 0;

	//이거 그랩 테스트 클래스로 옮겨
	std::function<void(unsigned char *, void *)> _callBack;
	void RegistCallBack(std::function<void(unsigned char *, void *)> pCallBack, void* userData)
	{
		_callBack = std::move(pCallBack);
		_userData = std::move(userData);
	}

	void OnCallBack()
	{
		if (_callBack != nullptr)
			_callBack(_pImg, _userData);
	}

	void StartThreadCallBack();

	void SetWidth(int width);

	void SetHeight(int height);

	void MakeImageBuf();

	void MakeVirtualImage();
};

