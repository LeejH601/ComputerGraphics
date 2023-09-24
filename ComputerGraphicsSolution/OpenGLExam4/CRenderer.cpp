#include "CRenderer.h"
#include <glm/glm.hpp>
#include "Timer.h"

std::vector<glm::vec4> Colors;
std::vector<glm::vec4> Rects;
std::vector<glm::vec4> origin_Rects;
std::vector<glm::vec2> Rect_Dirs;
float mouse_X, mouse_Y;
float Curr_x, Curr_Y;

enum class MOVE_STATE {
	MOVE1,
	MOVE2,
	NONE,
};

MOVE_STATE g_mouseState = MOVE_STATE::NONE;

int shapeState = 0;
int colorState = 0;

GLvoid drawScene(GLvoid)
{
	std::array<GLclampf, 4> f4ClearColor = CRenderer::GetInst()->GetClearColor();
	glClearColor(f4ClearColor[0], f4ClearColor[1], f4ClearColor[2], f4ClearColor[3]);
	glClear(GL_COLOR_BUFFER_BIT);

	g_Timer.Tick();
	float fElapsedTime = g_Timer.GetFrameTimeElapsed();

	Update(fElapsedTime);


	for (int i = 0; i < Rects.size(); ++i) {
		CRenderer::GetInst()->DrawRect(Rects[i].x, Rects[i].y, Rects[i].z, Rects[i].w, Colors[i].x, Colors[i].y, Colors[i].z);
	}


	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Update(float fTimeElapsed)
{
	static std::uniform_real_distribution<float> uid_color(0.0f, 1.0f);
	static float temp_time = 0.0f;
	static int prev_temp_time = 0;
	static float s_temp_time = 0.0f;
	static int s_prev_temp_time = 0;
	static float c_temp_time = 0.0f;
	static int c_prev_temp_time = 0;
	static glm::vec2 extend = { 0.1f, -0.1f };


	if (shapeState == 1) {
		s_temp_time += fTimeElapsed;
		for (int i = 0; i < Rects.size(); ++i) {
			Rects[i] = Rects[i] + glm::vec4(-extend.x, -extend.y, extend.x, extend.y) * fTimeElapsed;
		}

		if (s_prev_temp_time != (int)(s_temp_time)) {
			for (int i = 0; i < Rects.size(); ++i) {
				extend.x = -extend.x;
				extend.y = -extend.y;

				s_prev_temp_time = (int)(s_temp_time);
			}
		}
	}

	if (colorState == 1) {
		c_temp_time += fTimeElapsed;
		

		if (c_prev_temp_time != (int)(c_temp_time)) {
			for (int i = 0; i < Rects.size(); ++i) {
				Colors[i] = glm::vec4(uid_color(dre), uid_color(dre), uid_color(dre), 1.0f);

				c_prev_temp_time = (int)(c_temp_time);
			}
		}
	}

	if (g_mouseState == MOVE_STATE::MOVE1) {
		for (int i = 0; i < Rects.size(); ++i) {
			Rects[i] = Rects[i] + glm::vec4(Rect_Dirs[i].x, Rect_Dirs[i].y, Rect_Dirs[i].x, Rect_Dirs[i].y) * fTimeElapsed;
		}

		for (int i = 0; i < Rects.size(); ++i) {
			if (Rects[i].x < -1.f || Rects[i].z > 1.0f)
				Rect_Dirs[i].x = -Rect_Dirs[i].x;
			if (Rects[i].y < -1.f || Rects[i].w > 1.0f)
				Rect_Dirs[i].y = -Rect_Dirs[i].y;
		}
	}
	if (g_mouseState == MOVE_STATE::MOVE2) {
		temp_time += fTimeElapsed;

		for (int i = 0; i < Rects.size(); ++i) {
			Rects[i] = Rects[i] + glm::vec4(Rect_Dirs[i].x, Rect_Dirs[i].y, Rect_Dirs[i].x, Rect_Dirs[i].y) * fTimeElapsed;
		}

		if (prev_temp_time != (int)(temp_time)) {
			for (int i = 0; i < Rects.size(); ++i) {
				Rect_Dirs[i].y = -Rect_Dirs[i].y;
				prev_temp_time = (int)(temp_time);
			}
		}

		for (int i = 0; i < Rects.size(); ++i) {
			if (Rects[i].x < -1.f) {
				Rects[i].x += 2.0f;
				Rects[i].z += 2.0f;
			}
				
			if (Rects[i].z > 1.0f) {
				Rects[i].x -= 2.0f;
				Rects[i].z -= 2.0f;
			}
		}
	}
}

void MouseInput(int button, int state, int x, int y)
{
	static std::uniform_real_distribution<float> uid_color(0.0f, 1.0f);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mouse_X = (float)x / CRenderer::GetInst()->GetWidth();
		mouse_X = mouse_X * 2.0f - 1.0f;
		mouse_Y = (float)y / CRenderer::GetInst()->GetHeight();
		mouse_Y = mouse_Y * 2.0f - 1.0f;
		mouse_Y *= -1;
		Curr_x = mouse_X;
		Curr_Y = mouse_Y;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		float extend_X = abs(Curr_x - mouse_X);
		extend_X = 0.1f;
		float extend_Y = abs(Curr_Y - mouse_Y);
		extend_Y = 0.1f;
		if (Rects.size() < 5) {
			Rects.emplace_back(mouse_X - extend_X, mouse_Y - extend_Y, mouse_X + extend_X, mouse_Y + extend_Y);
			origin_Rects.emplace_back(Rects.back());
			Colors.emplace_back(uid_color(dre), uid_color(dre), uid_color(dre), 1.0f);
			Rect_Dirs.emplace_back(0, 0);
			Rect_Dirs.back() = glm::normalize(glm::vec2(1.0f,  -1.0f));
		}
	}

	drawScene();
}

void MouseMotion(int x, int y)
{

	Curr_x = (float)x / CRenderer::GetInst()->GetWidth();
	Curr_x = Curr_x * 2.0f - 1.0f;
	Curr_Y = (float)y / CRenderer::GetInst()->GetHeight();
	Curr_Y = Curr_Y * 2.0f - 1.0f;
	Curr_Y *= -1;

	
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
	case 'a':
		if (g_mouseState == MOVE_STATE::MOVE1) {
			g_mouseState = MOVE_STATE::NONE;
			break;
		}
		for (auto& dir : Rect_Dirs) {
			dir.x = 1.0f;
			dir.y = -1.0f;
			dir = glm::normalize(dir);
		}
		g_mouseState = MOVE_STATE::MOVE1;
		break;
	case 'i':
		if (g_mouseState == MOVE_STATE::MOVE2) {
			g_mouseState = MOVE_STATE::NONE;
			break;
		}
		g_mouseState = MOVE_STATE::MOVE2;
		break;
	case 'c':
		if (shapeState == 1) {
			shapeState = 0;
			break;
		}
		shapeState = 1;
		break;
	case 'o':
		if (colorState == 1) {
			colorState = 0;
			break;
		}
		colorState = 1;
		break;
	case 's':
		g_mouseState = MOVE_STATE::NONE;
		colorState = 0;
		shapeState = 0;
		break;
	case 'm':
		for (int i = 0; i < Rects.size(); ++i)
			Rects[i] = origin_Rects[i];
		break;
	case 'r':
		Rects.clear();
		origin_Rects.clear();
		Colors.clear();
		Rect_Dirs.clear();
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	}
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

	m_glf4ClearColor = { 0.0f,0.0f,1.0f,1.0f };
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
