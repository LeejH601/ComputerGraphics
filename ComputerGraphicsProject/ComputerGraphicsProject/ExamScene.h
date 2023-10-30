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

class CExamScene_20 : public CPBR_TestScene
{
protected:
	int MoveBase = 0;

	int MoveTop = 0;

	int RotateBarrel = 0;

	int MoveBarrel = 0;

	int RotateArm = 0;

	int cameraMoveX = 0;
	int cameraMoveY = 0;
	int cameraMoveZ = 0;

	int cameraRevolution = 0;
	bool cameraLookAt = true;

	glm::vec3 cameraSpring;

	glm::vec3 posBarrel;

public:
	CExamScene_20();
	virtual ~CExamScene_20();

	virtual void Init();
	virtual void RenderScene();
	virtual void BuildObjects();
	virtual void KeyInput(unsigned char key, int x, int y);
	virtual void Update(float fElapsedTime);

	virtual void UpdateCameraSpring();
};

class CExamScene_21 : public CExamScene_20
{
	int currentCameraIndex = 0;
	glm::vec3 otherCameraSpring[3];
public:
	CExamScene_21();
	virtual ~CExamScene_21();

	virtual void Init();
	virtual void KeyInput(unsigned char key, int x, int y);
	virtual void UpdateCameraSpring();
};


class CExamScene_22 : public CPBR_TestScene
{
	float m_fFloor = 0.0f;
	float m_fCurrentTime = 0.0f;
	float m_fRobotSpeed = 1.0f;

	float m_fMaxRobotSpeed = 2.0f;
	float m_fMinRobotSpeed = 0.1f;

	float m_fBaseMoveDegree = 30.0f;

	bool b_OpenDoor = false;
	UINT m_DwRobotDir;
	glm::vec3 m_vec3RobotDir;
	float m_fJumpVelocity = 0.0f;

	CObject* R_DoorFrame = nullptr;
	CObject* L_DoorFrame = nullptr;

	glm::vec3 BaseDoorPos[2];

	CObject* R_Arm_Joint = nullptr;
	CObject* L_Arm_Joint = nullptr;

	CObject* R_Leg_Joint = nullptr;
	CObject* L_Leg_Joint = nullptr;

	CObject* m_Robot = nullptr;

	std::vector<CObject*> m_obstacles;

	glm::vec3 cameraSpring;
	glm::vec3 BaseCameraSpring;

	int cameraMoveX = 0;
	int cameraMoveY = 0;
	int cameraMoveZ = 0;

public:
	CExamScene_22();
	virtual ~CExamScene_22();

	virtual void Init();
	virtual void RenderScene();
	virtual void KeyInput(unsigned char key, int x, int y);
	virtual void KeyUpInput(unsigned char key, int x, int y);
	virtual void Update(float fElapsedTime);
	virtual void BuildObjects();
	bool CheckCollision();
};

class CSPScene : public CPBR_TestScene
{


public:
	CSPScene();
	virtual ~CSPScene();

	virtual void Update(float fElapsedTime);
};