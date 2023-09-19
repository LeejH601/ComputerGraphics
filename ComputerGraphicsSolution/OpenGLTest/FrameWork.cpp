#include "FrameWork.h"
#include "CRenderer.h"

GLvoid Keyboard(unsigned char key, int x, int y)
{
	static std::uniform_real_distribution<float> urd_color(0.0f, 1.0f);

	switch (key) {
	case  'c':
		CRenderer::GetInst()->SetClearColor(0, 1, 1, 1);
		break;
	case 'm':
		CRenderer::GetInst()->SetClearColor(1, 0, 1, 1);
		break;
	case 'y':
		CRenderer::GetInst()->SetClearColor(1, 1, 0, 1);
		break;
	case 'a':
		CRenderer::GetInst()->SetClearColor(urd_color(dre), urd_color(dre), urd_color(dre), 1);
		break;
	case 'w':
		CRenderer::GetInst()->SetClearColor(1, 1, 1, 1);
		break;
	case 'b':
		CRenderer::GetInst()->SetClearColor(0, 0, 0, 1);
		break;
	case 't':
		CFrameWork::GetInst()->SetAutoColorChange(true);
		glutTimerFunc(CFrameWork::GetInst()->GetColorTimer(), TimerFunction, CFrameWork::GetInst()->GetAutoColorChange());
		break;
	case 's':
		CFrameWork::GetInst()->SetAutoColorChange(false);
		break;
	default:
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

	glutKeyboardFunc(Keyboard);

	return true;
}

void CFrameWork::Run()
{
	glutMainLoop();
}
