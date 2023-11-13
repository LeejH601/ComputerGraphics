#include "stdafx.h"
#include <iostream>
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"


#include "Global.h"
#include "Renderer.h"
#include "Camera.h"
#include "Object.h"
#include "Mesh.h"
#include "Light.h"
#include "Timer.h"
#include "Scene.h"
#include "ExamScene.h"

std::random_device rd;
std::default_random_engine dre(rd());

CRenderer* g_Renderer = NULL;
std::unique_ptr<CObject> g_pTestObj = nullptr;
std::unique_ptr<CObject> g_pTestObj2 = nullptr;
std::vector<std::shared_ptr<CObject>> g_TestObjects;
std::unique_ptr<CGameTimer> g_Timer;

CScene* g_currentScene = nullptr;

int g_WindowSizeX = 1280;
int g_WindowSizeY = 768;


std::vector<std::shared_ptr<CScene>> g_pSceneCache;



void RenderScene(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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


	g_currentScene->Update(fElapesdTime);
	g_currentScene->RenderScene();


	/*g_pMainCamera->BindShaderVariables(s_Program);

	g_pTestObj->BindShaderVariables(s_Program);


	g_pTestObj->Render();*/


	glutSwapBuffers();
	glutPostRedisplay();
}

void Idle(void)
{
	RenderScene();
}


void MouseInput(int button, int state, int x, int y)
{
	g_currentScene->MouseInput(button, state, x, y);
	ImGui_ImplGLUT_MouseFunc(button, state, x, y);
	RenderScene();
}

void MouseMotion(int x, int y)
{
	g_currentScene->MouseMotion(x, y);
	ImGui_ImplGLUT_MotionFunc(x, y);
	RenderScene();
}


void PassiveMotion(int x, int y)
{
	ImGui_ImplGLUT_MotionFunc(x, y);
}

void KeyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	default:
		break;
	}
	g_currentScene->KeyInput(key, x, y);
	ImGui_ImplGLUT_KeyboardFunc(key, x, y);
	RenderScene();
}

void KeyUpInput(unsigned char key, int x, int y)
{
	g_currentScene->KeyUpInput(key, x, y);
	ImGui_ImplGLUT_KeyboardUpFunc(key, x, y);
	RenderScene();
}

void SpecialKeyInput(int key, int x, int y)
{
	ImGui_ImplGLUT_SpecialFunc(key, x, y);
	RenderScene();
}

void SpecialKeyUpInput(int key, int x, int y)
{
	ImGui_ImplGLUT_SpecialUpFunc(key, x, y);
	RenderScene();
}


void ReshapeFunc(int w, int h)
{
	ImGui_ImplGLUT_ReshapeFunc(w, h);
	//RenderScene();
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

	std::shared_ptr<CScene> scene = std::make_shared<CPBR_TestScene>();
	g_pSceneCache.push_back(scene);
	g_currentScene = g_pSceneCache.back().get();
	g_currentScene->Enter();

	/*scene = std::make_shared<CExamScene_8>();
	g_pSceneCache.push_back(scene);*/

	glutDisplayFunc(RenderScene);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.WantCaptureMouse = true;

	ImGui::StyleColorsDark();

	ImGui_ImplGLUT_Init();
	ImGui_ImplOpenGL3_Init();

	ImGui_ImplGLUT_InstallFuncs();

	glutIdleFunc(Idle);
	glutReshapeFunc(ReshapeFunc);
	//glutPassiveMotionFunc(PassiveMotion);
	glutKeyboardFunc(KeyInput);
	glutKeyboardUpFunc(KeyUpInput);
	glutMouseFunc(MouseInput);
	glutMotionFunc(MouseMotion);
	glutSpecialFunc(SpecialKeyInput);

	g_Timer->Start();
	glutMainLoop();

	delete g_Renderer;

	ImGui_ImplGLUT_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

	return 0;
}