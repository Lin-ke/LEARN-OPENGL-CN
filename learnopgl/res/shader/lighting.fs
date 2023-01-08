#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D      specular;
    float     shininess;
};  // �������,�Ƴ��˻�����
struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
out vec4 FragColor;
in vec3 Normal; // ���㷨����
in vec3 FragPos;	//����������ռ������
in vec2 TexCoords; // ������ͼ

uniform Material material;
uniform vec3 viewPos; // �۲���λ��
uniform Light light;
// ��ϰ4
uniform sampler2D emission;

void main()
{ 
    // ������
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // ������ 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // �����
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec *vec3(texture(material.specular, TexCoords));  
	
    
	// ��ϰ4
	vec3 glow = 1 * texture(emission,TexCoords).xyz;
	vec3 result = ambient + diffuse + specular + glow;
    FragColor = vec4(result, 1.0);
}
