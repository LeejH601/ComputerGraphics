#include "Scene.h"
#include "CRenderer.h"

void CScene::Init()
{
}

void CScene::drawScene()
{
}

void CScene::Update(float fTimeElapsed)
{
}

void CScene::MouseInput(int button, int state, int x, int y)
{
}

void CScene::MouseMotion(int x, int y)
{
}

void CScene::KeyInput(unsigned char key, int x, int y)
{
}

void CScene::KeyUpInput(unsigned char key, int x, int y)
{
}




void CScene_4::drawScene()
{
	std::array<GLclampf, 4> f4ClearColor = CRenderer::GetInst()->GetClearColor();
	glClearColor(f4ClearColor[0], f4ClearColor[1], f4ClearColor[2], f4ClearColor[3]);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < Rects.size(); ++i) {
		CRenderer::GetInst()->DrawRect(Rects[i].x, Rects[i].y, Rects[i].z, Rects[i].w, Colors[i].x, Colors[i].y, Colors[i].z);
	}
}

void CScene_4::Update(float fTimeElapsed)
{
	std::cout << "fElapsedTime" << std::endl;

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

void CScene_4::MouseInput(int button, int state, int x, int y)
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
			Rect_Dirs.back() = glm::normalize(glm::vec2(1.0f, -1.0f));
		}
	}
}

void CScene_4::MouseMotion(int x, int y)
{

	Curr_x = (float)x / CRenderer::GetInst()->GetWidth();
	Curr_x = Curr_x * 2.0f - 1.0f;
	Curr_Y = (float)y / CRenderer::GetInst()->GetHeight();
	Curr_Y = Curr_Y * 2.0f - 1.0f;
	Curr_Y *= -1;

}

void CScene_4::KeyInput(unsigned char key, int x, int y)
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
}

CScene_5::CScene_5()
{
	Init();
}

void CScene_5::Init()
{
	static std::uniform_int_distribution<int> uid_rects(20, 40);
	static std::uniform_real_distribution<float> urd_pos(-1.f, 1.f);
	static std::uniform_real_distribution<float> urd_color(0.f, 1.f);

	Rects.clear();
	Rects_Collided.clear();
	Colors.clear();

	int nRects = uid_rects(dre);
	for (int i = 0; i < nRects; ++i) {
		glm::vec2 pos{ urd_pos(dre), urd_pos(dre) };
		float extend = 0.05f;
		Rects.emplace_back(pos.x - extend, pos.y - extend, pos.x + extend, pos.y + extend);
		Rects_Collided.emplace_back(false);
		Colors.emplace_back(urd_color(dre), urd_color(dre), urd_color(dre), 1.0f);
	}


}

void CScene_5::drawScene()
{
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < Rects.size(); ++i) {
		if(!Rects_Collided[i])
			CRenderer::GetInst()->DrawRect(Rects[i].x, Rects[i].y, Rects[i].z, Rects[i].w, Colors[i].x, Colors[i].y, Colors[i].z);
	}

	if (isEarse) {
		CRenderer::GetInst()->DrawRect(earserPos.x - earserSize.x, earserPos.y - earserSize.y, earserPos.x + earserSize.x, earserPos.y + earserSize.y,
			earserColor.x, earserColor.y, earserColor.z);
	}
}

void CScene_5::Update(float fTimeElapsed)
{
	for (int i = 0; i < Rects.size(); ++i) {
		if (intersect(glm::vec4(earserPos.x - earserSize.x, earserPos.y - earserSize.y, earserPos.x + earserSize.x, earserPos.y + earserSize.y),
			Rects[i]) && Rects_Collided[i] == false) {
			Rects_Collided[i] = true;
			earserColor = Colors[i];
			earserSize += glm::vec2(0.01f, 0.01f);
		}
	}
}

void CScene_5::MouseInput(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mouse_X = (float)x / CRenderer::GetInst()->GetWidth();
		mouse_X = mouse_X * 2.0f - 1.0f;
		mouse_Y = (float)y / CRenderer::GetInst()->GetHeight();
		mouse_Y = mouse_Y * 2.0f - 1.0f;
		mouse_Y *= -1;
		Curr_x = mouse_X;
		Curr_Y = mouse_Y;
		earserPos = glm::vec2(mouse_X, mouse_Y);
		earserSize = glm::vec2(0.05f, 0.05f);
		earserColor = glm::vec4(0, 0, 0, 1);
		isEarse = true;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		isEarse = false;
	}
}

void CScene_5::MouseMotion(int x, int y)
{
	mouse_X = (float)x / CRenderer::GetInst()->GetWidth();
	mouse_X = mouse_X * 2.0f - 1.0f;
	mouse_Y = (float)y / CRenderer::GetInst()->GetHeight();
	mouse_Y = mouse_Y * 2.0f - 1.0f;
	mouse_Y *= -1;

	earserPos.x = mouse_X;
	earserPos.y = mouse_Y;
}

void CScene_5::KeyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r':
		Init();
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	}
}

bool CScene_5::intersect(glm::vec4 r1, glm::vec4 r2)
{
	if (r1.x > r2.z)
		return false;
	if (r1.y > r2.w)
		return false;
	if (r1.z < r2.x)
		return false;
	if (r1.w < r2.y)
		return false;

	return true;
}
