#include "Mesh.h"

std::shared_ptr<CMesh> CMesh::CreateCubeMesh(float fWidth, float fHeight, float fDepth)
{
	static unsigned int cubeShapeindex[] = {
		// ¿∞∏È√º
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

	//mesh->m_pVertices[0].normal = 
	
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
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CMesh::Vertex) * m_pVertices.size(), m_pVertices.data(), GL_STATIC_DRAW);

	for (int i = 0; i < m_nSubMeshes; ++i) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOs[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_pnSubSetIndices[i], m_ppnSubSetIndices[i].data(), GL_STATIC_DRAW);
	}
}

void CMesh::BindShaderVariables(GLuint s_Program)
{
	GLuint posLoc = glGetAttribLocation(s_Program, "vPosition");
	glEnableVertexAttribArray(posLoc);
	GLuint normalLoc = glGetAttribLocation(s_Program, "vNormal");
	glEnableVertexAttribArray(normalLoc);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex), 0);
	glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex),
		(GLvoid*)(sizeof(glm::vec3)));
}

void CMesh::Render()
{
	if (m_nSubMeshes > 0) {
		for (int i = 0; i < m_nSubMeshes; ++i) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOs[i]);
			glDrawElements(GL_TRIANGLES, m_pnSubSetIndices[i], GL_UNSIGNED_INT, m_ppnSubSetIndices[i].data());
		}
	}
}
