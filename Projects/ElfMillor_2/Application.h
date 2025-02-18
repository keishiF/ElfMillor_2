#pragma once
#include "Vec3.h"

class Application
{
public:
	static Application& GetInstance();

	bool Init();

	void Run();

	void Terminate();

private:
	Application() {};
	Application(const Application&) = delete;
	void operator=(const Application&) = delete;

	int m_frame = 0;
	int imgHandle = 0;
};