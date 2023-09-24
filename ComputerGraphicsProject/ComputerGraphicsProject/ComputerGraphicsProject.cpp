#include "stdafx.h"
#include <iostream>
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"

#include "Renderer.h"
#include "Camera.h"
#include "Object.h"
#include "Mesh.h"
#include "Light.h"
#include "Timer.h"

CRenderer* g_Renderer = NULL;
std::unique_ptr<CCamera> g_pMainCamera = nullptr;
std::unique_ptr<CObject> g_pTestObj = nullptr;
std::unique_ptr<CObject> g_pTestObj2 = nullptr;
std::vector<std::shared_ptr<CObject>> g_TestObjects;
std::unique_ptr<CGameTimer> g_Timer;
CLight* g_SunLight = nullptr;
UINT gDwDirection;
float g_cxDelta, g_cyDelta;


enum class MOUSE_STATE {
	MOUSE_CILCK_LEFT,
	MOUSE_CILCK_RIGHT,
	MOUSE_CILCK_NONE,
};

MOUSE_STATE g_eMouseState = MOUSE_STATE::MOUSE_CILCK_NONE;
POINT g_ptOldMouseCursor{ 0,0 };
POINT g_ptCurrMouseCuror;

#define DW_FRONT 0x01
#define DW_BACK  0x02
#define DW_RIGHT 0x04
#define DW_LEFT	 0x08
#define DW_UP	 0x10
#define DW_DOWN  0x20
#define DW_SHIFT 0x40

int g_WindowSizeX = 1280;
int g_WindowSizeY = 768;

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	//glClear(GL_DEPTH_BUFFER_BIT);
	//g_Renderer->DrawAlphaClear();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT, GL_FILL);
	//glPolygonMode(GL_BACK, GL_LINE);

	

	g_Timer->Tick();
	float fTime = g_Timer->GetTotalTime();
	float fElapesdTime = g_Timer->GetFrameTimeElapsed();

	glm::vec3 cameraPosition = g_pMainCamera->GetPosition();
	glm::vec3 cameraVelocity = glm::vec3(0);

	if (gDwDirection != 0) {
		if (gDwDirection & DW_FRONT)
			cameraVelocity -= g_pMainCamera->m_vec3Look;
		if (gDwDirection & DW_BACK)
			cameraVelocity += g_pMainCamera->m_vec3Look;
		if (gDwDirection & DW_RIGHT)
			cameraVelocity += g_pMainCamera->m_vec3Right;
		if (gDwDirection & DW_LEFT)
			cameraVelocity -= g_pMainCamera->m_vec3Right;
		if (gDwDirection & DW_UP)
			cameraVelocity += g_pMainCamera->m_vec3Up;
		if (gDwDirection & DW_DOWN)
			cameraVelocity -= g_pMainCamera->m_vec3Up;
		cameraVelocity = glm::normalize(cameraVelocity);
	}

	if(!glm::all(glm::isnan(cameraVelocity)))
		cameraPosition += cameraVelocity * g_Timer->GetFrameTimeElapsed();
	g_pMainCamera->SetPosision(cameraPosition);

	if (g_eMouseState == MOUSE_STATE::MOUSE_CILCK_RIGHT) {
		if (abs(g_cxDelta) > 0.0001f || abs(g_cyDelta > 0.0001f)) {
			glm::vec3 axis = glm::vec3(0, 1, 0);
			glm::qua<float> qResult = glm::rotate(g_pMainCamera->m_vec4Rotation, glm::radians(g_cxDelta * 180.0f) * fElapesdTime, glm::normalize(axis));
			axis = glm::vec3(1, 0, 0);
			qResult = glm::rotate(qResult, glm::radians(g_cyDelta * 180.0f) * fElapesdTime, glm::normalize(axis));

			g_pMainCamera->m_vec4Rotation = qResult;

			g_cxDelta = 0;
			g_cyDelta = 0;
		}
	}
	
	//g_pMainCamera->m_vec4Rotation = glm::normalize(qResult);

	g_SunLight->m_vec3LightColor = glm::vec3(1, 1, 1);
	g_SunLight->m_vec3Direction = glm::normalize(glm::vec3(-1, -1, -1));
	g_SunLight->m_vec3Direction = glm::vec3(sin(fTime), cos(fTime), -1);

	GLuint s_Program = g_Renderer->TestShader;
	glUseProgram(s_Program);

	
	GLuint samplerULoc = glGetUniformLocation(s_Program, "u_IrradianceTexture");
	glUniform1i(samplerULoc, 2);


	g_SunLight->BindShaderVariables(s_Program);
	g_pMainCamera->BindShaderVariables(s_Program);

	g_pTestObj2->BindShaderVariables(s_Program);
	g_Renderer->m_tIrradianceTexture->BindShaderVariables(s_Program, GL_TEXTURE2);

	g_pTestObj2->Render();

	for (std::shared_ptr<CObject>& obj : g_TestObjects) {
		obj->BindShaderVariables(s_Program);
		g_Renderer->m_tIrradianceTexture->BindShaderVariables(s_Program, GL_TEXTURE2);

		obj->Render();
	}

	s_Program = g_Renderer->SkyBoxShader;
	glUseProgram(s_Program);

	g_SunLight->BindShaderVariables(s_Program);
	g_pMainCamera->BindShaderVariables(s_Program);

	glDepthFunc(GL_LEQUAL);

	g_pTestObj->BindShaderVariables(s_Program);
	g_pTestObj->Render();

	


	/*g_pMainCamera->BindShaderVariables(s_Program);

	g_pTestObj->BindShaderVariables(s_Program);


	g_pTestObj->Render();*/


	glutSwapBuffers();
}

