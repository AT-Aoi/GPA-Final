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

	float nbins = 8.0;
	vec3 tex_color = texelFetch(tex, ivec2(gl_FragCoord.xy), 0).rgb;
	tex_color = floor(tex_color * nbins) / nbins;
	color = vec4(tex_color, 1.0);
}