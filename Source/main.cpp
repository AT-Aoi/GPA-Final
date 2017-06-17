#define _USE_MATH_DEFINES
#include "../Externals/Include/Include.h"

#define MENU_NO_FILTER 1
#define MENU_IMAGE_ABSTRACTION 2
#define MENU_LAPLACIAN 3
#define MENU_SHRPEN 4
#define MENU_PIXELATION 5
#define MENU_FISH_EYE 6
#define MENU_SINE_WAVE 7
#define MENU_RED_BLUE 8
#define MENU_BLOOM 9
#define MENU_MAGNIFIER 10
#define CAMERA_TYPE2 true

#include <string>
#include <set>

GLubyte timer_cnt = 0;
bool timer_enabled = true;
unsigned int timer_speed = 16;

using namespace glm;
using namespace std;

#define DEFAULT_SHADER "fragment2.shader"
//#define SCENE_PATH "./lost-empire/"
//#define SCENE_FILE "lost_empire.obj"
#define SCENE_PATH "./tmp3/"
#define SCENE_FILE "tmp3.obj"
char** loadShaderSource(const char* file)
{
    FILE* fp = fopen(file, "rb");
    fseek(fp, 0, SEEK_END);
    long sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *src = new char[sz + 1];
    fread(src, sizeof(char), sz, fp);
    src[sz] = '\0';
    char **srcp = new char*[1];
    srcp[0] = src;
    return srcp;
}

void freeShaderSource(char** srcp)
{
    delete[] srcp[0];
    delete[] srcp;
}

// define a simple data structure for storing texture image raw data
typedef struct _TextureData
{
    _TextureData(void) :
        width(0),
        height(0),
        data(0)
    {
    }

    int width;
    int height;
    unsigned char* data;
} TextureData;

// load a png image and return a TextureData structure with raw data
// not limited to png format. works with any image format that is RGBA-32bit
TextureData loadPNG(const char* const pngFilepath)
{
    TextureData texture;
    int components;

    // load the texture with stb image, force RGBA (4 components required)
    stbi_uc *data = stbi_load(pngFilepath, &texture.width, &texture.height, &components, 4);

    // is the image successfully loaded?
    if (data != NULL)
    {
        // copy the raw data
        size_t dataSize = texture.width * texture.height * 4 * sizeof(unsigned char);
        texture.data = new unsigned char[dataSize];
        memcpy(texture.data, data, dataSize);

        // mirror the image vertically to comply with OpenGL convention
        for (size_t i = 0; i < texture.width; ++i)
        {
            for (size_t j = 0; j < texture.height / 2; ++j)
            {
                for (size_t k = 0; k < 4; ++k)
                {
                    size_t coord1 = (j * texture.width + i) * 4 + k;
                    size_t coord2 = ((texture.height - j - 1) * texture.width + i) * 4 + k;
                    std::swap(texture.data[coord1], texture.data[coord2]);
                }
            }
        }

        // release the loaded image
        stbi_image_free(data);
    }

    return texture;
}

struct Shape
{
	GLuint vao;
	GLuint vbo_position;
	GLuint vbo_normal;
	GLuint vbo_texcoord;
	GLuint ibo;
	int drawCount;
	int materialID;

	vector<float> positions;
	vector<float> normals;
	vector<float> texcoords;
	vector<unsigned> indices;
};

struct Material
{
	GLuint diffuse_tex;
};

const aiScene *scene;
vector<TextureData> textureDatas;
vector<Material> materials;
vector<Shape> shapes;
void loadMaterials() {
	set<string> loadedTextureData;
	printf("Material count: %u\n", scene->mNumMaterials);
	for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
	{
		aiMaterial *material = scene->mMaterials[i];
		Material myMaterial;
		aiString texturePath;
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) ==
			aiReturn_SUCCESS)
		{
			string fullPath = string(SCENE_PATH) + texturePath.C_Str();
			//if (loadedTextureData.find(fullPath) != loadedTextureData.end()) continue;
			printf("Load texture data: %s%s\n", SCENE_PATH, texturePath.C_Str());

			TextureData textureData = loadPNG(fullPath.c_str());
			textureDatas.push_back(textureData);

			loadedTextureData.insert(fullPath);

			// load width, height and data from texturePath.C_Str();
			glGenTextures(1, &myMaterial.diffuse_tex);
			glBindTexture(GL_TEXTURE_2D, myMaterial.diffuse_tex);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureData.width, textureData.height, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, textureData.data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			// load some default image as default_diffuse_tex
			//material.diffuse_tex = default_diffuse_tex;
		}
		// save material…
		materials.push_back(myMaterial);
	}
}


