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

private:
	std::vector<std::shared_ptr<CMaterial>> m_pMaterials;
	std::vector<std::shared_ptr<CMesh>> m_pMeshs;

public:
	UINT RegisterMaterial(std::shared_ptr<CMaterial> material) { m_pMaterials.push_back(material); return m_pMaterials.end() - m_pMaterials.begin(); };
	UINT RegisterMesh(std::shared_ptr<CMesh> mesh) { m_pMeshs.push_back(mesh); return m_pMeshs.end() - m_pMeshs.begin();};
	std::shared_ptr<CMaterial> GetMaterialFromIndex(UINT index) { if (index < m_pMaterials.size()) return m_pMaterials[index]; return nullptr; };
	std::shared_ptr<CMesh> GetMeshFromIndex(UINT index) { if (index < m_pMeshs.size()) return m_pMeshs[index]; return nullptr; };

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
