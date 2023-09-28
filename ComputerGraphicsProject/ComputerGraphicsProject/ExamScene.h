#pragma once
#include "Scene.h"
class CExamScene : public CScene
{
protected:
	std::shared_ptr<CObject> m_pSkyBoxObject;
	glm::vec3 m_AimbientMoveDir;
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

	/*virtual void MouseInput(int button, int state, int x, int y);
	virtual void MouseMotion(int x, int y);
	
	virtual void KeyUpInput(unsigned char key, int x, int y);
	virtual void SpecialKeyInput(int key, int x, int y);

	virtual void Enter();
	virtual void Exit();*/
};