string groundName = "g Mountains";
vector<aiMesh*> grounds;
void loadMeshes() {
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh *mesh = scene->mMeshes[i];
		Shape shape;
		if (strstr(mesh->mName.C_Str(), groundName.c_str())) grounds.push_back(mesh);
		printf("mesh[%d].name: %s\n", i, mesh->mName.C_Str());
		glGenVertexArrays(1, &shape.vao);
		glBindVertexArray(shape.vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		// create 3 vbos to hold data
		glGenBuffers(1, &shape.vbo_position);
		glGenBuffers(1, &shape.vbo_normal);
		glGenBuffers(1, &shape.vbo_texcoord);
		for (unsigned int v = 0; v < mesh->mNumVertices; ++v)
		{
			// mesh->mVertices[v][0~2] => position
			shape.positions.push_back(mesh->mVertices[v][0]);
			shape.positions.push_back(mesh->mVertices[v][1]);
			shape.positions.push_back(mesh->mVertices[v][2]);

			// mesh->mNormals[v][0~2] => normal
			shape.normals.push_back(mesh->mNormals[v][0]);
			shape.normals.push_back(mesh->mNormals[v][1]);
			shape.normals.push_back(mesh->mNormals[v][2]);

			// mesh->mTextureCoords[0][v][0~1] => texcoord
			shape.texcoords.push_back(mesh->mTextureCoords[0][v][0]);
			shape.texcoords.push_back(mesh->mTextureCoords[0][v][1]);
		}
		// create 1 ibo to hold data
		glGenBuffers(1, &shape.ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shape.ibo);
		for (unsigned int f = 0; f < mesh->mNumFaces; ++f)
		{
			// mesh->mFaces[f].mIndices[0~2] => index
			//printf("mesh->mFaces[%u].mIndices[0] => %u\n", f, mesh->mFaces[f].mIndices[0]);
			//printf("mesh->mFaces[%u].mIndices[1] => %u\n", f, mesh->mFaces[f].mIndices[1]);
			//printf("mesh->mFaces[%u].mIndices[2] => %u\n", f, mesh->mFaces[f].mIndices[2]);
			shape.indices.push_back(mesh->mFaces[f].mIndices[0]);
			shape.indices.push_back(mesh->mFaces[f].mIndices[1]);
			shape.indices.push_back(mesh->mFaces[f].mIndices[2]);
		}
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indices.size() * sizeof(unsigned), shape.indices.data(), GL_STATIC_DRAW);
		// glVertexAttribPointer/ glEnableVertexArraycalls…
		glBindBuffer(GL_ARRAY_BUFFER, shape.vbo_position);
		glBufferData(GL_ARRAY_BUFFER, shape.positions.size() * sizeof(float), shape.positions.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, shape.vbo_texcoord);
		glBufferData(GL_ARRAY_BUFFER, shape.texcoords.size() * sizeof(float), shape.texcoords.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, shape.vbo_normal);
		glBufferData(GL_ARRAY_BUFFER, shape.normals.size() * sizeof(float), shape.normals.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		printf("Mesh[%u] -> Material[%u]\n", i, mesh->mMaterialIndex);
		shape.materialID = mesh->mMaterialIndex;
		shape.drawCount = mesh->mNumFaces * 3;
		// save shape…
		shapes.push_back(shape);
	}
}

GLuint currentProgram;

GLuint program;
GLuint mv_location;
GLuint proj_location;
mat4 model;
mat4 view;
mat4 projection;

GLuint program2;
GLuint um4test;
int time;
GLuint um4time;
GLuint window_vao;
GLuint window_buffer;
GLuint FBO;
GLuint depthRBO;
GLuint FBODataTexture;

static const GLfloat window_positions[] =
{
	1.0f,-1.0f,1.0f,0.0f,
	-1.0f,-1.0f,0.0f,0.0f,
	-1.0f,1.0f,0.0f,1.0f,
	1.0f,1.0f,1.0f,1.0f
};

