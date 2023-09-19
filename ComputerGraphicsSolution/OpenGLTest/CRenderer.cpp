#include "CRenderer.h"


GLvoid drawScene(GLvoid)
{
	std::array<GLclampf, 4> f4ClearColor = CRenderer::GetInst()->GetClearColor();
	glClearColor(f4ClearColor[0], f4ClearColor[1], f4ClearColor[2], f4ClearColor[3]);
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

bool CRenderer::init(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	m_glf4ClearColor = { 0.0f,0.0f,1.0f,1.0f };
	m_nFrameWidth = 800;
	m_nFrameHeight = 600;

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

		return true;
}

void CRenderer::SetClearColor(float r, float g, float b, float a)
{
	m_glf4ClearColor[0] = r;
	m_glf4ClearColor[1] = g;
	m_glf4ClearColor[2] = b;
	m_glf4ClearColor[3] = a;
}
