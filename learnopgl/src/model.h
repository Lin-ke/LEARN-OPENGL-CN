#pragma once
#ifndef MODEL
#define MODEL 
#include "shader.h"
#include "mesh.h"
#include "stb_image.h"
#define STB_IMAGE_IMPLEMENTATION  
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);
class Model
{
public:
	/*  ����   */
	Model(const char *path)
	{
		loadModel(path);
	}
	void Draw(Shader shader);
private:
	/*  ģ������  */
	//�洢��ĿǰΪֹ���ص����������Ż���ȷ�������ᱻ���ض�Ρ�
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;

	/*  ����   */
	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
		std::string typeName);
}; 
#endif // !MODEL