/// wrapper，用来记录当前 program
void useProgram(GLuint program) {
	glUseProgram(program);
	currentProgram = program;
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	useProgram(program);
	glUniformMatrix4fv(mv_location, 1, GL_FALSE, (const GLfloat*)&(view * model));
	glUniformMatrix4fv(proj_location, 1, GL_FALSE, (const GLfloat*)&projection);
	glActiveTexture(GL_TEXTURE0);
	//glUniform1i(tex_location, 0);
	for (int i = 0; i < shapes.size(); ++i)
	{
		glBindVertexArray(shapes[i].vao);
		int materialID = shapes[i].materialID;
		glBindTexture(GL_TEXTURE_2D, materials[materialID].diffuse_tex);
		glDrawElements(GL_TRIANGLES, shapes[i].drawCount, GL_UNSIGNED_INT, 0);
	}

}

GLuint createProgram(char *fragmentShaderSourcePath) {
	// Create Shader Program
	GLuint newProgram = glCreateProgram();

	// Create customize shader by tell openGL specify shader type 
	GLuint vertexShader2 = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);

	// Load shader file
	char** vertexShaderSource2 = loadShaderSource("vertex2.shader");
	char** fragmentShaderSource2 = loadShaderSource(fragmentShaderSourcePath);

	// Assign content of these shader files to those shaders we created before
	glShaderSource(vertexShader2, 1, vertexShaderSource2, NULL);
	glShaderSource(fragmentShader2, 1, fragmentShaderSource2, NULL);

	// Free the shader file string(won't be used any more)
	freeShaderSource(vertexShaderSource2);
	freeShaderSource(fragmentShaderSource2);

	// Compile these shaders
	glCompileShader(vertexShader2);
	glCompileShader(fragmentShader2);

	glPrintShaderLog(vertexShader2);
	glPrintShaderLog(fragmentShader2);

	// Assign the program we created before with these shaders
	glAttachShader(newProgram, vertexShader2);
	glAttachShader(newProgram, fragmentShader2);
	glLinkProgram(newProgram);

	um4time = glGetUniformLocation(newProgram, "time");

	// 初始这个 program 的 uniform 参数
	//glUseProgram(newProgram);
	//glUniform1f(um4comparisonBarX, comparisonBarX);
	//glUseProgram(program);

	return newProgram;
}

void My_Reshape(int width, int height);
void initShader() {
	// Create Shader Program
	program = glCreateProgram();

	// Create customize shader by tell openGL specify shader type 
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Load shader file
	char** vertexShaderSource = loadShaderSource("vertex.shader");
	char** fragmentShaderSource = loadShaderSource("fragment.shader");

	// Assign content of these shader files to those shaders we created before
	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
	glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

	// Free the shader file string(won't be used any more)
	freeShaderSource(vertexShaderSource);
	freeShaderSource(fragmentShaderSource);

	// Compile these shaders
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	glPrintShaderLog(vertexShader);
	glPrintShaderLog(fragmentShader);

	// Assign the program we created before with these shaders
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	// Tell OpenGL to use this shader program now
	useProgram(program);

	mv_location = glGetUniformLocation(program, "um4mv");
	proj_location = glGetUniformLocation(program, "um4p");
	um4test = glGetUniformLocation(program, "test");

	program2 = createProgram(DEFAULT_SHADER);

	glGenVertexArrays(1, &window_vao);
	glBindVertexArray(window_vao);

	glGenBuffers(1, &window_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, window_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(window_positions), window_positions, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 4, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 4, (const GLvoid*)(sizeof(GL_FLOAT) * 2));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glGenFramebuffers(1, &FBO);

	My_Reshape(600, 600);
}

