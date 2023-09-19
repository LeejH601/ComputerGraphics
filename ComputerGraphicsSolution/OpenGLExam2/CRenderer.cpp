#include "CRenderer.h"
#include <glm/glm/glm.hpp>

std::vector<glm::vec4> BigRects;
std::vector<glm::vec4> SmallRects;

GLvoid drawScene(GLvoid)
{
	std::array<GLclampf, 4> f4ClearColor = CRenderer::GetInst()->GetClearColor();
	glClearColor(f4ClearColor[0], f4ClearColor[1], f4ClearColor[2], f4ClearColor[3]);
	glClear(GL_COLOR_BUFFER_BIT);


	float x = 2;
	float y = 2;

	

	if (BigRects.size() == 0){
		BigRects.resize(4);
		BigRects[0] = glm::vec4(0.f, 0.f, x / 2, y / 2);
		BigRects[1] = glm::vec4(0, -y / 2, x / 2, 0);
		BigRects[2] = glm::vec4(-x / 2, -y / 2, 0, 0);
		BigRects[3] = glm::vec4(-x / 2, 0, 0, y / 2 );
	}

	if (SmallRects.size() == 0) {
		SmallRects.resize(4);
		SmallRects[0] = glm::vec4(0.f, 0.f, x / 2, y / 2);
		SmallRects[0].x += 0.1f; SmallRects[0].y += 0.1f;
		SmallRects[0].z -= 0.1f; SmallRects[0].w -= 0.1f;
		SmallRects[1] = glm::vec4(0, -y / 2, x / 2, 0);
		SmallRects[1].x += 0.1f; SmallRects[1].y += 0.1f;
		SmallRects[1].z -= 0.1f; SmallRects[1].w -= 0.1f;
		SmallRects[2] = glm::vec4(-x / 2, -y / 2, 0, 0);
		SmallRects[2].x += 0.1f; SmallRects[2].y += 0.1f;
		SmallRects[2].z -= 0.1f; SmallRects[2].w -= 0.1f;
		SmallRects[3] = glm::vec4(-x / 2, 0, 0, y / 2);
		SmallRects[3].x += 0.1f; SmallRects[3].y += 0.1f;
		SmallRects[3].z -= 0.1f; SmallRects[3].w -= 0.1f;
	}


	

	for (int i = 0; i < 4; ++i) {
		std::array<GLfloat, 3> colors = CRenderer::GetInst()->GetBigRectColorFromIndex(i);
		CRenderer::GetInst()->DrawRect(BigRects[i].x, BigRects[i].y, BigRects[i].z, BigRects[i].w, colors[0], colors[1], colors[2]);
		//CRenderer::GetInst()->DrawRect(BigRects[i], colors[0], colors[1], colors[2]);
	}

	for (int i = 0; i < 4; ++i) {
		std::array<GLfloat, 3> colors = CRenderer::GetInst()->GetSmallRectColorFromIndex(i);
		CRenderer::GetInst()->DrawRect(SmallRects[i], colors[0], colors[1], colors[2]);
	}

	//CRenderer::GetInst()->DrawRect(-0.5,-0.5, 0.5, 0.5, 1,1,1);
	
	/*colors = CRenderer::GetInst()->GetRectColorFromIndex(1);
	CRenderer::GetInst()->DrawRect(0, -y / 2, x / 2, 0, colors[0], colors[1], colors[2]);
	colors = CRenderer::GetInst()->GetRectColorFromIndex(2);
	CRenderer::GetInst()->DrawRect(-x / 2, -y / 2, 0, 0, colors[0], colors[1], colors[2]);
	colors = CRenderer::GetInst()->GetRectColorFromIndex(3);
	CRenderer::GetInst()->DrawRect(-x / 2, 0, 0, y / 2, colors[0], colors[1], colors[2]);*/

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void MouseInput(int button, int state, int x, int y)
{
	static std::uniform_real_distribution<float> urd_color(0.0f, 1.0f);

	float rx = (float)x / CRenderer::GetInst()->GetWidth();
	rx = rx * 2.0f - 1.0f;
	float ry = (float)y / CRenderer::GetInst()->GetHeight();
	ry = ry * 2.0f - 1.0f;
	ry *= -1;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		int i = 0;
		bool flag = false;
		for (auto& rect : SmallRects) {
			if (rx > rect.x && rx < rect.z) {
				if (ry > rect.y && ry < rect.w) {
					CRenderer::GetInst()->m_f3SmallRectColors[i][0] = urd_color(dre);
					CRenderer::GetInst()->m_f3SmallRectColors[i][1] = urd_color(dre);
					CRenderer::GetInst()->m_f3SmallRectColors[i][2] = urd_color(dre);
					flag = true;
				}
			}
			i++;
		}
		if (flag)
			return;
		i = 0;
		flag = false;
		for (auto& rect : BigRects) {
			if (rx > rect.x && rx < rect.z) {
				if (ry > rect.y && ry < rect.w) {
					CRenderer::GetInst()->m_f3BigRectColors[i][0] = urd_color(dre);
					CRenderer::GetInst()->m_f3BigRectColors[i][1] = urd_color(dre);
					CRenderer::GetInst()->m_f3BigRectColors[i][2] = urd_color(dre);
					std::cout << i << std::endl;
				}
			}
			i++;
		}
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		int i = 0;
		bool flag = false;
		for (auto& rect : SmallRects) {
			if (rx > rect.x && rx < rect.z) {
				if (ry > rect.y && ry < rect.w) {
					glm::vec4 cache = rect;
					rect.x = rect.x + 0.1f;
					rect.y += 0.1f;
					rect.z -= 0.1f;
					rect.w -= 0.1f;
					if (rect.x >= rect.z)
						rect = cache;
					flag = true;
				}
			}
			i++;
		}
		if (flag)
			return;
		i = 0;
		flag = false;
		for (auto& rect : BigRects) {
			if (rx > rect.x && rx < rect.z) {
				if (ry > rect.y && ry < rect.w) {
					glm::vec4 cache = rect;
					SmallRects[i].x -= 0.1f;
					SmallRects[i].y -= 0.1f;
					SmallRects[i].z += 0.1f;
					SmallRects[i].w += 0.1f;
					if (rect.x >= 1.0)
						rect = cache;

					std::cout << i << std::endl;
				}
			}
			i++;
		}
	}
}

bool CRenderer::init(int argc, char** argv)
{
	static std::uniform_real_distribution<float> urd_color(0.0f, 1.0f);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	m_glf4ClearColor = { 0.0f,0.0f,1.0f,1.0f };
	m_nFrameWidth = 800;
	m_nFrameHeight = 600;

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
	glutReshapeFunc(Reshape);
	glutMouseFunc(MouseInput);

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
