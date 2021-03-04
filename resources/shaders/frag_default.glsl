#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D diffuse;
uniform sampler2D specular;
uniform sampler2D roughness;
void main()
{   
    
    vec3 diffuseResult = texture(diffuse, TexCoords).rgb;
    vec3 specularResult = texture(specular, TexCoords).rgb;
    
    vec3 result = diffuseResult + specularResult;
    FragColor = vec4(result, 1.0f);
}