void My_Init()
{
    glClearColor(0.0f, 0.6f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	initShader();

	scene = aiImportFile(SCENE_PATH SCENE_FILE, aiProcessPreset_TargetRealtime_Fast);
	
	loadMaterials();
	loadMeshes();
}

void My_Display()
{	// TODO :
	// (1) Bind the framebuffer object correctly
	// (2) Draw the buffer with color
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	useProgram(program);

	static const GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	static const GLfloat one = 1.0f;

	// TODO :
	// (1) Clear the color buffer (GL_COLOR) with the color of white
	// (2) Clear the depth buffer (GL_DEPTH) with value one 
	glClearBufferfv(GL_COLOR, 0, white);
	glClearBufferfv(GL_DEPTH, 0, &one);

	render();

	// Re-bind the framebuffer and clear it 
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, FBODataTexture);

	// TODO :
	// (1) Bind the vao we want to render
	// (2) Use the correct shader program
	glBindVertexArray(window_vao);
	useProgram(program2);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	if (um4time != -1) {
		useProgram(program2);
		glUniform1i(um4time, time);
		useProgram(program);
	}

	glutSwapBuffers();
}

//vec3 cameraPos = vec3(0.0f, 0.0f, 0.0f);
//vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
//vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
vec3 cameraPos = vec3(-23.54, 42.39, 2.00);
vec3 cameraTarget = vec3(103.65, 42.09, 139.51);
vec3 cameraUp = vec3(0.06, 1.00, 0.01);
int windowWidth, windowHeight;
void My_Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;

	float viewportAspect = (float)width / (float)height;
	projection = perspective(radians(60.0f), viewportAspect, 0.1f, 1000.0f);

	view = lookAt(cameraPos, cameraTarget, cameraUp);

	// If the windows is reshaped, we need to reset some settings of framebuffer
	glDeleteRenderbuffers(1, &depthRBO);
	glDeleteTextures(1, &FBODataTexture);
	glGenRenderbuffers(1, &depthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);


	// TODO :
	// (1) Generate a texture for FBO
	// (2) Bind it so that we can specify the format of the textrue
	glGenTextures(1, &FBODataTexture);
	glBindTexture(GL_TEXTURE_2D, FBODataTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// TODO :
	// (1) Bind the framebuffer with first parameter "GL_DRAW_FRAMEBUFFER" 
	// (2) Attach a renderbuffer object to a framebuffer object
	// (3) Attach a texture image to a framebuffer object
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBODataTexture, 0);
}

void calculatePlane(tuple<aiVector3D*, aiVector3D*, aiVector3D*> cameraTriangle, float &a, float &b, float &c, float &d) {
	aiVector3D *v1 = get<0>(cameraTriangle);
	aiVector3D *v2 = get<1>(cameraTriangle);
	aiVector3D *v3 = get<2>(cameraTriangle);
	d = 1;
	float detD = determinant(mat3(
		v1->x, v1->y, v1->z,
		v2->x, v2->y, v2->z,
		v3->x, v3->y, v3->z));
	float detDa = -d * determinant(mat3(
		1, v1->y, v1->z,
		1, v2->y, v2->z,
		1, v3->y, v3->z));
	float detDb = -d * determinant(mat3(
		v1->x, 1, v1->z,
		v2->x, 1, v2->z,
		v3->x, 1, v3->z));
	float detDc = -d * determinant(mat3(
		v1->x, v1->y, 1,
		v2->x, v2->y, 1,
		v3->x, v3->y, 1));
	a = detDa / detD;
	b = detDb / detD;
	c = detDc / detD;
}

tuple<aiVector3D*, aiVector3D*, aiVector3D*> cameraTriangle;
// 让镜头下掉至地面上
float cameraHeight = 20;
float fallSpeed = 1;
void fallCamera() {
	float a, b, c, d; // 平面方程式参数
	calculatePlane(cameraTriangle, a, b, c, d);
	float groundY = - (a * cameraPos.x + c * cameraPos.z + d) / b;
//	printf("groundY: %.2f\n", groundY);
	float newY = std::max(cameraPos.y - fallSpeed, groundY + cameraHeight);
	if (cameraPos.y != newY) {
		cameraPos.y = newY;
		lookAt(cameraPos, cameraTarget, cameraUp);
		glutPostRedisplay();
	}
}

tuple<aiVector3D*, aiVector3D*, aiVector3D*> getCameraTriangle();
void My_Timer(int val)
{
	++time;
	cameraTriangle = getCameraTriangle();
	if (get<0>(cameraTriangle)) fallCamera();
	glutPostRedisplay();
	glutTimerFunc(timer_speed, My_Timer, val);
}

