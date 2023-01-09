#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D      specular;
    float     shininess;
};  // �������,�Ƴ��˻�����
struct Light {
    vec3 position;  
	vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	float cutOff;
	float outerCutOff;
    float constant;
    float linear;
    float quadratic;
};
out vec4 FragColor;
in vec3 Normal; // ���㷨����
in vec3 FragPos;	//����������ռ������
in vec2 TexCoords; // ������ͼ

uniform Material material;
uniform vec3 viewPos; // �۲���λ��
uniform Light light;

void main()
{ 


    // ������
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    // ������ 
	// ƽ�й�
	//vec3 lightDir = normalize(-light.direction);
	// ���Դ
	//vec3 lightDir = normalize(light.position - FragPos);
	// �۹⣬���Դ+��������Ƕ�
	vec3 lightDir = normalize(light.position - FragPos);
	float theta     = dot(lightDir, normalize(-light.direction));
	float epsilon   = light.cutOff - light.outerCutOff;
	// ʹ��intensity�Ա���ifelse
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);     
	  // ִ�й��ռ���
	vec3 norm = normalize(Normal);
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb; 
		// �����
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  
	// ˥��
	float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    ambient  *= attenuation;  
    diffuse   *= attenuation;
    specular *= attenuation;   
        
    vec3 result = ambient + intensity*(diffuse + specular);
    FragColor = vec4(result, 1.0);


}
