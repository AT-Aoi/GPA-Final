#version 410

layout(location = 0) in vec3 iv3vertex;
layout(location = 1) in vec2 iv2tex_coord;
layout(location = 2) in vec3 iv3normal;

uniform mat4 um4mv;
uniform mat4 um4p;
uniform vec2 test;

out VertexData
{
	vec3 N; // eye space normal
	vec3 L; // eye space light vector
	vec3 H; // eye space halfway vector
	vec2 texcoord;
} vertexData;

vec3 light = vec3(-30, 60, 80);

void testFunc() {
	if (test.x == 0 && test.y == 0) {
//		test.x = 0.5;
//		test.y = 0.5;
		return;
	}

	light = vec3(test.x * 50, test.y * 50, 50);
}

void main()
{
	testFunc();

	vec4 P = um4mv * vec4(iv3vertex, 1);
	gl_Position = um4p * um4mv * vec4(iv3vertex, 1.0);
	vertexData.texcoord = iv2tex_coord;

	vec3 V = -P.xyz;
	vertexData.N = mat3(um4mv) * iv3normal;
	vertexData.L = light - P.xyz;
	vertexData.H = vertexData.L + V;
}