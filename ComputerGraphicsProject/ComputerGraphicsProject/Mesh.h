#pragma once
#include "stdafx.h"

#define BOX_SIZE 1.0f



//GLfloat textureST[][2] = {
//	{0.0, 0.0},
//	{1.0, 0.0},
//	{0.0, 1.0},
//	{1.0, 1.0},
//	{0.5, 1.0}
//};
//
//unsigned int textureindex[] = {
//	// 육면체
//	2, 0, 1,
//	2, 1, 3
//};
//
//float texturevertices[72];
//




//unsigned int cubeNormalindex[] = {
//	// 육면체
//	0, 0, 0,
//	0, 0, 0,
//	1, 1, 1,
//	1, 1, 1,
//	2, 2, 2,
//	2, 2, 2,
//	3, 3, 3,
//	3, 3, 3,
//	4, 4, 4,
//	4, 4, 4,
//	5, 5, 5,
//	5, 5, 5
//};

class CMesh
{
public:

	

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 tangent;
		glm::vec3 bitangent;
		glm::vec2 texcoord;
	};

	static std::shared_ptr<CMesh> CreateCubeMesh(float dx, float dy, float dz);
	static std::shared_ptr<CMesh> LoadMeshFromFile(std::string fileName);

	virtual void CreateShaderVariables();
	virtual void BindShaderVariables(GLuint s_Program);

	virtual void Render();

private:
	std::string m_strMeshName;

	UINT m_nVertices = 0;
	std::vector<Vertex> m_pVertices;

	UINT m_nIndices = 0;
	std::vector<UINT> m_pnIndices;

	std::vector<UINT> m_pnSubSetIndices;
	std::vector<UINT> m_pnSubSetStartIndices;
	std::vector<std::vector<UINT>> m_ppnSubSetIndices;

	GLuint m_VAO;
	GLuint m_VBO;
	std::vector<GLuint> m_IBOs;

	// 사용할지는 모름
	UINT                            m_nSlot = 0;
	UINT                            m_nSubMeshes = 0;
	UINT                            m_nStride = 0;
	UINT                            m_nOffset = 0;
	UINT                            m_nStartIndex = 0;
	int                             m_nBaseVertex = 0;


};