void Idle(void)
{
	RenderScene();
}

void MouseInput(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		g_eMouseState = MOUSE_STATE::MOUSE_CILCK_RIGHT;
		g_ptOldMouseCursor = { x,y };
		g_ptOldMouseCursor.x -= g_WindowSizeX / 2;
		g_ptOldMouseCursor.y -= g_WindowSizeY / 2;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
		g_eMouseState = MOUSE_STATE::MOUSE_CILCK_NONE;
	}
	RenderScene();
}

void MouseMotion(int x, int y)
{
	switch (g_eMouseState)
	{
	case MOUSE_STATE::MOUSE_CILCK_LEFT:
		break;
	case MOUSE_STATE::MOUSE_CILCK_RIGHT:
		g_ptCurrMouseCuror = { x,y };
		g_ptCurrMouseCuror.x -= g_WindowSizeX / 2;
		g_ptCurrMouseCuror.y -= g_WindowSizeY / 2;
		g_cxDelta = (float)(g_ptCurrMouseCuror.x - g_ptOldMouseCursor.x);
		g_cyDelta = (float)(g_ptCurrMouseCuror.y - g_ptOldMouseCursor.y);
		g_ptOldMouseCursor = g_ptCurrMouseCuror;

		break;
	case MOUSE_STATE::MOUSE_CILCK_NONE:
		break;
	default:
		break;
	}
	RenderScene();
}

void KeyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		gDwDirection |= DW_FRONT;
		break;
	case 's':
		gDwDirection |= DW_BACK;
		break;
	case 'd':
		gDwDirection |= DW_RIGHT;
		break;
	case 'a':
		gDwDirection |= DW_LEFT;
		break;
	case 'q':
		gDwDirection |= DW_DOWN;
		break;
	case 'e':
		gDwDirection |= DW_UP;
		break;
	case 'f':
		g_pMainCamera->m_vec3Look.x += 0.1f;
		g_pMainCamera->m_vec3Look = glm::normalize(g_pMainCamera->m_vec3Look);
		break;
	case 'g':
		g_pMainCamera->m_vec3Look.x -= 0.1f;
		g_pMainCamera->m_vec3Look = glm::normalize(g_pMainCamera->m_vec3Look);
		break;
	default:
		break;
	}
	glm::vec3 pos = g_pMainCamera->GetPosition();
	std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
	RenderScene();
}

void KeyUpInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		gDwDirection &= (~DW_FRONT);
		break;
	case 's':
		gDwDirection &= (~DW_BACK);
		break;
	case 'd':
		gDwDirection &= (~DW_RIGHT);
		break;
	case 'a':
		gDwDirection &= (~DW_LEFT);
		break;
	case 'q':
		gDwDirection &= (~DW_DOWN);
		break;
	case 'e':
		gDwDirection &= (~DW_UP);
		break;
	default:
		break;
	}
	RenderScene();
}

