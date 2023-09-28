#pragma once
#include "stdafx.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include "Object.h"

class CScene
{
protected:
	CTexture m_tCubeMapTexture;
	std::shared_ptr<CTexture> m_tIrradianceTexture;
	std::shared_ptr<CTexture> m_tFilteringedEnvironmentTexture;
	std::shared_ptr<CTexture> m_tPreCoumputedBRDFLUTexture;

	bool m_bInitialized = false;

	std::shared_ptr<CCamera> m_sptrMainCamera;
	CCamera* m_pMainCamera = nullptr;
	std::vector<CLight> m_pLights;
	CLight* m_pSunLight = nullptr;

	MOUSE_STATE m_eMouseState = MOUSE_STATE::MOUSE_CILCK_NONE;
	POINT m_ptOldMouseCursor{ 0,0 };
	POINT m_ptCurrMouseCuror;

	UINT DwDirection;
	float cxDelta, cyDelta;


	std::vector<std::shared_ptr<CObject>> m_pObjects;

public:
	CScene();
	virtual ~CScene();

	virtual void Init();
	virtual void RenderScene();
	virtual void MouseInput(int button, int state, int x, int y);
	virtual void MouseMotion(int x, int y);
	virtual void KeyInput(unsigned char key, int x, int y);
	virtual void KeyUpInput(unsigned char key, int x, int y);
	virtual void SpecialKeyInput(int key, int x, int y);
	virtual void Update(float fElapsedTime);

	virtual void Enter();
	virtual void Exit();
};

class CPBR_TestScene : public CScene
{
	std::shared_ptr<CObject> m_pSkyBoxObject;

public:
	CPBR_TestScene();
	virtual ~CPBR_TestScene();

	virtual void Init();
	virtual void RenderScene();
	virtual void MouseInput(int button, int state, int x, int y);
	virtual void MouseMotion(int x, int y);
	virtual void KeyInput(unsigned char key, int x, int y);
	virtual void KeyUpInput(unsigned char key, int x, int y);
	virtual void SpecialKeyInput(int key, int x, int y);
	virtual void Update(float fElapsedTime);
};

