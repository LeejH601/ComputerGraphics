#pragma once
#include "stdafx.h"

class CScene
{
public:
	virtual void Init();
	virtual void drawScene();
	virtual void Update(float fTimeElapsed);
	virtual void MouseInput(int button, int state, int x, int y);
	virtual void MouseMotion(int x, int y);
	virtual void KeyInput(unsigned char key, int x, int y);
	virtual void KeyUpInput(unsigned char key, int x, int y);
};

class CScene_4 : public CScene
{
	std::vector<glm::vec4> Colors;
	std::vector<glm::vec4> Rects;
	std::vector<glm::vec4> origin_Rects;
	std::vector<glm::vec2> Rect_Dirs;
	float mouse_X, mouse_Y;
	float Curr_x, Curr_Y;

	enum class MOVE_STATE {
		MOVE1,
		MOVE2,
		NONE,
	};

	MOVE_STATE g_mouseState = MOVE_STATE::NONE;

	int shapeState = 0;
	int colorState = 0;

public:
	virtual void drawScene();
	virtual void Update(float fTimeElapsed);
	virtual void MouseInput(int button, int state, int x, int y);
	virtual void MouseMotion(int x, int y);
	virtual void KeyInput(unsigned char key, int x, int y);
};


class CScene_5 : public CScene
{
	std::vector<glm::vec4> Rects;
	std::vector<bool> Rects_Collided;
	std::vector<glm::vec4> Colors;
	float mouse_X, mouse_Y;
	float Curr_x, Curr_Y;
	glm::vec2 earserPos;
	glm::vec2 earserSize;
	glm::vec4 earserColor;
	bool isEarse = false;

public:
	CScene_5();
	virtual void Init();
	virtual void drawScene();
	virtual void Update(float fTimeElapsed);
	virtual void MouseInput(int button, int state, int x, int y);
	virtual void MouseMotion(int x, int y);
	virtual void KeyInput(unsigned char key, int x, int y);
	bool intersect(glm::vec4 r1, glm::vec4 r2);
};