// 用做测试，给 program 维护一个 test vec，并在把数字显示在标题
void updateTestUniform(int x, int y) {
	float fx = x / 600.0;
	float fy = y / 600.0;

	char title[30];
	if (um4test != -1) {
		GLuint oldProgram = -1;
		if (currentProgram != program) {
			oldProgram = currentProgram;
			useProgram(program);
		}

		glUniform2f(um4test, fx, fy);
		sprintf(title, "(%.3f, %.3f)", fx, fy);
		glutSetWindowTitle(title);

		if (oldProgram != -1) useProgram(oldProgram);
	}
}

int oldX, oldY;
float speed = 0.5;
const float FPS_CAMERA_VERTICAL_ROTATION_RATE = 0.5;
void traceMouse(int x, int y)
{
//	printf("Mouse is over at (%d, %d)\n", x, y);

	if (x != oldX || y != oldY) {
		updateTestUniform(x, y);

		float dx = x - oldX, dy = y - oldY;
		if (CAMERA_TYPE2) dy *= FPS_CAMERA_VERTICAL_ROTATION_RATE; // 垂直镜头旋转幅度小一点

		vec3 cameraFront = cameraTarget - cameraPos;
		vec3 up = vec3(0, 1, 0);
		mat4 rotation = rotate(mat4(), (float)deg2rad(length(vec2(dx, dy))) * speed,
			normalize(up) * dx + normalize(cross(cameraFront, up)) * dy);

		//if (FPS_CAMERA) { // 限制镜头仰角俯角
		//	vec3 horizontalFront = cameraFront;
		//	horizontalFront.y = 0;

		//	float angle = degrees(acos(dot(normalize(cameraFront), normalize(horizontalFront))));
		//	if (abs(angle) < 20) cameraTarget = mat3(rotation) * cameraTarget;
		//	printf("angle: %.2f\n", angle);

		//} else {
			cameraTarget = mat3(rotation) * cameraTarget;
		//}
		cameraUp = mat3(rotation) * cameraUp;


		view = lookAt(cameraPos, cameraTarget, cameraUp);

		oldX = x;
		oldY = y;

		glutPostRedisplay();

		printf("cameraPos:	(%.2f, %.2f, %.2f)\n", cameraPos.x, cameraPos.y, cameraPos.z);
		printf("cameraFront:	(%.2f, %.2f, %.2f)\n", cameraTarget.x, cameraTarget.y, cameraTarget.z);
		printf("cameraUp:	(%.2f, %.2f, %.2f)\n", cameraUp.x, cameraUp.y, cameraUp.z);
	}
}

void My_Mouse(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN)
	{
		printf("Mouse %d is pressed at (%d, %d)\n", button, x, y);
		oldX = x;
		oldY = y;
	}
	else if(state == GLUT_UP)
	{
		printf("Mouse %d is released at (%d, %d)\n", button, x, y);
	}
}

bool toLeft(const aiVector3D &p, const aiVector3D &q, const vec3 &s) {
	return	p.x * q.z - p.z * q.x
		+	q.x * s.z - q.z * s.x
		+	s.x * p.z - s.z * p.x > 0;
}

bool belowCamera(const aiVector3D &p, const aiVector3D &q, const aiVector3D &s) {
	bool left1 = toLeft(p, q, cameraPos);
	bool left2 = toLeft(q, s, cameraPos);
	bool left3 = toLeft(s, p, cameraPos);
	//printf("toLeft: (%d %d %d)\n", left1, left2, left3);
	return left1 == left2 && left2 == left3;
}

tuple<aiVector3D*, aiVector3D*, aiVector3D*> getCameraTriangle() {
	for (unsigned i = 0; i < grounds.size(); ++i) {
		aiMesh *ground = grounds[i];
		for (unsigned j = 0; j < ground->mNumFaces; ++j) {
			aiFace &face = ground->mFaces[j];
			tuple<aiVector3D*, aiVector3D*, aiVector3D*> t = make_tuple(
				&ground->mVertices[face.mIndices[0]],
				&ground->mVertices[face.mIndices[1]],
				&ground->mVertices[face.mIndices[2]]);
			if (belowCamera(*get<0>(t), *get<1>(t), *get<2>(t))) {
//				printf("camera: (%.2f, %.2f, %.2f) in triangle (%.2f, %.2f, %.2f), (%.2f, %.2f, %.2f), (%.2f, %.2f, %.2f)\n",
					//cameraPos.x, cameraPos.y, cameraPos.z,
					//get<0>(t)->x, get<0>(t)->y, get<0>(t)->z,
					//get<1>(t)->x, get<1>(t)->y, get<1>(t)->z,
					//get<2>(t)->x, get<2>(t)->y, get<2>(t)->z);
				return t;
			}
		}
	}

	return make_tuple(nullptr, nullptr, nullptr);
}

