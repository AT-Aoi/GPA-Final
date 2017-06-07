#version 410 core

uniform sampler2D tex;
out vec4 color;

in VS_OUT
{
	vec2 texcoord;
} fs_in;

vec2 img_size = vec2(600, 600);
float sigma_e = 2.0f;
float sigma_r = 2.8f;
float phi = 3.4f;
float tau = 0.99f;
float twoSigmaESquared = 2.0 * sigma_e * sigma_e;
float twoSigmaRSquared = 2.0 * sigma_r * sigma_r;
int halfWidth = int(ceil(2.0 * sigma_r));

vec4 blurColor;
vec4 quantizationColor;
vec4 differenceOfGaussianColor;

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
	for (int i = -half_size; i <= half_size; ++i)
	{
		for (int j = -half_size; j <= half_size; ++j)
		{
			ivec2 coord = ivec2(gl_FragCoord.xy) + ivec2(i, j);
			color_sum += texelFetch(tex, coord, 0);
		}
	}
	int sample_count = (half_size * 2 + 1) * (half_size * 2 + 1);

	blurColor = color_sum / sample_count;


	float nbins = 8.0;
	vec3 tex_color = texelFetch(tex, ivec2(gl_FragCoord.xy), 0).rgb;
	tex_color = floor(tex_color * nbins) / nbins;
	quantizationColor = vec4(tex_color, 1.0);

	vec2 sum = vec2(0.0);
	vec2 norm = vec2(0.0);
	for (int i = -halfWidth; i <= halfWidth; ++i) {
		for (int j = -halfWidth; j <= halfWidth; ++j) {
			float d = length(vec2(i, j));
			vec2 kernel = vec2(exp(-d * d / twoSigmaESquared), exp(-d * d / twoSigmaRSquared));
			vec4 c = texture(tex, fs_in.texcoord + vec2(i, j) / img_size);
			vec2 L = vec2(0.299 * c.r + 0.587 * c.g + 0.114 * c.b);
			norm += 2.0 * kernel;
			sum += kernel * L;
		}
	}
	sum /= norm;
	float H = 100.0 * (sum.x - tau * sum.y);
	float edge = (H > 0.0) ? 1.0 : 2.0 * smoothstep(-2.0, 2.0, phi * H);
	differenceOfGaussianColor = vec4(edge, edge, edge, 1.0);

	color = blurColor + quantizationColor + differenceOfGaussianColor / 3;
}