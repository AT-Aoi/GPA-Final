#version 410 core

uniform sampler2D tex;
uniform ivec2 mouse;
uniform int height;
out vec4 color;

in VS_OUT
{
	vec2 texcoord;
} fs_in;

const float RADIUS = 100;
const float THICKNESS = 5;

const float time = 2;

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

	float dx = mouse.x - gl_FragCoord.x , dy = height - gl_FragCoord.y - mouse.y,
		r2 = dx * dx + dy * dy;
	if (r2 < RADIUS * RADIUS)
		color = texelFetch(tex, ivec2(gl_FragCoord.xy) + ivec2(dx / time + 0.5, dy / time + 0.5), 0);
	else if (r2 < (RADIUS + THICKNESS) * (RADIUS + THICKNESS))
		color = vec4(0, 0, 0, 1);
	else 
		color = texelFetch(tex, ivec2(gl_FragCoord.xy), 0);
}