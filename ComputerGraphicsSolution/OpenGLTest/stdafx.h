#pragma once

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <array>
#include <random>

#define DECLARE_SINGLE(MYType)\
public:\
	static MYType* GetInst()\
	{\
		static MYType m_pInst;\
		return &m_pInst;\
	}

extern std::random_device rd;
extern std::default_random_engine dre;