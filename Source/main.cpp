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
//#define SCENE_PATH "./Mountains2/"
//#define SCENE_FILE "Mountains2.obj"
#define SCENE_PATH "./tmp3/"
#define SCENE_FILE "tmp3.obj"

//*改这个点光源
vec3 lightPos[] = { vec3(-83.6, 31.0, 211.6), vec3(-81.2, 32.4, 206.8), vec3(-78.8, 33.7, 202.0) };//, vec3(-12.9, 42.0, 6.1), vec3(21.5, 42.0, -59.8), vec3(3.5, 42.0, -80.5), vec3(-29.9, 36.0, -155.8), vec3(-44.8, 32.0, -169.7), vec3(-79.0, 18.2, -214.0), vec3(-84.0, 17.0, -216.4), vec3(19.6, 42.6, 74.1), vec3(19.9, 42.5, 44.2), vec3(21.2, 43.0, 38.7), vec3(22.5, 43.5, 33.5), vec3(81.0, 51.4, 26.1), vec3(75.0, 51.5, 12.6), vec3(77.0, 51.5, 7.5), vec3(83.2, 51.5, -1.7), vec3(191.4, 43.0, -12.3), vec3(-18.9, 42.7, -38.7), vec3(-151.0, 42.5, -48.1), vec3(149.8, 42.5, -53.3), vec3(123.7, 42.5, -91.2), vec3(129.1, 42.7, -92.0), vec3(-49.3, 37.4, 161.3), vec3(-28.5, 38.2, 153.9), vec3(37.8, 42.4, 100.1), vec3(47.5, 43.6, 80.3), vec3(187.7, 42.6, 33.0), vec3(83.7, 51.9, 3.6), vec3(90.1, 51.9, -5.5), vec3(-14.0, 42.5, -32.2), vec3(-17.0, 42.5, -48.3), vec3(152.7, 42.2, -73.9), vec3(265.5, 61.7, -88.0), vec3(40.8, 42.4, -92.8), vec3(17.3, 41.7, -111.4), vec3(-46.6, 32.7, -166.0), vec3(-49.2, 31.5, -170.8), vec3(-51.0, 30.2, -175.6), vec3(-52.7, 28.8, -181.0), vec3(171.2, 42.0, 55.1), vec3(-21.2, 41.8, 49.0), vec3(189.0, 43.9, -11.7), vec3(31.4, 42.0, -69.1), vec3(230.0, 48.0, -79.2), vec3(234.7, 50.7, -80.3), vec3(249.5, 56.9, -83.8), vec3(0.5, 42.4, -89.4), vec3(16.7, 42.4, -104.0), vec3(-62.8, 37.5, 177.8), vec3(-65.6, 37.3, 175.5), vec3(-63.0, 37.6, 170.5), vec3(4.8, 39.7, 138.0), vec3(148.9, 43.9, 87.3), vec3(31.5, 42.7, 86.0), vec3(26.9, 42.6, 82.9), vec3(56.1, 44.1, 79.5), vec3(-6.3, 42.8, 13.7), vec3(-16.3, 42.4, -13.6), vec3(175.0, 41.9, -54.4), vec3(86.6, 42.4, -97.6), vec3(31.5, 42.4, -113.4), vec3(41.5, 42.4, -117.6), vec3(22.8, 41.1, 129.2), vec3(25.2, 41.5, 124.3), vec3(27.7, 41.7, 119.3), vec3(46.1, 44.0, 95.5), vec3(134.8, 42.7, 95.9), vec3(134.0, 43.6, 70.8), vec3(142.7, 44.4, 56.8), vec3(-1.2, 41.9, 49.6), vec3(174.4, 42.6, 28.5), vec3(168.4, 43.0, -3.9), vec3(18.7, 43.0, -43.7), vec3(183.3, 42.1, -53.4), vec3(-17.5, 42.4, -63.7), vec3(64.1, 48.1, -64.2), vec3(233.2, 49.0, -76.3), vec3(-9.0, 42.5, -77.9), vec3(39.7, 42.0, -78.4), vec3(255.2, 58.4, -81.7), vec3(260.4, 59.9, -82.9), vec3(265.5, 61.6, -84.2), vec3(291.4, 68.0, -91.5), vec3(296.3, 69.2, -92.9), vec3(343.0, 75.0, -97.6), vec3(61.6, 46.2, 68.1), vec3(46.9, 45.0, 61.7), vec3(186.1, 42.7, -28.5), vec3(81.7, 51.9, -52.0), vec3(357.0, 74.3, -93.1), vec3(101.0, 42.7, -98.9), vec3(26.5, 41.2, 113.6), vec3(71.2, 41.9, 103.7), vec3(132.3, 43.0, 101.6), vec3(51.9, 45.1, 70.7), vec3(137.9, 49.9, -20.4), vec3(141.8, 47.0, -22.6), vec3(176.5, 42.3, -26.4), vec3(179.9, 42.1, -30.9), vec3(158.6, 42.5, -31.4), vec3(186.4, 42.0, -38.3), vec3(164.1, 42.4, -69.4), vec3(123.2, 42.8, -82.8), vec3(32.2, 41.9, -109.8), vec3(37.3, 42.0, -111.9), vec3(-72.5, 35.0, 197.0), vec3(4.5, 40.3, 142.8), vec3(124.5, 42.2, 120.1), vec3(125.3, 42.0, 109.2), vec3(129.0, 44.0, 69.9), vec3(29.8, 44.0, 52.9), vec3(77.5, 43.7, 47.9), vec3(-21.4, 51.9, 44.4), vec3(-14.6, 41.9, 23.6), vec3(-8.3, 41.9, 17.1), vec3(167.1, 42.6, 16.2), vec3(166.1, 42.2, 10.4), vec3(149.7, 42.1, -28.1), vec3(148.7, 42.2, -34.3), vec3(146.5, 42.4, -44.9), vec3(37.8, 42.5, -88.9), vec3(-34.6, 36.0, -155.1), vec3(127.4, 42.2, 113.8), vec3(76.2, 44.1, 82.0), vec3(71.1, 44.4, 79.5), vec3(19.6, 42.4, 54.8), vec3(0.1, 42.6, 53.1), vec3(5.6, 42.6, 53.1), vec3(11.2, 42.6, 53.0), vec3(94.4, 51.8, 22.1), vec3(185.1, 42.1, 8.8), vec3(114.6, 52.1, -1.9), vec3(128.4, 51.9, -10.8), vec3(190.2, 43.3, -24.4), vec3(-21.1, 41.9, -32.0), vec3(80.9, 51.5, -46.3), vec3(120.5, 42.6, -92.7) };
GLuint lightPos_location;

