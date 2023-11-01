#pragma once
#include "Scene.h"
class CExamScene : public CScene
{
protected:
	std::shared_ptr<CObject> m_pSkyBoxObject;
	glm::vec3 m_AimbientMoveDir{ 0,0,0 };
public:
	CExamScene() = default;
	virtual void Init();
};


class CExamScene_7 : public CExamScene
{
public:
	CExamScene_7();
	virtual ~CExamScene_7();

	virtual void Init();
	virtual void RenderScene();
	virtual void KeyInput(unsigned char key, int x, int y);
	virtual void Update(float fElapsedTime);
};

class CExamScene_8 : public CExamScene_7
{
	GLenum m_eFillmode = GL_FILL;
public:
	CExamScene_8();
	virtual ~CExamScene_8();

	virtual void Init();
	virtual void RenderScene();
	virtual void KeyInput(unsigned char key, int x, int y);
	virtual void MouseInput(int button, int state, int x, int y);
	virtual void Update(float fElapsedTime);
};

class CExamScene_9 : public CExamScene_7
{
	float width = 1.67 * 5.0f, height = 1.0f * 5.0f;
	int m_moveType = 0;
	std::vector<glm::vec3> m_moveDir;
	std::vector<float> m_collisionDelay;
public:
	CExamScene_9();
	virtual ~CExamScene_9();

	virtual void Init();
	virtual void RenderScene();
	virtual void KeyInput(unsigned char key, int x, int y);
	virtual void MouseInput(int button, int state, int x, int y);
	virtual void Update(float fElapsedTime);
};


class CExamScene_18 : public CPBR_TestScene
{
public:
	CExamScene_18();
	virtual ~CExamScene_18();

	virtual void Init();
	//virtual void RenderScene();
	virtual void BuildObjects();
	virtual void Update(float fElapsedTime);
};

class CSPScene : public CPBR_TestScene
{
	POINT m_startPos;
	POINT m_endPos;
	glm::vec3 p1, p2;
public:
	CSPScene();
	virtual ~CSPScene();

	virtual void MouseInput(int button, int state, int x, int y);
	virtual void Update(float fElapsedTime);
};