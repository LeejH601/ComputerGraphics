#pragma once
#include "stdafx.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include "Object.h"
#include "DynamicObject.h"


#define MAX_LIGHTS 8

class CScene
{
	struct UBO_LIGHT
	{
		CLight lights[MAX_LIGHTS];
		UINT nLights;
	};
protected:
	CTexture m_tCubeMapTexture;
	std::shared_ptr<CTexture> m_tIrradianceTexture;
	std::shared_ptr<CTexture> m_tFilteringedEnvironmentTexture;
	std::shared_ptr<CTexture> m_tPreCoumputedBRDFLUTexture;
	std::shared_ptr<CTexture> m_tShadowDepthTexture;

	bool m_bInitialized = false;

	GLenum m_ePolygonFace = GL_FRONT_AND_BACK;
	GLenum m_ePolygonMode = GL_FILL;

	std::vector<std::shared_ptr<CCamera>> m_sptrCameras;
	CCamera* m_pMainCamera = nullptr;
	std::vector<CLight> m_pLights;
	CLight* m_pSunLight = nullptr;
	GLuint m_UBOLights = -1;
	GLuint m_UBOLightIndex = -1;
	UBO_LIGHT UBOLightData;

	UINT m_nShadowMapWidth, m_nShadowMapHeight;
	GLuint m_FBOShadowDepth;

	MOUSE_STATE m_eMouseState = MOUSE_STATE::MOUSE_CILCK_NONE;
	POINT m_ptOldMouseCursor{ 0,0 };
	POINT m_ptCurrMouseCuror;

	UINT DwDirection = 0;
	float cxDelta, cyDelta;

	bool m_bRegenarateView = true;

	std::vector<std::shared_ptr<CObject>> m_pObjects;

public:
	CScene();
	virtual ~CScene();

	virtual void Init();
	virtual void BuildObjects() {};
	virtual void RenderScene();
	virtual void MouseInput(int button, int state, int x, int y);
	virtual void MouseMotion(int x, int y);
	virtual void KeyInput(unsigned char key, int x, int y);
	virtual void KeyUpInput(unsigned char key, int x, int y);
	virtual void SpecialKeyInput(int key, int x, int y);
	virtual void BindShaderVariables(GLuint s_Program);
	virtual void Update(float fElapsedTime);
	virtual void SetPolygonMode(GLenum face = GL_FRONT_AND_BACK, GLenum mode = GL_FILL);

	virtual void Enter();
	virtual void Exit();
};

class CPBR_TestScene : public CScene
{
protected:
	std::shared_ptr<CObject> m_pSkyBoxObject;

public:
	CPBR_TestScene();
	virtual ~CPBR_TestScene();

	virtual void Init();
	virtual void BuildObjects();
	virtual void RenderScene();
	virtual void MouseInput(int button, int state, int x, int y);
	virtual void MouseMotion(int x, int y);
	virtual void KeyInput(unsigned char key, int x, int y);
	virtual void KeyUpInput(unsigned char key, int x, int y);
	virtual void SpecialKeyInput(int key, int x, int y);
	virtual void Update(float fElapsedTime);
};