void SpecialKeyInput(int key, int x, int y)
{
	RenderScene();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(g_WindowSizeX, g_WindowSizeY);
	glutCreateWindow("Game Software Engineering KPU");

	glewInit();
	if (glewIsSupported("GL_VERSION_3_0"))
	{
		std::cout << " GLEW Version is 3.0\n ";
	}
	else
	{
		std::cout << "GLEW 3.0 not supported\n ";
	}

	// Initialize Renderer
	g_Timer = std::make_unique<CGameTimer>();
	g_Renderer = new CRenderer(g_WindowSizeX, g_WindowSizeY);
	if (!g_Renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}

	g_SunLight = new CLight();
	g_pMainCamera = std::make_unique<CCamera>();

	g_pMainCamera->RegenarationViewMatrix();
	g_pMainCamera->GenerateProjectionMatrix(glm::radians(90.0f), (float)g_WindowSizeX / (float)g_WindowSizeY, 0.1f, 50.0f);

	g_pTestObj = std::make_unique<CObject>();
	g_pTestObj2 = std::make_unique<CObject>();

	std::shared_ptr<CMesh> testCubeMesh;
	testCubeMesh = CMesh::CreateCubeMesh(1.0f, 1.0f, 1.0f);
	testCubeMesh->CreateShaderVariables();

	std::shared_ptr<CMesh> testSphereMesh;
	testSphereMesh = CMesh::CreateSphereMesh(20, 20);
	testSphereMesh->CreateShaderVariables();

	std::shared_ptr<CMaterial> pMaterial = std::make_shared<CMaterial>();
	std::shared_ptr<CMaterial> pMaterial2 = std::make_shared<CMaterial>();
	std::shared_ptr<CTexture> pTexture = g_Renderer->m_tIrradianceTexture;
	//pTexture->LoadTextureFromPNG("./Textures/rgb.png", GL_NEAREST);
	//pTexture->LoadTextureHDR("./Textures/poly_haven_studio_4k.hdr", GL_LINEAR);
	pMaterial->SetBaseTexture(pTexture);

	//g_pTestObj->LoadGeometryAndAnimationFromFile("./Objects/TestModel.bin");
	//g_pTestObj->SetMesh(testCubeMesh);
	//g_pTestObj->SetMaterial(pMaterial2);

	g_pTestObj->SetMesh(testCubeMesh);
	g_pTestObj->SetMaterial(pMaterial);


	g_pTestObj2->LoadGeometryAndAnimationFromFile("./Objects/Plane.bin");
	g_pTestObj2->GetMaterial(0)->RoughnessColor = 1.0f;
	g_pTestObj2->GetMaterial(0)->FresnelColor = 1.3f;

	g_TestObjects.resize(10);
	glm::vec3 basePos{ 0,1.5,0 };
	/*for (int i = 0; i < 10; ++i) {
		g_TestObjects[i] = std::make_shared<CObject>();
		g_TestObjects[i]->SetMesh(testSphereMesh);
		g_TestObjects[i]->SetPosition(basePos);
		std::shared_ptr<CMaterial> Material = std::make_shared<CMaterial>();
		Material->RoughnessColor = (0.1f + (i * 0.1f));
		g_TestObjects[i]->SetMaterial(Material);
		basePos.x += 2.0f;
	}*/
	for (int i = 0; i < 10; ++i) {
		g_TestObjects[i] = std::make_shared<CObject>();
		g_TestObjects[i]->LoadGeometryAndAnimationFromFile("./Objects/TestModel.bin");
		g_TestObjects[i]->SetPosition(basePos);
		basePos.x += 2.0f;
	}

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyInput);
	glutKeyboardUpFunc(KeyUpInput);
	glutMouseFunc(MouseInput);
	glutMotionFunc(MouseMotion);
	glutSpecialFunc(SpecialKeyInput);

	g_Timer->Start();
	glutMainLoop();

	delete g_Renderer;
	delete g_SunLight;

	return 0;
}