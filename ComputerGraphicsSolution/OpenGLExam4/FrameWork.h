#pragma once
#include "stdafx.h"

GLvoid Keyboard(unsigned char key, int x, int y);
void TimerFunction(int value);

class CFrameWork
{
	float m_fColorTimer = 100;
	bool m_bAutoColorChange = false;

public:
	DECLARE_SINGLE(CFrameWork);

	CFrameWork() = default;
	~CFrameWork() = default;

	bool init(int argc, char** argv);
	void Run();

	float GetColorTimer() { return m_fColorTimer; };
	bool GetAutoColorChange() { return m_bAutoColorChange; };
	void SetAutoColorChange(bool flag) { m_bAutoColorChange = flag; };

	void DrawRects();
};

