#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;



void main()
{           
    vec3 norm = fs_in.Normal*0.5+0.5;
    FragColor = vec4(fs_in.Normal , 1.0);
}