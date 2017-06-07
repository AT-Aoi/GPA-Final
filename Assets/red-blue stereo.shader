#version 410 core

uniform sampler2D tex;
out vec4 color;

in VS_OUT
{
	vec2 texcoord;
} fs_in;

uniform float comparisonBarX;
const float comparisonBarThickness = 0.005;
bool comparisonBar() {
	if (fs_in.texcoord.x > comparisonBarX + comparisonBarThickness) {
		color = texelFetch(tex, ivec2(gl_FragCoord.xy), 0);
		return true;
	}
	else if (fs_in.texcoord.x > comparisonBarX - comparisonBarThickness) {
		if (0.4 < fs_in.texcoord.y && fs_in.texcoord.y < 0.6) {
			color = vec4(0, 0.5, 0.2, 1);
		}
		else {
			color = vec4(0, 0, 0, 1);
		}
		return true;
	}
	return false;
}

void main(void)
{
	if (comparisonBar()) return;

    vec2 offset = vec2(0.005, 0);
    vec4 texture_color_Left = texture(tex, fs_in.texcoord - offset);
    vec4 texture_color_Right = texture(tex, fs_in.texcoord + offset);
    float colorR = texture_color_Left.r * 0.299 + texture_color_Left.g * 0.587 + texture_color_Left.b * 0.114;
    float colorG = texture_color_Right.g;
    float colorB = texture_color_Right.b;
	vec4 texture_color = vec4(colorR, colorG, colorB, 1);
    color = texture_color;
}