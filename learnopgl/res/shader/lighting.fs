#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D      specular;
    float     shininess;
};  // 物体材质,移除了环境光
struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
out vec4 FragColor;
in vec3 Normal; // 顶点法向量
in vec3 FragPos;	//顶点在世界空间的坐标
in vec2 TexCoords; // 光照贴图

uniform Material material;
uniform vec3 viewPos; // 观察者位置
uniform Light light;
// 练习4
uniform sampler2D emission;

void main()
{ 
    // 环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // 漫反射 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // 镜面光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec *vec3(texture(material.specular, TexCoords));  
	
    
	// 练习4
	vec3 glow = 1 * texture(emission,TexCoords).xyz;
	vec3 result = ambient + diffuse + specular + glow;
    FragColor = vec4(result, 1.0);
}
