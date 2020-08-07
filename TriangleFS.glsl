#version 330 core 
out vec4 FragColor;

in vec3 vectorColor;

void main()
{
    FragColor = vec4(vectorColor, 1.0);
}