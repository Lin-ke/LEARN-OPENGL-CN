#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

float near = 0.1; 
float far  = 10.0; 
uniform sampler2D texture1;
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}
void main()
{    
    //FragColor = texture(texture1, TexCoords);
	// ���ӻ��������
    // FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
	// ����Ⱥ���������Թ�ϵ,����near = 0
	float depth = LinearizeDepth(gl_FragCoord.z) / far; // Ϊ����ʾ���� far
    FragColor = vec4(vec3(depth), 1.0);
}