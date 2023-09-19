#include "stdafx.h"
#include "FrameWork.h"

std::random_device rd;
std::default_random_engine dre(rd());


void main(int argc, char** argv)
{
	CFrameWork::GetInst()->init(argc, argv);
	CFrameWork::GetInst()->Run();
}

