#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;


struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
};
uniform Material material;

struct DirLight
{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;

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
uniform int nrPointLights;
uniform PointLight pointLights[16];

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float innerCutoff;
	float outerCutoff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform SpotLight spotLight;


uniform bool useTexture;
uniform vec3 colorTint;
uniform float shininess;

vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 albedo, vec3 specularColor)
{
	vec3 lightDir = normalize(-light.direction);

	//diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	//specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f);

	//combine results
	vec3 ambient = light.ambient * albedo;
	vec3 diffuse = light.diffuse * diff * albedo;
	vec3 specular = light.specular * spec * specularColor;
	return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 albedo, vec3 specularColor)
{
	vec3 lightDir = normalize(light.position - fragPos);

	//diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	//specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f);

	//attenuation
	float dist = length(light.position - fragPos);
	float attenuation = 1.0 / 
	(light.constant + light.linear * dist + light.quadratic * (dist*dist));

	//combine results
	vec3 ambient = light.ambient * albedo;
	vec3 diffuse = light.diffuse * diff * albedo;
	vec3 specular = light.specular * spec * specularColor;
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 albedo, vec3 specularColor)
{
	vec3 lightDir = normalize(light.position - fragPos);

	float epsilon = light.innerCutoff - light.outerCutoff;
	float theta = max(dot(lightDir, normalize(-light.direction)), 0.0);
	float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

	//diffuse
	float diff = max(dot(normal, lightDir) , 0.0);

	//specular 
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

	//combine results
	vec3 ambient = light.ambient * albedo;
	vec3 diffuse = light.diffuse * diff * albedo;
	vec3 specular = light.specular * spec * specularColor;
	
	diffuse *= intensity;
	specular *= intensity;

	return vec3(ambient + diffuse + specular);
}

void main()
{    
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec4 albedo = useTexture ? texture(material.texture_diffuse1, TexCoords) : vec4(colorTint, 1.0f);
	vec3 specularColor = useTexture ? texture(material.texture_specular1, TexCoords).rgb : vec3(shininess);
    
	vec3 result = CalculateDirLight(dirLight, normal, viewDir, albedo.rgb, specularColor);

	for (int i = 0; i < nrPointLights; i++)
		result += CalculatePointLight(pointLights[i], normal, FragPos, viewDir, albedo.rgb, specularColor);

	result += CalculateSpotLight(spotLight, normal, FragPos, viewDir, albedo.rgb, specularColor);

	FragColor = vec4(result, albedo.a);
}