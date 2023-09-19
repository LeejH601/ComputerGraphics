#pragma once
#include "stdafx.h"

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

class CRenderer
{
private:
	std::array<GLclampf, 4> m_glf4ClearColor;
	int m_nFrameWidth;
	int m_nFrameHeight;

public:
	DECLARE_SINGLE(CRenderer);

	CRenderer() = default;
	virtual ~CRenderer() = default;

	bool init(int argc, char** argv);

	std::array<GLclampf, 4>& GetClearColor() { return m_glf4ClearColor; };
	void SetClearColor(float r, float g, float b, float a);
};