void My_Keyboard(unsigned char key, int x, int y)
{
	printf("Key %c is pressed at (%d, %d)\n", key, x, y);
	vec3 dPos = vec3(0);
	vec3 cameraFront = cameraTarget - cameraPos;
	if (CAMERA_TYPE2) {
		switch (key)
		{
		case 'w':
			dPos = cameraFront;
			break;
		case 's':
			dPos = -cameraFront;
			break;
		case 'a':
			dPos = -cross(cameraFront, cameraUp);
			break;
		case 'd':
			dPos = cross(cameraFront, cameraUp);
			break;
		default:
			break;
		}
		dPos.y = 0;
		dPos = normalize(dPos);
	} else {
		switch (key)
		{
		case 'w':
			dPos = normalize(cameraFront);
			break;
		case 's':
			dPos = -normalize(cameraFront);
			break;
		case 'a':
			dPos = -normalize(cross(cameraFront, cameraUp));
			break;
		case 'd':
			dPos = normalize(cross(cameraFront, cameraUp));
			break;
		case 'z':
			dPos = normalize(cameraUp);
			break;
		case 'x':
			dPos = -normalize(cameraUp);
			break;
		default:
			break;
		}
	}

	if (dPos == vec3(0)) return;

	cameraPos += dPos;
	cameraTarget += dPos;
	//cameraUp += dPos;
	view = lookAt(cameraPos, cameraTarget, cameraUp);
	printf("cameraPos:		(%.2f, %.2f, %.2f)\n", cameraPos.x, cameraPos.y, cameraPos.z);
	printf("cameraFront:	(%.2f, %.2f, %.2f)\n", cameraTarget.x, cameraTarget.y, cameraTarget.z);
	printf("cameraUp:		(%.2f, %.2f, %.2f)\n", cameraUp.x, cameraUp.y, cameraUp.z);
}

void My_SpecialKeys(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_F1:
		printf("F1 is pressed at (%d, %d)\n", x, y);
		break;
	case GLUT_KEY_PAGE_UP:
		printf("Page up is pressed at (%d, %d)\n", x, y);
		break;
	case GLUT_KEY_LEFT:
		printf("Left arrow is pressed at (%d, %d)\n", x, y);
		break;
	default:
		printf("Other special key is pressed at (%d, %d)\n", x, y);
		break;
	}
}

void My_Menu(int id)
{
	switch(id)
	{
	case MENU_NO_FILTER:
		
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
#ifdef __APPLE__
    // Change working directory to source code path
    chdir(__FILEPATH__("/../Assets/"));
#endif
	// Initialize GLUT and GLEW, then create a window.
	////////////////////
	glutInit(&argc, argv);
#ifdef _MSC_VER
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#else
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("X1057117_AS3"); // You cannot use OpenGL functions before this line; The OpenGL context must be created first by glutCreateWindow()!
#ifdef _MSC_VER
	glewInit();
#endif
    glPrintContextInfo();
	My_Init();

	// Create a menu and bind it to mouse right button.
	int menu_main = glutCreateMenu(My_Menu);

	glutSetMenu(menu_main);
	glutAddMenuEntry("No Filter", MENU_NO_FILTER);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// Register GLUT callback functions.
	glutDisplayFunc(My_Display);
	glutReshapeFunc(My_Reshape);
	glutMouseFunc(My_Mouse);
	glutKeyboardFunc(My_Keyboard);
	glutSpecialFunc(My_SpecialKeys);
	glutTimerFunc(timer_speed, My_Timer, 0); 
	glutMotionFunc(traceMouse);
	glutPassiveMotionFunc(updateTestUniform);
	
	// Enter main event loop.
	glutMainLoop();

	return 0;
}
