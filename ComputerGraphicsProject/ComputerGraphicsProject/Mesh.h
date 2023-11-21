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
		glm::vec2 texcoord0;
		glm::vec2 texcoord1;
	};


	static std::shared_ptr<CMesh> CreateCubeMesh(float dx, float dy, float dz);
	static std::shared_ptr<CMesh> CreateCubeMeshForIndex(float dx, float dy, float dz);
	static std::shared_ptr<CMesh> CreateSphereMesh(int n_slices, int n_stacks);
	static std::shared_ptr<CMesh> CreateNDCMesh();
	static std::shared_ptr<CMesh> CreatePointMesh();
	static std::shared_ptr<CMesh> CreateLineMesh();
	static std::shared_ptr<CMesh> CreateTriangleMesh();
	static std::shared_ptr<CMesh> CreateRectMesh();
	static std::shared_ptr<CMesh> LoadMeshFromFile(FILE* pInFile);

	std::vector<Vertex> GetVertexs() { return m_pVertices; };
	std::vector<UINT> GetSubSetIndice(UINT index) { 
		if(m_nSubMeshes > index)
			return m_ppnSubSetIndices[index];
		return std::vector<UINT>();
	};

	void SetVertexs(std::vector<Vertex>& vertexs) {
		if (m_nVertices == 0) {
			m_pVertices = vertexs;
			m_nVertices = m_pVertices.size();
		}
	}

	void SetSubmesh(std::vector<UINT>& subsetIndices);

	virtual void CreateShaderVariables();
	virtual void BindShaderVariables(GLuint s_Program);

	virtual void Render();
	virtual void RenderInstanced(int nInstance);

	void SetName(std::string str) { m_strMeshName = str; };
	std::string GetName() { return m_strMeshName; };

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

	glm::vec3 m_vec3AABBCenter;
	glm::vec3 m_vec3AABBExtents;

	GLenum m_PrimitiveTopology = GL_TRIANGLES;

	// 사용할지는 모름
	UINT                            m_nSlot = 0;
	UINT                            m_nSubMeshes = 0;
	UINT                            m_nStride = 0;
	UINT                            m_nOffset = 0;
	UINT                            m_nStartIndex = 0;
	int                             m_nBaseVertex = 0;


};

