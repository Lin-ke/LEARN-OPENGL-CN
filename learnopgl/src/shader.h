#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <typeinfo>
class Shader
{
public:
	// ����ID
	unsigned int ID;

	// ��������ȡ��������ɫ��
	Shader(const char* vertexPath, const char* fragmentPath);
	// ʹ��/�������
	void use();
	// uniform���ߺ���
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setMat4(const std::string &name, glm::mat4 value) const;
	void setVec3(const std::string &name, glm::vec3 value) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
	template <typename T>
	void set(const std::string &name, T val);
	
};
template <typename T>
void Shader::set(const std::string &name, T val) {
	if (typeid(T) == typeid(int)) {
		setInt(name, *(int*)(&val));
		}
	if (typeid(T) == typeid(glm::vec3))
		setVec3(name, static_cast<glm::vec3>(val));





}
#endif