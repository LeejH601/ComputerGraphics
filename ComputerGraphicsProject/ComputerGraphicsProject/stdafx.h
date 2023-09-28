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


extern int ReadIntegerFromFile(FILE* pInFile);
extern float ReadFloatFromFile(FILE* pInFile);
extern BYTE ReadStringFromFile(FILE* pInFile, char* pstrToken);

extern std::random_device rd;
extern std::default_random_engine dre;