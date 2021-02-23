#version 330 core

out vec4 FragColor;
in vec2 Tex;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixValue;

void main()
{
    FragColor = mix(texture(texture1, Tex), texture(texture2, vec2(1.0f - Tex.x, Tex.y)), mixValue);
} 