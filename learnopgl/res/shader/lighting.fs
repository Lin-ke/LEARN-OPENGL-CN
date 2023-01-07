#version 330 core
out vec4 FragColor;
in vec3 Normal; // 顶点法向量
in vec3 FragPos;	//顶点在世界空间的坐标

uniform vec3 lightPos; // 光源的位置向量
uniform vec3 objectColor; // 物体颜色
uniform vec3 lightColor; // 光的颜色
uniform vec3 viewPos; // 观察者位置

void main()
{

	// 环境光
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
	// 漫反射
	vec3 norm = normalize(Normal); 
	vec3 lightDir = normalize(lightPos - FragPos); // 指向光源的向量
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	// 镜面反射
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
	vec3 specular = specularStrength * spec * lightColor;
    vec3 result = (ambient + diffuse+specular)*objectColor;
    FragColor = vec4(result, 1.0);
}
