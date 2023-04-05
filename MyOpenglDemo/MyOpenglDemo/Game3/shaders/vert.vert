#version 330 core
layout (location = 7) in vec3 aPos;
layout (location = 12) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	TexCoord = vec2(aTexCoord.x, 1 - aTexCoord.y);
}

