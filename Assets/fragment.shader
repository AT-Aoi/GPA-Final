#version 410

layout(location = 0) out vec4 fragColor;

//*改这个点光源个数
#define LIGHT_NUM 3

uniform sampler2D tex;
uniform mat4 um4mv;
uniform mat4 um4p;
uniform vec2 test;

in vec2 texcoord;

///====================Lighting====================

//*平行光（太阳光）的参数，调得有夜晚感觉一点
vec3 directionalKs = vec3(1);
vec3 directionalIa = vec3(0.1);
vec3 directionalId = vec3(0.2);
vec3 directionalIs = vec3(0.515);
float directionalShiness = 4.43;

in DirectionalLight{
	vec3 N; // eye space normal
	vec3 L; // eye space light vector
	vec3 H; // eye space halfway vector
} directionalLight;

vec3 directionalLighting(in vec3 color) {
	vec3 N = normalize(directionalLight.N);
	vec3 L = normalize(directionalLight.L);
	vec3 H = normalize(directionalLight.H);

	float dist = length(directionalLight.L);
	float theta = max(dot(N, L), 0);
	float phi = max(dot(N, H), 0);

	vec3 ambient = color * directionalIa;
	vec3 diffuse = color * directionalId * theta;
	vec3 specular = directionalKs * directionalIs * pow(phi, directionalShiness);

	return ambient + diffuse + specular;
}

//*点光源的参数，调得有灯光感觉一点
vec3 pointKs = vec3(1);
vec3 pointIa = vec3(0.1);
vec3 pointId = vec3(4); // 0.5
vec3 pointIs = vec3(0.112); // * 0.515;
float pointShiness = 0.177 * 5; // 4.43

float c1 = 0.9;
float c2 = 0.225;
float c3 = 0.022;
float c4 = -0.0001;

in PointLight
{
	vec3 N; // eye space normal
	vec3 L; // eye space light vector
	vec3 H; // eye space halfway vector
} pointLight[LIGHT_NUM];

vec3 pointLighting(in vec3 color, in int i) {
	float dist = length(pointLight[i].L);

	if (20 < dist) return vec3(0); // too far, no effect

	vec3 N = normalize(pointLight[i].N);
	vec3 L = normalize(pointLight[i].L);
	vec3 H = normalize(pointLight[i].H);

	float theta = max(dot(N, L), 0);
	float phi = max(dot(N, H), 0);
	float fa = min(1 / (c1 + c2 * dist + c3 * dist * dist), 1);

	vec3 ambient = color * pointIa;
	vec3 diffuse = color * pointId * theta;
	vec3 specular = pointKs * pointIs * pow(phi, pointShiness);

	return ambient + fa * (diffuse + specular);
}

///====================Lighting====================

///====================Fog====================

in vec4 viewCoord;

const vec3 fogColor = vec3(0.5, 0.5, 0.5);
const float fogDensity = 0.005;

vec3 makeFog(in vec3 color) {
	float dist = length(viewCoord);
	float fogFactor = clamp(1.0 / exp(dist * fogDensity), 0.0, 1.0);
	return mix(fogColor, color, fogFactor);
}

///====================Fog====================

/* skybox */
uniform samplerCube skybox;
uniform int state;
in vec3 _vertex;
/* skybox */

void testFunc() {
	if (test.x == 0 && test.y == 0) return;

//	pointIs = vec3(1) * test.x;
//	pointShiness = 5 * test.y;
//	c2 = test.x;
//	c3 = test.y;
//	c4 = -0.0001 * test.x;
}

void main()
{
	testFunc();

	if (state == 0) {
		vec3 textureColor = texture(tex, texcoord).rgb;
		if (textureColor == vec3(0)) discard;

		vec3 color = directionalLighting(textureColor);
		for (int i = 0; i < LIGHT_NUM; ++i) {
			color += pointLighting(textureColor, i);
		}

		color = makeFog(color);

		fragColor = vec4(color, 1.0);
	}
	else if (state == 1) {
		fragColor = texture(skybox, _vertex);
	}
}