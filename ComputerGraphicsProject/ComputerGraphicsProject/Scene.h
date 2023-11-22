#pragma once
#include "stdafx.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include "Object.h"
#include "DynamicObject.h"
#include "PhysicallyBasedBloomEffecter.h"


#define MAX_LIGHTS 8

struct RENDERTARGET_INFO
{
	GLint InternalFormat;
	GLenum Format;
	GLenum type;
	GLuint Attachment;
};

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

	bool m_bEnableMultiRenderTarget = false;
	GLuint m_FBOMultiRenderTarget = -1;
	std::vector<RENDERTARGET_INFO> m_RBOInfo;
	std::vector<GLuint> m_RBOs;
	std::shared_ptr<CMesh> m_NdcMesh;

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

	bool m_bEnablePhysicallyBasedBloom = false;
	float m_fbloomFilterRadius = 0.005f;
	CPhysicallyBasedBloomEffecter m_PBBloomEffecter;

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
	virtual void CreateMultiRenderTargetObject(int nWidth,int nHeight, std::vector<RENDERTARGET_INFO> RBOs);
	virtual void BindFrameBufferObject(bool Is_FBO_Clear = true, GLbitfield FBO_Clear_Option = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	virtual void PostProcessing();

	std::vector<std::shared_ptr<CObject>>& GetObjects() { return m_pObjects; };

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

