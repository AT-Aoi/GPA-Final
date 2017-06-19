#version 410

layout(location = 0) in vec3 iv3vertex;
layout(location = 1) in vec2 iv2tex_coord;
layout(location = 2) in vec3 iv3normal;

#define LIGHT_NUM 1

uniform mat4 um4mv;
uniform mat4 um4p;
uniform vec2 test;
uniform vec3 lightPos[LIGHT_NUM];

out vec2 texcoord;

///====================Lighting====================

out PointLight
{
	vec3 N; // eye space normal
	vec3 L; // eye space light vector
	vec3 H; // eye space halfway vector
} pointLight[LIGHT_NUM];

out DirectionalLight{
	vec3 N; // eye space normal
	vec3 L; // eye space light vector
	vec3 H; // eye space halfway vector
} directionalLight;

vec3 sun = vec3(-57.48, 200, 200.64);

void directionalLighting() {
	vec4 P = um4mv * vec4(iv3vertex, 1);

	directionalLight.N = mat3(um4mv) * iv3normal;
	directionalLight.L = (um4mv * vec4(sun, 1)).xyz - P.xyz;
	directionalLight.H = directionalLight.L + -P.xyz;
}

void pointLighting(in int i) {
	vec4 P = um4mv * vec4(iv3vertex, 1);

	pointLight[i].N = mat3(um4mv) * iv3normal;
	pointLight[i].L = (um4mv * vec4(lightPos[i], 1)).xyz - P.xyz;
	pointLight[i].H = pointLight[i].L + -P.xyz;
}

///====================Lighting====================

///====================Fog====================

out vec4 viewCoord;

void makeFog() {
	viewCoord = um4mv * vec4(iv3vertex, 1.0);
}

///====================Fog====================

void testFunc() {
	if (test.x == 0 && test.y == 0) return;

	
}

/*skybox*/
out vec3 _vertex;
/*skybox*/

void main()
{
	testFunc();

	texcoord = iv2tex_coord;

	directionalLighting();
	for (int i = 0; i < LIGHT_NUM; ++i) {
		pointLighting(i);
	}
	makeFog();
	gl_Position = um4p * um4mv * vec4(iv3vertex, 1.0);

	/*skybox*/
	_vertex = iv3vertex;
}