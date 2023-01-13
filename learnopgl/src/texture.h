#pragma once
#ifndef TEXTURE
#define TEXTURE
#include <string>
#include <assimp/types.h>
struct Texture {
	unsigned int id;
	std::string type;
	aiString  path;
};

#endif // !TEXTURE



