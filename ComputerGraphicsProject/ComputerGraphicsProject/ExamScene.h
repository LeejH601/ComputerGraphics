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
