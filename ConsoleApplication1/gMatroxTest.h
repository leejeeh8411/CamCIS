#pragma once

#include <functional>

class gMatroxTest
{
public:
	gMatroxTest();
	~gMatroxTest();

	void* _userData;
	unsigned char* _pImg;

	//�̰� �׷� �׽�Ʈ Ŭ������ �Ű�
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
};