// Skybox setting 
GLuint cubemapTexture;
GLuint state_location;
GLuint skyboxVAO, skyboxVBO;
vector<const GLchar*> faces;
GLfloat skyboxVertices[] = {
	// Positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f
};
// skybox setting

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

//skybox cube map
GLuint loadCubemap(vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE1);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		TextureData texture = loadPNG(faces[i]);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_RGBA8, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.data
		);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

void setSkybox() {
	faces.push_back("skybox/right.png");
	faces.push_back("skybox/left.png");
	faces.push_back("skybox/up.png");
	faces.push_back("skybox/down.png");
	faces.push_back("skybox/front.png");
	faces.push_back("skybox/back.png");

	cubemapTexture = loadCubemap(faces);

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
}
//skybox

set<string> groundNames = { "g Mountains", "g Mountains001", "g Mountains002", "g Mountains003", "CourtYardTexturing_2", "CourtYardTexturing_3", "SidewalkMainTexturing", "CourtYardTexturing_1", "CourtYardTexturing", "StoneStepTexturing", "Street", "StoneStepTexturing_1", "Green", "Green_2", "Green_1", "VeggieFieldTexturing", "landscape_VeggieFieldTexturing", "landscape_VeggieFieldTexturing_2", "landscape_VeggieFieldTexturing_1", "landscape_Green", "VeggieFieldTexturing_1"};
vector<aiMesh*> grounds;
vector<aiMesh*> obstacles;
void loadMeshes() {
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh *mesh = scene->mMeshes[i];
		Shape shape;

		printf("mesh[%d].name: %s\n", i, mesh->mName.C_Str());
		if (groundNames.find(string(mesh->mName.C_Str())) == groundNames.end()) obstacles.push_back(mesh);
		else grounds.push_back(mesh);

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
		//printf("Mesh[%u] -> Material[%u]\n", i, mesh->mMaterialIndex);
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

	//change state
	glUniform1i(state_location, 0);
	//
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

	/* render skybox */
	glUniform1i(state_location, 1);
	mat4 Identity(1.0);
	mat4 S = scale(Identity, vec3(500.0f, 500.0f, 500.0f));
	glUniformMatrix4fv(mv_location, 1, GL_FALSE, (const GLfloat*)&(view * S));

	glDepthMask(GL_FALSE);

	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE1);
	glUniform1i(glGetUniformLocation(program, "skybox"), 1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDepthMask(GL_TRUE);
	//skybox
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
	//skybox
	state_location = glGetUniformLocation(program, "state");
	setSkybox();
	//skybox
	lightPos_location = glGetUniformLocation(program, "lightPos");
	glUniform3fv(lightPos_location, sizeof(lightPos) / sizeof(vec3), (GLfloat*)lightPos);

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
//cameraPos:      (-204.79, 270.81, 183.95)
//	cameraFront : (0.72, -0.55, -0.42)
//	cameraUp : (0.34, 0.81, -0.49)
vec3 cameraPos = vec3(-57.48, 26.90, 200.64);
vec3 cameraFront = vec3(0.36, -0.10, -0.93);
vec3 cameraUp = vec3(0.07, 0.99, -0.08);
int windowWidth, windowHeight;
void My_Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;

	float viewportAspect = (float)width / (float)height;
	projection = perspective(radians(60.0f), viewportAspect, 0.1f, 1000.0f);

	view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

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

void calculatePlane(const tuple<aiVector3D*, aiVector3D*, aiVector3D*> triangle, float &a, float &b, float &c, float &d) {
	aiVector3D *v1 = get<0>(triangle);
	aiVector3D *v2 = get<1>(triangle);
	aiVector3D *v3 = get<2>(triangle);
	d = 10000;
	float detD = determinant(mat3(
		v1->x, v1->y, v1->z,
		v2->x, v2->y, v2->z,
		v3->x, v3->y, v3->z)) + epsilon<float>();
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

bool toLeft(const aiVector3D &p, const aiVector3D &q, const vec3 &s) {
	return	p.x * q.z - p.z * q.x
		+ q.x * s.z - q.z * s.x
		+ s.x * p.z - s.z * p.x > 0;
}

bool inTriangleProjectY(const aiVector3D &p, const aiVector3D &q, const aiVector3D &s, const vec3 &r) {
	bool left1 = toLeft(p, q, r);
	bool left2 = toLeft(q, s, r);
	bool left3 = toLeft(s, p, r);
	//printf("toLeft: (%d %d %d)\n", left1, left2, left3);
	return left1 == left2 && left2 == left3;
}

float calculateAltitude(float a, float b, float c, float d) {
	return -(a * cameraPos.x + c * cameraPos.z + d) / b;
}

// 返回 true 表示交点在线段内，false 表示不在
bool calculateSegmentPlaneIntersection(const vec3 &v1, const vec3 &v2, float a, float b, float c, float d, vec3& v) {
	float dx = v1.x - v2.x, dy = v1.y - v2.y, dz = v1.z - v2.z;
	float detD = determinant(mat4(
		a, b, c, 0,
		1, 0, 0, dx,
		0, 1, 0, dy,
		0, 0, 1, dz
	));
	float detDt = determinant(mat4(
		a, b, c, -d,
		1, 0, 0, v1.x,
		0, 1, 0, v1.y,
		0, 0, 1, v1.z
	));
	float t = detDt / detD;
	v = vec3(v1.x - dx * t, v1.y - dy * t, v1.z - dz * t);

	return 0 < t && t <= 1;
}

float cameraHeight = 2;
float fallSpeed = 1;
tuple<aiVector3D*, aiVector3D*, aiVector3D*> cameraTriangle;
// 让镜头下掉至地面上
void fallCamera() {
	float a, b, c, d;
	calculatePlane(cameraTriangle, a, b, c, d);
	float altitude = calculateAltitude(a, b, c, d);
//	printf("altitude: %.2f\n", altitude);
	if (isinf(altitude)) return;

	float newY = std::max(cameraPos.y - fallSpeed, altitude + cameraHeight);
	if (cameraPos.y != newY) {
		//printf("newY: %.2f\n", newY);
		//if (isinf(newY)) {
			//printf("inf: %.2f %.2f %.2f %.2f %.lf\n", cameraPos.y, fallSpeed, altitude, cameraHeight, epsilon<float>());
		//}
		cameraPos.y = newY;
		lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glutPostRedisplay();
	}
}

tuple<aiVector3D*, aiVector3D*, aiVector3D*> getCameraTriangle() {
	tuple<aiVector3D*, aiVector3D*, aiVector3D*> t = make_tuple(nullptr, nullptr, nullptr);
	float altitude = 0;
	for (unsigned i = 0; i < grounds.size(); ++i) {
		aiMesh *ground = grounds[i];
		for (unsigned j = 0; j < ground->mNumFaces; ++j) {
			aiFace &face = ground->mFaces[j];
			tuple<aiVector3D*, aiVector3D*, aiVector3D*> newT = make_tuple(
				&ground->mVertices[face.mIndices[0]],
				&ground->mVertices[face.mIndices[1]],
				&ground->mVertices[face.mIndices[2]]);
			if (inTriangleProjectY(*get<0>(newT), *get<1>(newT), *get<2>(newT), cameraPos)) {
				float a, b, c, d;
				calculatePlane(newT, a, b, c, d);
				float newAltitude = calculateAltitude(a, b, c, d);
				if (altitude < newAltitude) {
					t = newT;
					altitude = newAltitude;
				}
				//printf("camera: (%.2f, %.2f, %.2f) in triangle (%.2f, %.2f, %.2f), (%.2f, %.2f, %.2f), (%.2f, %.2f, %.2f)\n",
				//cameraPos.x, cameraPos.y, cameraPos.z,
				//get<0>(t)->x, get<0>(t)->y, get<0>(t)->z,
				//get<1>(t)->x, get<1>(t)->y, get<1>(t)->z,
				//get<2>(t)->x, get<2>(t)->y, get<2>(t)->z);
			}
		}
	}

	return t;
}

bool collided(const vec3 &start, const vec3 &end) {
	long numFaces = 0;
	for (vector<aiMesh*>::iterator i = obstacles.begin(), oEnd = obstacles.end(); i < oEnd; ++i) {
		aiMesh *obstacle = *i;
		numFaces += obstacle->mNumFaces;
		for (unsigned j = 0; j < obstacle->mNumFaces; ++j) {
			aiFace &face = obstacle->mFaces[j];
			tuple<aiVector3D*, aiVector3D*, aiVector3D*> t = make_tuple(
				&obstacle->mVertices[face.mIndices[0]],
				&obstacle->mVertices[face.mIndices[1]],
				&obstacle->mVertices[face.mIndices[2]]);

			float a, b, c, d;
			calculatePlane(t, a, b, c, d);
			vec3 p;
			if (!calculateSegmentPlaneIntersection(start, end, a, b, c, d, p)) continue;
			if (inTriangleProjectY(*get<0>(t), *get<1>(t), *get<2>(t), p)) return true;
		}
	}
	printf("numFaces: %ld\n", numFaces);
	return false;
}

void My_Timer(int val)
{
	++time;
	if (CAMERA_TYPE2) {
		cameraTriangle = getCameraTriangle();
		if (get<0>(cameraTriangle)) fallCamera();
	}
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

		vec3 up = CAMERA_TYPE2 ? vec3(0, 1, 0) : cameraUp;

		mat4 rotation = rotate(mat4(), (float)deg2rad(length(vec2(dx, dy))) * speed,
			normalize(up) * dx + normalize(cross(cameraFront, up)) * dy);

		//if (FPS_CAMERA) { // 限制镜头仰角俯角
		//	vec3 horizontalFront = cameraFront;
		//	horizontalFront.y = 0;

		//	float angle = degrees(acos(dot(normalize(cameraFront), normalize(horizontalFront))));
		//	if (abs(angle) < 20) cameraTarget = mat3(rotation) * cameraTarget;
		//	printf("angle: %.2f\n", angle);

		//} else {
			//cameraTarget = mat3(rotation) * cameraTarget;
			cameraFront = mat3(rotation) * cameraFront;
		//}
		cameraUp = mat3(rotation) * cameraUp;


		view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		oldX = x;
		oldY = y;

		glutPostRedisplay();

		printf("cameraPos:	(%.2f, %.2f, %.2f)\n", cameraPos.x, cameraPos.y, cameraPos.z);
		printf("cameraFront:	(%.2f, %.2f, %.2f)\n", cameraFront.x, cameraFront.y, cameraFront.z);
		printf("cameraUp:	(%.2f, %.2f, %.2f)\n", cameraUp.x, cameraUp.y, cameraUp.z);
	}
}

void My_Mouse(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN)
	{
//		printf("Mouse %d is pressed at (%d, %d)\n", button, x, y);
		oldX = x;
		oldY = y;
	}
	else if(state == GLUT_UP)
	{
//		printf("Mouse %d is released at (%d, %d)\n", button, x, y);
	}
}

void My_Keyboard(unsigned char key, int x, int y)
{
//	printf("Key %c is pressed at (%d, %d)\n", key, x, y);
	vec3 dPos = vec3(0);
	bool moving = true;
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
			moving = false;
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
			moving = false;
			break;
		}
	}

	if (!moving) return;

	//if (collided(cameraPos, cameraPos + dPos)) {
	//	printf("collided\n");
	//	return;
	//}

	cameraPos += dPos;
	view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	printf("cameraPos:		(%.2f, %.2f, %.2f)\n", cameraPos.x, cameraPos.y, cameraPos.z);
	printf("cameraFront:	(%.2f, %.2f, %.2f)\n", cameraFront.x, cameraFront.y, cameraFront.z);
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
	glutInitWindowPosition(500, 100);
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
