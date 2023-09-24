#include "stdafx.h"
#include "FrameWork.h"
#include "Timer.h"

std::random_device rd;
std::default_random_engine dre(rd());
CGameTimer g_Timer;

void main(int argc, char** argv)
{
	CFrameWork::GetInst()->init(argc, argv);
	CFrameWork::GetInst()->Run();
}

