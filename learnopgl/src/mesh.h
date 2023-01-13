#pragma once
#ifndef MESH
#define MESH
#include <vector>
#include "vertex.h"
#include "texture.h"
#include "shader.h"
class Mesh {
public:
	/*  ��������  */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	/*  ����  */
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(Shader shader);
private:
	/*  ��Ⱦ����  */
	unsigned int VAO, VBO, EBO;
	/*  ����  */
	void setupMesh();
};
#endif // !MESH

