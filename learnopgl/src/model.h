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
	/*  函数   */
	Model(const char *path)
	{
		loadModel(path);
	}
	void Draw(Shader shader);
private:
	/*  模型数据  */
	//存储到目前为止加载的所有纹理，优化以确保纹理不会被加载多次。
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;

	/*  函数   */
	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
		std::string typeName);
}; 
#endif // !MODEL

