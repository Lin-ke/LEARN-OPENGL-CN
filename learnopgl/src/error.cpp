#include <iostream>
#include <glad/glad.h>
#include "error.h"

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    bool flag = true;
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ") " << function << " " << file << ":" << line << std::endl;
        flag = false;
    }
    return flag;
}