#version 410 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texcoord;

out VS_OUT
{
	vec2 texcoord;
} fs_in;

void main(void)
{
	gl_Position = vec4(position, 1.0, 1.0);
	fs_in.texcoord = texcoord;
}