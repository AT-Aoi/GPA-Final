#version 410 core

uniform sampler2D tex;
out vec4 color;

float laplacian[9] = float[](
	-1, -1, -1,
	-1,  8, -1,
	-1, -1, -1
);

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

	vec3 sumColor;
	for (int y = -1; y <= 1; ++y) {
		for (int x = -1; x <= 1; ++x) {
			sumColor += texelFetch(tex, ivec2(gl_FragCoord.xy) + ivec2(x, y), 0).rgb * laplacian[(x + 1) + (y + 1) * 3];
		}
	}
	color = vec4(sumColor, 0);
}