#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 bPos;
layout (location = 2) in vec2 cPos;

out vec3 vectorColor;
out vec2 vectorTexture;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vectorColor = bPos;
	vectorTexture = cPos;
}