#include "stdafx.h"
#include <iostream>
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"

#include "Renderer.h"
#include "Camera.h"
#include "Object.h"
#include "Mesh.h"

CRenderer* g_Renderer = NULL;
std::unique_ptr<CCamera> g_pMainCamera = nullptr;
std::unique_ptr<CObject> g_pTestObj = nullptr;

int g_WindowSizeX = 1280;
int g_WindowSizeY = 768;

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClear(GL_DEPTH_BUFFER_BIT);
	//g_Renderer->DrawAlphaClear();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	GLuint s_Program = g_Renderer->TestShader;
	glUseProgram(s_Program);
	g_pMainCamera->BindShaderVariables(s_Program);
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
	RenderScene();
}

void KeyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		g_pMainCamera->SetPosision(g_pMainCamera->GetPosition() + glm::vec3(0, 0, -0.1));
		break;
	case 's':
		g_pMainCamera->SetPosision(g_pMainCamera->GetPosition() + glm::vec3(0, 0, 0.1));
		break;
	case 'd':
		g_pMainCamera->SetPosision(g_pMainCamera->GetPosition() + glm::vec3(0.1, 0, 0));
		break;
	case 'a':
		g_pMainCamera->SetPosision(g_pMainCamera->GetPosition() + glm::vec3(-0.1, 0, 0));
		break;
	case 'q':
		g_pMainCamera->SetPosision(g_pMainCamera->GetPosition() + glm::vec3(0, -0.1, 0));
		break;
	case 'e':
		g_pMainCamera->SetPosision(g_pMainCamera->GetPosition() + glm::vec3(0, 0.1, 0));
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
	g_Renderer = new CRenderer(g_WindowSizeX, g_WindowSizeY);
	if (!g_Renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}

	g_pMainCamera = std::make_unique<CCamera>();

	g_pMainCamera->RegenarationViewMatrix();
	g_pMainCamera->GenerateProjectionMatrix(glm::radians(65.0f), (float)g_WindowSizeX / (float)g_WindowSizeY, 0.1f, 50.0f);

	g_pTestObj = std::make_unique<CObject>();
	std::shared_ptr<CMesh> testCubeMesh;
	testCubeMesh = CMesh::CreateCubeMesh(1.0f, 1.0f, 1.0f);
	testCubeMesh->CreateShaderVariables();
	g_pTestObj->SetMesh(testCubeMesh.get());

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyInput);
	glutMouseFunc(MouseInput);
	glutSpecialFunc(SpecialKeyInput);

	glutMainLoop();

	delete g_Renderer;

	return 0;
}