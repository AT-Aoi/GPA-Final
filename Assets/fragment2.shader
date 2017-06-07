#version 410 core

uniform sampler2D tex;

in VS_OUT
{
	vec2 texcoord;
} fs_in;

out vec4 color;

void main(void)
{
	color = texture(tex, fs_in.texcoord);
}