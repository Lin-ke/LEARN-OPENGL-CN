

#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
// 这个方块占满屏幕，直接就是屏幕坐标。
out vec2 TexCoords;

void main()
{
    gl_Position = vec4(position, 1.0f);
    TexCoords = texCoords;
}

