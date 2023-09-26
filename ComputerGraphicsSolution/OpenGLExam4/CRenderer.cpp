#include "CRenderer.h"
#include <glm/glm.hpp>
#include "Timer.h"
#include "Scene.h"


std::shared_ptr<CScene> g_CurrentScene;


GLvoid drawScene(GLvoid)
{
	g_Timer.Tick();
	float fElapsedTime = g_Timer.GetFrameTimeElapsed();

	g_CurrentScene->Update(fElapsedTime);
	g_CurrentScene->drawScene();

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Update(float fTimeElapsed)
{
	
}

void MouseInput(int button, int state, int x, int y)
{
	g_CurrentScene->MouseInput(button, state, x, y);

	drawScene();
}

void MouseMotion(int x, int y)
{
	g_CurrentScene->MouseMotion(x, y);

	drawScene();
}

void Idle(void)
{
	drawScene();
}

void KeyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '4':
		g_CurrentScene = std::make_shared<CScene_4>();
		break;
	case '5':
		g_CurrentScene = std::make_shared<CScene_5>();
		break;
	case '6':
		g_CurrentScene = std::make_shared<CScene_6>();
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	g_CurrentScene->KeyInput(key, x, y);
	drawScene();
}

void KeyUpInput(unsigned char key, int x, int y)
{
	drawScene();
}

void SpecialKeyInput(int key, int x, int y)
{
	drawScene();
}

bool CRenderer::init(int argc, char** argv)
{
	static std::uniform_real_distribution<float> urd_color(0.0f, 1.0f);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	m_glf4ClearColor = { 0.1f,0.1f,0.1f,1.0f };
	m_nFrameWidth = 800;
	m_nFrameHeight = 800;

	for (auto& colors : m_f3BigRectColors) {
		colors[0] = urd_color(dre);
		colors[1] = urd_color(dre);
		colors[2] = urd_color(dre);
	}

	for (auto& colors : m_f3SmallRectColors) {
		colors[0] = urd_color(dre);
		colors[1] = urd_color(dre);
		colors[2] = urd_color(dre);
	}

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(m_nFrameWidth, m_nFrameHeight);
	glutCreateWindow("Example1");


	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	g_CurrentScene = std::make_shared<CScene_6>();

	glutDisplayFunc(drawScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyInput);
	glutKeyboardUpFunc(KeyUpInput);
	glutSpecialFunc(SpecialKeyInput);
	glutReshapeFunc(Reshape);
	glutMouseFunc(MouseInput);
	glutMotionFunc(MouseMotion);

	g_Timer.Start();

	return true;
}

void CRenderer::SetClearColor(float r, float g, float b, float a)
{
	m_glf4ClearColor[0] = r;
	m_glf4ClearColor[1] = g;
	m_glf4ClearColor[2] = b;
	m_glf4ClearColor[3] = a;
}

void CRenderer::DrawRect(float x1, float y1, float x2, float y2, GLfloat r, GLfloat g, GLfloat b)
{
	glColor3f(r, g, b);
	glRectf(x1, y1, x2, y2);
}

void CRenderer::DrawRect(glm::vec4 rect, GLfloat r, GLfloat g, GLfloat b)
{
	glColor3f(r, g, b);
	glRectf(rect.x, rect.y, rect.z, rect.w);
}
