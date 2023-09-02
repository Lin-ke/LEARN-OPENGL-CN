#include "model.h"
void Model::Draw(Shader shader) {
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}
void Model::loadModel(std::string path) {
	Assimp::Importer importer;

	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}
void Model::processNode(aiNode *node, const aiScene *scene) {
	// 处理节点所有的网格（如果有的话）
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// 接下来对它的子节点重复这一过程
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene); // mChildren : aiNode**
	}

}
Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
		std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;
		// 处理顶点位置、法线和纹理坐标
		// 位置
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		// 法线
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
		// 纹理坐标
		if (mesh->mTextureCoords[0]) // 网格是否包含纹理坐标？
		{
			glm::vec2 vec;
			// 顶点最多可包含8个不同的纹理坐标。 因此，我们假设我们不会使用顶点可以具有多个纹理坐标的模型，因此我们总是采用第一个集合（0）。
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		if (mesh->mTangents) {
			// u向量
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;
		}
		if (mesh->mBitangents) {
			// v向量
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
			
		}
		vertices.push_back(vertex);
	}
	//现在遍历每个网格面（一个面是一个三角形的网格）并检索相应的顶点索引。
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// 检索面的所有索引并将它们存储在索引向量中
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// 加工材料
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	// 我们假设着色器中的采样器名称约定。 每个漫反射纹理应命名为'texture_diffuseN'，其中N是从1到MAX_SAMPLER_NUMBER的序列号。
	//同样适用于其他纹理，如下列总结：
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN

	// 1. 漫反射贴图
	std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. 高光贴图
	std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3.法线贴图
	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. 高度贴图
	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	// 返回从提取的网格数据创建的网格对象

	return Mesh(vertices, indices, textures);
}
std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	// 每种类型的纹理
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		
		mat->GetTexture(type, i, &str);
		// 检查之前是否加载了纹理，如果是，则继续下一次迭代：跳过加载新纹理
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data, str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;// 已加载具有相同文件路径的纹理，继续下一个（优化）。
			}
		}
		if (!skip)
		{   // 如果尚未加载纹理，请加载它
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);  //将其存储为整个模型加载的纹理，以确保我们不会加载重复纹理。
		}
	}
	return textures;
}

unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;
		// format选择srgb进行gamma矫正。
		//比如diffuse纹理，这种为物体上色的纹理几乎都是在sRGB空间中的,不用矫正。
		//而为了获取光照参数的纹理，像specular贴图和法线贴图几乎都在线性空间中，需要矫正。
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "纹理无法从此路径加载: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}
