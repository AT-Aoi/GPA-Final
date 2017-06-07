#version 410 core

uniform sampler2D tex;
uniform int offset;
out vec4 color;

in VS_OUT
{
	vec2 texcoord;
} fs_in;

const float PI = acos(-1);
float power1 = 0.1;
float power2 = 2.4532;

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

	float x = fs_in.texcoord.x, y = fs_in.texcoord.y;
	x += power1 * sin(y * power2 * PI + offset / 20.0);
	color = texture(tex, vec2(x, y));
}