#version 330 core 
out vec4 FragColor;

in vec3 vectorColor;
in vec2 vectorTexture;

uniform sampler2D ourTexture;
uniform sampler2D snudderTexture;

uniform float mixValue;

void main()
{
    FragColor = mix(texture(ourTexture, vectorTexture), texture(snudderTexture, vec2(vectorTexture.x, vectorTexture.y)), mixValue);
}