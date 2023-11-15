#pragma once

#include <stdio.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <sstream>
#include <random>
#include <chrono>


#include "Dependencies/glew.h"
#include "Dependencies/freeglut.h"
#include "Dependencies/freeglut_ext.h"
#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/ext.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"
#include "Imgui/imgui.h"
#include "Imgui/imgui_internal.h"
#include "Imgui/imgui_impl_opengl3.h"
#include "Imgui/imgui_impl_glut.h"
#include <DirectXMath.h>
#include <DirectXCollision.h>


extern int ReadIntegerFromFile(FILE* pInFile);
extern float ReadFloatFromFile(FILE* pInFile);
extern BYTE ReadStringFromFile(FILE* pInFile, char* pstrToken);
extern float Distance(glm::vec4 plane, glm::vec3 p);

extern std::random_device rd;
extern std::default_random_engine dre;