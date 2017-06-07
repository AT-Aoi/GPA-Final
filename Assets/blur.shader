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

	int half_size = 2;
	vec4 color_sum = vec4(0);
	for (int i = -half_size; i <= half_size ; ++i)
	{
		for (int j = -half_size; j <= half_size ; ++j)
		{
			ivec2 coord = ivec2(gl_FragCoord.xy) + ivec2(i, j);
			color_sum += texelFetch(tex, coord, 0);
		}
	}
	int sample_count = (half_size* 2 + 1) * (half_size* 2 + 1);
	color = color_sum / sample_count;
}