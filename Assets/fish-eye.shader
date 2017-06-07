#version 410 core

uniform sampler2D tex;
out vec4 color;

in VS_OUT
{
	vec2 texcoord;
} fs_in;

const float PI = acos(-1);
const int WIDTH = 600;
const int HEIGHT = 600;
const float radius = 1;

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
	float pz = sqrt(radius - x * x - y * y);
	float d = atan(sqrt(x * x + y * y), pz) / PI;
	float phi = atan(y, x);
	float u = d * cos(phi) + 0.5, v = d * sin(phi) + 0.5;
	color = texture(tex, vec2(u, v));
}