#include "Mesh.h"

std::shared_ptr<CMesh> CMesh::CreateCubeMesh(float fWidth, float fHeight, float fDepth)
{
	std::shared_ptr<CMesh> mesh = std::make_shared<CMesh>();

	mesh->m_nVertices = 36;
	mesh->m_pVertices.resize(mesh->m_nVertices);

	static unsigned int cubeShapeindex[] = {
		// À°¸éÃ¼
		0, 1, 2, // +z
		0, 2, 3,
		7, 6, 5, // -z
		7, 5, 4,
		3, 2, 6, // +x
		3, 6, 7,
		4, 5, 1, // -x
		4, 1, 0,
		4, 0, 3, // +y
		4, 3, 7,
		1, 5, 6, // -y
		1, 6, 2
	};


	static GLfloat cubeNormal[][3] = {
	{0.0, 0.0, 1.0},
	{0.0, 0.0, -1.0},
	{1.0, 0.0, 0.0},
	{-1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, -1.0, 0.0}
	};

	static unsigned int cubeNormalindex[] = {
		// À°¸éÃ¼
		0, 0, 0,
		0, 0, 0,
		1, 1, 1,
		1, 1, 1,
		2, 2, 2,
		2, 2, 2,
		3, 3, 3,
		3, 3, 3,
		4, 4, 4,
		4, 4, 4,
		5, 5, 5,
		5, 5, 5
	};


	std::vector<glm::vec3> Vertices; Vertices.resize(8);

	float fx = fWidth * 0.5f, fy = fHeight * 0.5f, fz = fDepth * 0.5f;

	
	Vertices[0] = glm::vec3(-fx, fy, fz);
	Vertices[1] = glm::vec3(-fx, -fy, fz);
	Vertices[2] = glm::vec3(fx, -fy, fz);
	Vertices[3] = glm::vec3(fx, fy, fz);
	Vertices[4] = glm::vec3(-fx, fy, -fz);
	Vertices[5] = glm::vec3(-fx, -fy, -fz);
	Vertices[6] = glm::vec3(fx, -fy, -fz);
	Vertices[7] = glm::vec3(fx, fy, -fz);

	for (int i = 0; i < mesh->m_nVertices; ++i) {
		mesh->m_pVertices[i].position = Vertices[cubeShapeindex[i]];
		mesh->m_pVertices[i].normal = glm::vec3(cubeNormal[cubeNormalindex[i]][0], cubeNormal[cubeNormalindex[i]][1], cubeNormal[cubeNormalindex[i]][2]);
	}

	return mesh;
}

