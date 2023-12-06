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





class CRouteDisplayer
{
	static const UINT MAX_INSTANCE_SIZE = 50;

	std::shared_ptr<CObject> m_pBaseRoutePointObject;

	std::vector<glm::mat4x4> m_mat4x4InstanceWorlds;

public:
	CRouteDisplayer();
	virtual ~CRouteDisplayer();

	void calculateInstanceWorldTransform(glm::vec3 linearVelocity, glm::vec3 linearAcceleration, glm::vec3 startPosition, float offsetDT = 0.0f);
	void calculateInstanceWorldTransform(CPhysicsComponent physics, glm::vec3 startPosition, float offsetDT = 0.0f);
	void calculateInstanceWorldTransform(CPhysicsComponent physics, CObject* obj, float offsetDT = 0.0f);
	virtual void Render(GLuint s_Program);
};

class CSPScene : public CPBR_TestScene
{
	POINT m_startPos;
	POINT m_endPos;
	glm::vec4 p1, p2, p3;
	std::vector<std::shared_ptr<CObject>> pointObj;
	std::vector<std::shared_ptr<CDynamicObject>> baseObject;
	std::shared_ptr<CObject> m_pbasket;
	glm::vec3 m_vec3BasketRoute[2];
	float m_fBasketDT = 0.0f;

	std::vector<std::shared_ptr<CObject>> m_pInbasketObjects;
	std::shared_ptr<CRouteDisplayer> m_pRouteDisplayer;

	float m_fCorrectionSpeed = 1.0f;

	static std::uniform_real_distribution<float> urd_velocityScale;
	static std::uniform_int_distribution<unsigned int> urd_material;
	static std::uniform_real_distribution<float> urd_rotate;

	float fRouteOffsetDT = 0.0f;

	bool m_bShowRoute = false;

public:
	CSPScene();
	virtual ~CSPScene();

	virtual void BuildObjects();
	virtual void MouseInput(int button, int state, int x, int y);
	virtual void Update(float fElapsedTime);
	virtual void KeyInput(unsigned char key, int x, int y);
	virtual void RenderScene();
};

class CExamScene_25 : public CPBR_TestScene
{
	CObject* mainObj = nullptr;
	std::vector<std::shared_ptr<CObject>> pObjectCache;

	bool yInput = false;
	bool rInput = false;
	bool RInput = false;
	bool zInput = false;
	bool ZInput = false;
public:
	CExamScene_25();
	virtual ~CExamScene_25();

	virtual void Init();
	virtual void RenderScene();
	virtual void KeyInput(unsigned char key, int x, int y);
	virtual void KeyUpInput(unsigned char key, int x, int y);
	virtual void Update(float fElapsedTime);
	virtual void BuildObjects();
};

class CExamScene_26 : public CExamScene_25
{
	std::vector<glm::vec3> m_vec3ColorList;
	bool rInput = false;
	bool RInput = false;
public:
	CExamScene_26();
	virtual ~CExamScene_26();

	virtual void Init();
	virtual void KeyInput(unsigned char key, int x, int y);
	virtual void KeyUpInput(unsigned char key, int x, int y);
	virtual void Update(float fElapsedTime);
	virtual void BuildObjects();
};

class CExamScene_27 : public CExamScene_20
{
	std::shared_ptr<CObject> m_pSunLightObject;
	glm::vec3 m_vec3LightColorCache;
	int rInput = 0;
public:
	CExamScene_27();
	virtual ~CExamScene_27();

	virtual void BuildObjects();
	virtual void KeyInput(unsigned char key, int x, int y);
	virtual void Update(float fElapsedTime);
};




class CAMScene : public CPBR_TestScene
{
	float m_fSide = 10.0f;
	float m_fCubeBaseSize = 1.0f;

	int m_nSideCubes = 10;
	float m_fEachCubeSide = 1.0f;
	float m_fEachCubeSizeScale = 1.0f;

	float m_fMinHeight = 0.0f;

	int m_nAnimationType = 0;

	bool yInput = false;
	bool YInput = false;

	std::uniform_real_distribution<float> urd{ 0,1 };
	std::uniform_real_distribution<float> urd_Scale{ 0,5 };
public:
	CAMScene();
	virtual ~CAMScene();

	void CreateCubes();
	virtual void BuildObjects();
	virtual void Update(float fElapsedTime);
	virtual void KeyInput(unsigned char key, int x, int y);
	virtual void KeyUpInput(unsigned char key, int x, int y);
};


class CExamScene_30 : public CPBR_TestScene
{

	bool yInput = false;
	bool YInput = false;

	bool xInput = false;
	bool XInput = false;

public:
	CExamScene_30();
	virtual ~CExamScene_30();

	virtual void BuildObjects();
	virtual void KeyInput(unsigned char key, int x, int y);
	virtual void KeyUpInput(unsigned char key, int x, int y);
	virtual void Update(float fElapsedTime);
};

class CExamScene_31 : public CPBR_TestScene
{
	std::uniform_real_distribution<float> urd_pos{ -5,5 };
	std::uniform_real_distribution<float> urd_scale{ 0,2 };
	std::uniform_real_distribution<float> urd_Accel{ 0.1,1 };

	bool yInput = false;
	bool sInput = true;
public:
	CExamScene_31();
	virtual ~CExamScene_31();

	virtual void BuildObjects();
	virtual void KeyInput(unsigned char key, int x, int y);
	virtual void KeyUpInput(unsigned char key, int x, int y);
	virtual void Update(float fElapsedTime);
};