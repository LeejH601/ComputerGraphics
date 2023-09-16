#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include "stdafx.h"



class CRenderer
{
public:
	CRenderer(int windowSizeX, int windowSizeY);
	~CRenderer();

	bool IsInitialized();
	void DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a);
	
	GLuint TestShader = -1;

private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	int MakeVertexShader(const char* vertexPath, int n);
	int MakeFragmentShader(const char* fragmentPath, int count);
	void checkCompileErrors(unsigned int shader, std::string type);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void GetGLPosition(float x, float y, float *newX, float *newY);

	bool m_Initialized = false;

	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;


};