std::shared_ptr<CMesh> CMesh::CreateCubeMeshForIndex(float fWidth, float fHeight, float fDepth)
{
	static unsigned int cubeShapeindex[] = {
		// À°¸éÃ¼
		0, 1, 2,
		0, 2, 3,
		7, 6, 5,
		7, 5, 4,
		3, 2, 6,
		3, 6, 7,
		4, 5, 1,
		4, 1, 0,
		4, 0, 3,
		4, 3, 7,
		1, 5, 6,
		1, 6, 2
	};

	static unsigned int cubeShapeindex2[] = {
		// À°¸éÃ¼
		0, 2, 1,
		0, 3, 2,
		7, 5, 6,
		7, 4, 5,
		3, 6, 2,
		3, 7, 6,
		4, 1, 5,
		4, 0, 1,
		4, 3, 0,
		4, 7, 3,
		1, 6, 5,
		1, 1, 6
	};

	//static GLuint indices[] = {
	//	// Front face
	//	0, 1, 2, 2, 3, 0,

	//	// Back face
	//	4, 5, 6, 6, 7, 4,

	//	// Top face
	//	0, 3, 7, 7, 4, 0,

	//	// Bottom face
	//	1, 2, 6, 6, 5, 1,

	//	// Left face
	//	0, 1, 5, 5, 4, 0,

	//	// Right face
	//	2, 3, 7, 7, 6, 2
	//};

	static GLuint indices[] = {
	0, 1, 2, 3, 7, 6, 5, 4, 0, 1
	};

	static GLfloat cubeNormal[][3] = {
	{0.0, 0.0, 1.0},
	{0.0, 0.0, -1.0},
	{1.0, 0.0, 0.0},
	{-1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, -1.0, 0.0}
	};

	std::shared_ptr<CMesh> mesh = std::make_shared<CMesh>();

	mesh->m_nVertices = 8;
	mesh->m_pVertices.resize(mesh->m_nVertices);

	float fx = fWidth * 0.5f, fy = fHeight * 0.5f, fz = fDepth * 0.5f;
	mesh->m_pVertices[0].position = glm::vec3(-fx, fy, fz);
	mesh->m_pVertices[1].position = glm::vec3(-fx, -fy, fz);
	mesh->m_pVertices[2].position = glm::vec3(fx, -fy, fz);
	mesh->m_pVertices[3].position = glm::vec3(fx, fy, fz);
	mesh->m_pVertices[4].position = glm::vec3(-fx, fy, -fz);
	mesh->m_pVertices[5].position = glm::vec3(-fx, -fy, -fz);
	mesh->m_pVertices[6].position = glm::vec3(fx, -fy, -fz);
	mesh->m_pVertices[7].position = glm::vec3(fx, fy, -fz);


	mesh->m_pVertices[0].normal = glm::normalize(mesh->m_pVertices[0].position);
	mesh->m_pVertices[1].normal = glm::normalize(mesh->m_pVertices[1].position);
	mesh->m_pVertices[2].normal = glm::normalize(mesh->m_pVertices[2].position);
	mesh->m_pVertices[3].normal = glm::normalize(mesh->m_pVertices[3].position);
	mesh->m_pVertices[4].normal = glm::normalize(mesh->m_pVertices[4].position);
	mesh->m_pVertices[5].normal = glm::normalize(mesh->m_pVertices[5].position);
	mesh->m_pVertices[6].normal = glm::normalize(mesh->m_pVertices[6].position);
	mesh->m_pVertices[7].normal = glm::normalize(mesh->m_pVertices[7].position);
	
	mesh->m_nSubMeshes = 1;
	mesh->m_pnSubSetIndices.resize(mesh->m_nSubMeshes);
	mesh->m_pnSubSetIndices[0] = 36;
	mesh->m_ppnSubSetIndices.resize(mesh->m_nSubMeshes);
	mesh->m_ppnSubSetIndices[0].resize(mesh->m_pnSubSetIndices[0]);

	for (int i = 0; i < mesh->m_pnSubSetIndices[0]; ++i) {
		mesh->m_ppnSubSetIndices[0][i] = cubeShapeindex[i];
	}


	return mesh;
}

std::shared_ptr<CMesh> CMesh::LoadMeshFromFile(std::string fileName)
{
	FILE* objFile;

	fopen_s(&objFile, fileName.c_str(), "rb");

	if (objFile == NULL) {
		printf("Impossible to open the file !\n");
		return nullptr;
	}

	std::shared_ptr<CMesh> mesh = std::make_shared<CMesh>();

	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf_s(objFile, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf_s(objFile, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);


		}
	}


}

void CMesh::CreateShaderVariables()
{
	//glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	//glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(CMesh::Vertex) * m_pVertices.size(), m_pVertices.data(), GL_STATIC_DRAW);

	m_IBOs.resize(m_nSubMeshes);
	for (int i = 0; i < m_nSubMeshes; ++i) {
		glGenBuffers(1, &m_IBOs[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOs[i]);
		long long test = m_pnSubSetIndices[i] * sizeof(UINT);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_pnSubSetIndices[i] * sizeof(UINT), m_ppnSubSetIndices[i].data(), GL_STATIC_DRAW);
	}
}

void CMesh::BindShaderVariables(GLuint s_Program)
{
	GLuint posLoc = glGetAttribLocation(s_Program, "v_Position");
	glEnableVertexAttribArray(posLoc);
	GLuint normalLoc = glGetAttribLocation(s_Program, "v_Normal");
	glEnableVertexAttribArray(normalLoc);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex), 0);
	glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex),
		(GLvoid*)(sizeof(float) * 3));
}

void CMesh::Render()
{
	if (m_nSubMeshes > 0) {
		for (int i = 0; i < m_nSubMeshes; ++i) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOs[i]);
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, m_ppnSubSetIndices[i].data());
			glDrawElements(GL_TRIANGLES, m_pnSubSetIndices[i] * sizeof(UINT), GL_UNSIGNED_INT, NULL);
		}
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, m_nVertices);
	}
}
