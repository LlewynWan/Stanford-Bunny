#version 330 core
in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 viewPos;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

struct PointLight
{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform PointLight pointLight;

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform DirLight dirLight;

out vec4 FragColor;

vec3 CalcPointLight(PointLight light, vec3 Normal, vec3 FragPos, vec3 viewPos)
{
	vec3 ambient = light.ambient * material.ambient;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(FragPos - light.position);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse); 

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = normalize(reflect(-lightDir, norm));
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	return (ambient + diffuse + specular) * objectColor * attenuation;
}

vec3 CalcDirLight(DirLight light, vec3 Normal, vec3 viewPos)
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 reflectDir = normalize(reflect(-lightDir, norm));
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 ambient = light.ambient * material.ambient;
	vec3 diffuse = light.diffuse * diff * material.diffuse;
	vec3 specular = light.specular * spec * material.specular;

	return (ambient + diffuse + specular) * objectColor;
}

void main()
{
	vec3 result = CalcDirLight(dirLight, Normal, viewPos) + CalcPointLight(pointLight, Normal, FragPos, viewPos);
	FragColor = vec4(result, 1.0);
}
