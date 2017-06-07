#version 410 core

uniform sampler2D tex;
out vec4 color;

int grid = 10;

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

	vec4 sumColor;
	int y = int(gl_FragCoord.y) / grid * grid,
		x = int(gl_FragCoord.x) / grid * grid;
	for (int dy = 0; dy <= grid; ++dy) {
		for (int dx = 0; dx <= grid; ++dx) {
			sumColor += texelFetch(tex, ivec2(x + dx, y + dy), 0);
		}
	}
	color = sumColor / (grid * grid);
}