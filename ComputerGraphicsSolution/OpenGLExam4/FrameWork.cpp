#include "FrameWork.h"
#include "CRenderer.h"

GLvoid Keyboard(unsigned char key, int x, int y)
{

	switch (key) {
	case 'a':
		break;
	}

	glutPostRedisplay();
}

void TimerFunction(int value)
{
	static std::uniform_real_distribution<float> urd_color(0.0f, 1.0f);

	if (value == 0)
		return;

	CRenderer::GetInst()->SetClearColor(urd_color(dre), urd_color(dre), urd_color(dre), 1);
	glutPostRedisplay();

	glutTimerFunc(CFrameWork::GetInst()->GetColorTimer(), TimerFunction, CFrameWork::GetInst()->GetAutoColorChange());
}

bool CFrameWork::init(int argc, char** argv)
{
	CRenderer::GetInst()->init(argc, argv);


	return true;
}

void CFrameWork::Run()
{
	glutMainLoop();
}

void CFrameWork::DrawRects()
{
}
