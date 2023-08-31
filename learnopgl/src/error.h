#pragma once
#ifndef ERROR_H 
#define ERROR_H
#define Assert(x) if((!x)) __debugbreak();
#define GLCall(x) {GLClearError();\
    x;\
    Assert(GLLogCall(#x, __FILE__, __LINE__))} 
void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

#endif
