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
	std::array<std::array<GLfloat, 3>, 4> m_f3BigRectColors;
	std::array<std::array<GLfloat, 3>, 4> m_f3SmallRectColors;
	std::array<std::array<int, 4>, 4> m_i4Rects;

public:
	DECLARE_SINGLE(CRenderer);

	CRenderer() = default;
	virtual ~CRenderer() = default;

	bool init(int argc, char** argv);

	std::array<GLclampf, 4>& GetClearColor() { return m_glf4ClearColor; };
	void SetClearColor(float r, float g, float b, float a);
	int GetWidth() { return m_nFrameWidth; };
	int GetHeight() { return m_nFrameHeight; };

	void DrawRect(float x1, float y1, float x2, float y2, GLfloat r, GLfloat g, GLfloat b);
	void DrawRect(glm::vec4 rect, GLfloat r, GLfloat g, GLfloat b);
	std::array<GLfloat, 3>& GetBigRectColorFromIndex(int index) { return m_f3BigRectColors[index]; };
	std::array<GLfloat, 3>& GetSmallRectColorFromIndex(int index) { return m_f3SmallRectColors[index]; };
};

