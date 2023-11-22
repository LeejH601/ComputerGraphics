#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include "stdafx.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"


class CRenderer
{
public:
	CRenderer(int windowSizeX, int windowSizeY);
	~CRenderer();

	bool IsInitialized();
	void DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a);
	
	GLuint TestShader = -1;
	GLuint CubeShader = -1;
	GLuint MakeCubeMapShader = -1;
	GLuint SkyBoxShader = -1;
	GLuint irradianceShader = -1;
	GLuint preFilteringCubeMapShader = -1;
	GLuint preComputingBRDFShader = -1;
	GLuint LineShader = -1;
	GLuint PointInstanceShader = -1;
	GLuint BaseColorRenderShader = -1;
	GLuint PostProcessShader = -1;
	GLuint BloomDownSampleShader = -1;
	GLuint BloomUpSampleShader = -1;

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


extern CRenderer* g_Renderer;
