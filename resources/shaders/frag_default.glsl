#version 330 core

out vec4 FragColor;

in vec2 Tex;
in vec3 Col;

uniform sampler2D texture1;
uniform vec4 color;

void main()
{
    FragColor = (texture(texture1, Tex) * color);
} 