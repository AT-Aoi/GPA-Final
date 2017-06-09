#version 410

layout(location = 0) out vec4 fragColor;

uniform mat4 um4mv;
uniform mat4 um4p;
uniform vec2 test;

in VertexData
{
	vec3 N; // eye space normal
	vec3 L; // eye space light vector
	vec3 H; // eye space halfway vector
	vec2 texcoord;
} vertexData;

uniform sampler2D tex;

vec3 Ks = vec3(1, 1, 1);
vec3 Ia = vec3(0.3, 0.3, 0.3);
vec3 Id = vec3(0.5, 0.5, 0.5);
vec3 Is = vec3(1, 1, 1) * 0.515;
float shiness = 4.43;

void testFunc() {
	if (test.x == 0 && test.y == 0) return;

	//shiness = test.x * 10;
	//Is = vec3(1, 1, 1) * test.y;
}

void main()
{
	testFunc();

	vec3 N = normalize(vertexData.N);
	vec3 L = normalize(vertexData.L);
	vec3 H = normalize(vertexData.H);

	float theta = max(dot(N, L), 0);
	float phi = max(dot(N, H), 0);
	vec3 texColor = texture(tex, vertexData.texcoord).rgb;

	vec3 ambient = texColor * Ia;
	vec3 diffuse = texColor * Id * theta;
	vec3 specular = Ks * Is * pow(phi, shiness);
	fragColor = vec4(ambient + diffuse + specular, 1.0);
}