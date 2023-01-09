#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D      specular;
    float     shininess;
};  // 物体材质,移除了环境光
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
in vec3 Normal; // 顶点法向量
in vec3 FragPos;	//顶点在世界空间的坐标
in vec2 TexCoords; // 光照贴图

uniform Material material;
uniform vec3 viewPos; // 观察者位置
uniform Light light;

void main()
{ 


    // 环境光
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    // 漫反射 
	// 平行光
	//vec3 lightDir = normalize(-light.direction);
	// 点光源
	//vec3 lightDir = normalize(light.position - FragPos);
	// 聚光，点光源+考虑照射角度
	vec3 lightDir = normalize(light.position - FragPos);
	float theta     = dot(lightDir, normalize(-light.direction));
	float epsilon   = light.cutOff - light.outerCutOff;
	// 使用intensity以避免ifelse
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);     
	  // 执行光照计算
	vec3 norm = normalize(Normal);
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb; 
		// 镜面光
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  
	// 衰减
	float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    ambient  *= attenuation;  
    diffuse   *= attenuation;
    specular *= attenuation;   
        
    vec3 result = ambient + intensity*(diffuse + specular);
    FragColor = vec4(result, 1.0);


}
