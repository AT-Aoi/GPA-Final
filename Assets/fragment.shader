#version 410

layout(location = 0) out vec4 fragColor;

uniform sampler2D tex;
uniform mat4 um4mv;
uniform mat4 um4p;
uniform vec2 test;

///====================Lighting====================

in VertexData
{
	vec3 N; // eye space normal
vec3 L; // eye space light vector
vec3 H; // eye space halfway vector
vec2 texcoord;
} vertexData;

vec3 Ks = vec3(1, 1, 1);
vec3 Ia = vec3(0.3, 0.3, 0.3);
vec3 Id = vec3(0.5, 0.5, 0.5);
vec3 Is = vec3(1, 1, 1) * 0.515;
float shiness = 4.43;

vec3 lighting(vec3 color) {
	vec3 N = normalize(vertexData.N);
	vec3 L = normalize(vertexData.L);
	vec3 H = normalize(vertexData.H);

	float theta = max(dot(N, L), 0);
	float phi = max(dot(N, H), 0);

	vec3 ambient = color * Ia;
	vec3 diffuse = color * Id * theta;
	vec3 specular = Ks * Is * pow(phi, shiness);

	return ambient + diffuse + specular;
}

///====================Lighting====================

///====================Fog====================

in vec4 viewCoord;

const vec3 fogColor = vec3(0.5, 0.5, 0.5);
const float fogDensity = 0.005;

vec3 makeFog(vec3 color) {
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

	//shiness = test.x * 10;
	//Is = vec3(1, 1, 1) * test.y;
}

void main()
{
	testFunc();

	if (state == 0) {
		vec3 color = texture(tex, vertexData.texcoord).rgb;
		if (color == vec3(0)) discard;
		color = lighting(color);
		color = makeFog(color);

		fragColor = vec4(color, 1.0);
	}
	else if (state == 1) {
		fragColor = texture(skybox, _vertex);
	}
}