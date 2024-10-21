#version 460 core
in vec3 CurrentPos;

out vec4 FragColor;

void main()
{
	FragColor = vec4(CurrentPos, 1.0);
}