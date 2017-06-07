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

	vec3 tex_color = texture(tex, fs_in.texcoord).rgb;
	const vec3 grayscale = vec3(0.212, 0.715, 0.072);
	float Y = dot(tex_color, grayscale);
	color = vec4(vec3(Y), 1.0);
}