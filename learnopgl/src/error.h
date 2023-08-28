
#define Assert(x) if((!x)) __debugbreak();
#define GLCall(x) {GLClearError();\
    x;\
    Assert(GLLogCall(#x, __FILE__, __LINE__))} 

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
