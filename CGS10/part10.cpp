// Arkadiusz Gabrys qe83mepi
// Agnieszka Zacher by57zeja

#include "camera.h"
#include "offLoader.h"

#include <QtOpenGL/QGLWidget>
#include <QtGui/qimage.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

// some global variables:
bool wireframe_mode = false;

// window size
int window_widht = 1024;
int window_height = 1024;

// global camera variable
cameraSystem cam;
const float forwardDelta = 2;
const float angleDelta = 2.0f;
glm::vec2 mouseStartPosition;

// space
float spaceLength = 400;

float t  = 0;  // the time parameter (incremented in the idle-function)
float speed = 0.1;  // rotation speed of the light source in degree/frame
 
int currentX,currentY; // global variables to store the current mouse position

// textures
glm::vec4 planetColor(1.0);
glm::vec4 backgroundColor(0.0);
GLubyte blackMask[] = { 0 };
GLuint earthTex, earthMaskTex, earthTex_night, earthTex_normal, moonTex, saturnTex, backgroundTex, blackMaskTex; 

// file paths
const string background_filePath = "./data/background.jpg";
const string earthDay_filePath = "./data/earth_day.jpg";
const string earthMask_filePath = "./data/earth_reflect.jpg";
const string earthNormals_filePath = "./data/earth_normals.jpg";
const string earthNight_filePath = "./data/earth_night.jpg";
const string moon_filePath = "./data/moon.jpg";
const string saturn_filePath = "./data/saturn.jpg";

const float planetSlices = 124;
const float planetStacks = 124;

// sun
const double sunRadius = 25;
const double sunSlices = 100;
const double sunStacks = 100;
glm::vec4 sunColor(1.0, 1.0, 0.0, 1.0);
glm::vec4 lightSource(0.0, 0.0, 0.0, 1.0);

// earth
const double earthRadius = 12;
const float earthDegree = 2.0;
glm::vec4 earthColor = planetColor;
// moon
const double moonRadius = 6;
const float moonDegree = 4.0;
glm::vec4 moonColor = planetColor;

// saturn
const double saturnRadius = 16;
const float saturnDegree = 1.0;
glm::vec4 saturnColor = planetColor;
// rings
const double distanceFromSaturn = 7.0;
const double distanceBetweenRings = 0.02;
const double numberOfRings = 10;
const double numberOfCircleSegments = 60;
glm::vec4 ringsColor(0.8, 0.6, 0.5, 0.0);

// shuttle
OffObject *objA;    // the shuttle
glm::vec4 shuttleColor(0.7, 0.7, 0.7, 1.0);

// corona vectors
glm::vec3 nCorona, uCorona, rCorona;

/**
 * @brief P,V,M:
 * your matrices for setting the scene, no matrix stack anymore
 */
glm::mat4 P, V, M;		

/**
 * @brief The ShaderUniforms struct:
 * every shader has its own uniforms,
 * binding of uniforms is done in bindUniforms()
 *
 */
struct ShaderUniforms
{
	GLuint Shader;
	GLint location_MVP;
	GLint location_MV;
    GLint location_P;
	GLint location_NormalMatrix ; 
	GLint location_Time;
	GLint location_LightSourceViewSpace;
	GLint location_Color;

	GLint location_Texture;
    GLint location_Mask;

    void bindUniforms(glm::mat4& M, glm::mat4& V, glm::mat4& P, glm::vec4& LightSource, glm::vec4& Color, float  t, GLuint TexID, GLuint TexMaskID = 0,
                      GLuint Tex2ID = 0, std::string Tex2Name = "", GLuint Tex3ID = 0, std::string Tex3Name = "")
	{		
		location_Time					= glGetUniformLocation(Shader, "Time");
		location_MVP					= glGetUniformLocation(Shader, "MVP");
		location_MV						= glGetUniformLocation(Shader, "MV");
        location_P                     = glGetUniformLocation(Shader, "P");
		location_NormalMatrix			= glGetUniformLocation(Shader, "NormalMatrix");
		location_LightSourceViewSpace	= glGetUniformLocation(Shader, "LightSource");
		location_Color					= glGetUniformLocation(Shader, "Color");

        location_Texture = glGetUniformLocation(Shader, "Texture");
        location_Mask = glGetUniformLocation(Shader, "Mask");

        glm::mat4 MP            = P;
		glm::mat4 MV			= V*M;
		glm::mat4 MVP			= P*MV;
		glm::mat3 NormalMatrix	= glm::transpose(glm::inverse(glm::mat3(MV)));


        glUniformMatrix4fv(location_MVP, 1, false, glm::value_ptr(MVP));
        glUniformMatrix4fv(location_MV, 1, false, glm::value_ptr(MV));
        glUniformMatrix4fv(location_P, 1, false, glm::value_ptr(MP));
        glUniformMatrix3fv(location_NormalMatrix, 1, false, glm::value_ptr(NormalMatrix));
        glUniform4fv(location_LightSourceViewSpace, 1, glm::value_ptr(LightSource));
        glUniform4fv(location_Color, 1, glm::value_ptr(Color));
        glUniform1f(location_Time, 10 * t);

        glUniform1i(location_Texture, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TexID);

        if (TexMaskID != 0)
        {
            glUniform1i(location_Mask, 1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, TexMaskID);
        }

        if (Tex2ID != 0)
        {
            glUniform1i(glGetUniformLocation(Shader, Tex2Name.c_str()), 2);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, Tex2ID);
        }

        if (Tex3ID != 0)
        {
            glUniform1i(glGetUniformLocation(Shader, Tex3Name.c_str()), 3);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, Tex3ID);
        }
	}

};

ShaderUniforms SunShader, TexturePhongShader, BumpShader, CoronaShader; // the shaders

/**
/**
 * @brief The GeometryData struct:
 * store the VertexArrayObject and number of vertices and indices
 */
struct GeometryData
{
	GLuint vao;
	unsigned int numVertices;
	unsigned int numIndices; 
};

GeometryData geometryShuttle, geometryCube, geometrySphere, geometryRings, geometryCorona;

/**
 * @brief The Vertex struct:
 * store vertices with according normals
 * and texture coordinates
 * NEW: store tangents
 */
struct Vertex {

	Vertex(glm::vec3 p, glm::vec3 n )
	{
		position[0] = p.x;
		position[1] = p.y;
		position[2] = p.z;
		position[3] = 1;

		normal[0] = n.x;
		normal[1] = n.y;
		normal[2] = n.z;			 
		normal[3] = 1;
			 
	}; 
	Vertex(glm::vec3 p, glm::vec3 n, glm::vec2 t)	
	{
		position[0] = p.x;
		position[1] = p.y;
		position[2] = p.z;
		position[3] = 1;

		normal[0] = n.x;
		normal[1] = n.y;
		normal[2] = n.z;			 
		normal[3] = 1;

		texcoord[0] = t.x;
		texcoord[1] = t.y;
	};

    Vertex(glm::vec3 p, glm::vec3 n, glm::vec2 t, glm::vec3  tan)
    {
        position[0] = p.x;
        position[1] = p.y;
        position[2] = p.z;
        position[3] = 1;

        normal[0] = n.x;
        normal[1] = n.y;
        normal[2] = n.z;
        normal[3] = 1;

        texcoord[0] = t.x;
        texcoord[1] = t.y;

        tangent[0] = tan.x;
        tangent[1] = tan.y;
        tangent[2] = tan.z;
        tangent[3] = 1;
    };

	Vertex(glm::vec3 p, glm::vec3 n, glm::vec2 t, glm::vec4 tan)	
	{
		position[0] = p.x;
		position[1] = p.y;
		position[2] = p.z;
		position[3] = 1;

		normal[0] = n.x;
		normal[1] = n.y;
		normal[2] = n.z;			 
		normal[3] = 1;

		texcoord[0] = t.x;
		texcoord[1] = t.y;

		tangent[0] = tan.x;
		tangent[1] = tan.y;
		tangent[2] = tan.z;
		tangent[3] = tan.w;
	};

	GLfloat position[4];
	GLfloat normal[4];
	GLfloat texcoord[2];
	GLfloat tangent[4];
};

 
void bindVertexArrayObjects(GeometryData& geometry, const std::vector<Vertex> &vertexdata, const std::vector<unsigned short> &indices) 
{
	
	//*bind to GL
	glGenVertexArrays(1, &(geometry.vao));
    glBindVertexArray(geometry.vao);
	
	geometry.numVertices = vertexdata.size();
	geometry.numIndices = indices.size();

	// Create and bind a BO for vertex data
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, geometry.numVertices * sizeof(Vertex), vertexdata.data(), GL_STATIC_DRAW);
	

	// set up vertex attributes
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	
	// copy data into the buffer object


	// Create and bind a BO for index data
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	// copy data into the buffer object
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry.numIndices * sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);


    glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

///TODO
void createCorona()
{
    glm::vec2 t(1.0);
    glm::vec3 n(0, 0, 1);
    glm::vec3 p;

    // set corona vectors
    nCorona = n;
    uCorona = glm::vec3(0, 1, 0);
    rCorona = glm::vec3(1, 0, 0);

    glm::vec4 colorIn(1.0, 1.0, 0.0, 1.0);
    glm::vec4 colorOut(1.0, 1.0, 0.0, 0.0);
    colorOut.a = 0.0;

    unsigned short index = 0;
    std::vector<Vertex> vertexdata;
    std::vector<unsigned short> indices;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// TODO: Alpha blending
    // Create Corona Geometry
    // assign color attributes to vertices
    // check "struct Vertex" and "bindVertexArrayObjects" for details
    vertexdata.push_back(Vertex(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), t, colorIn));
    indices.push_back(index++);

    float r = 50.0f;
    for (int i = 0; i<100; i++)
    {
        float arg = PI*(float)i / 50.0;
        p = glm::vec3(r*cos(arg), r*sin(arg), 0);
        vertexdata.push_back(Vertex(p, n, t, colorOut));
        indices.push_back(index++);
    }

    indices.push_back(1);

    bindVertexArrayObjects(geometryCorona, vertexdata, indices);

}

void createRings()
{
    glm::vec2 t;
    glm::vec3 n(0, 0, -1);
    glm::vec3 p;

    unsigned short index = 0;
    std::vector<Vertex> vertexdata;
    std::vector<unsigned short> indices;

    float r = 1;
    for (int i = 0; i<100; i++)
    {
        float arg = PI*(float)i / 50.0;
        p = glm::vec3(r*cos(arg), 0, r*sin(arg));
        vertexdata.push_back(Vertex(p, n, t));
        indices.push_back(index++);
    }

    bindVertexArrayObjects(geometryRings, vertexdata, indices);

}

void createSphereWithTangents()
{
	float r=1;
	int slices = planetSlices;
	int stacks = planetStacks;
	
	float dTheta = 2.0*PI/(float)stacks;
	float dPhi = PI/(float)slices;  

	std::vector<Vertex> vertexdata ;
	std::vector<unsigned short> indices;

	glm::vec2 t(0.5,1.0);
    glm::vec3 n(0,1,0);    
    glm::vec3 p(0,r,0);  
    glm::vec3 tangent(0,0,1);
	vertexdata.push_back(Vertex (p,n,t,tangent));
   
	//North pole
	for (int i = stacks; i>=0; i--) {
		glm::vec2 t(1-i*dTheta/(2.0*PI),(PI-dPhi)/PI);
		glm::vec3 n(sin(dPhi)*cos(i*dTheta), cos(dPhi), sin(dPhi)*sin(i*dTheta));
		glm::vec3 p(r*sin(dPhi)*cos(i*dTheta), r*cos(dPhi), r*sin(dPhi)*sin(i*dTheta));		
		
		glm::vec3 tangent(sin(dPhi)*-sin(i*dTheta), 0, sin(dPhi)*cos(i*dTheta));
		tangent = glm::normalize(tangent);

		vertexdata.push_back(Vertex (p,n,t, tangent));
    } 
	int triangleID = 0;
	for ( ;triangleID < stacks; triangleID++) 
	{
		indices.push_back(0);
		indices.push_back(triangleID);
		indices.push_back(triangleID+1);
	}
	
	indices.push_back(0);
	indices.push_back(triangleID);
	indices.push_back(1);
	
	
	// Middle Part 
	 
	//	v0--- v2
	//  |  	/ |
	//  |  /  | 
	//  | /   |
	//  v1--- v3

	for (int j=1; j<slices-1; j++) 
	{
		for (int i = stacks; i>=0; i--) 
		{    			
			t = glm::vec2 (1-i*dTheta/(2.0*PI),(PI-(j+1)*dPhi)/PI); 
			n = glm::vec3 (sin((j+1)*dPhi)*cos(i*dTheta), cos((j+1)*dPhi), sin((j+1)*dPhi)*sin(i*dTheta));
			p = glm::vec3 (r*sin((j+1)*dPhi)*cos(i*dTheta), r*cos((j+1)*dPhi), r*sin((j+1)*dPhi)*sin(i*dTheta));

			glm::vec3 tangent(sin((j+1)*dPhi)*-sin(i*dTheta), 0, sin((j+1)*dPhi)*cos(i*dTheta));
			tangent = glm::normalize(tangent);     
	
			vertexdata.push_back(Vertex (p,n,t, tangent));

			//add two triangles
 
			indices.push_back(vertexdata.size()  - stacks-2);		//v0
			indices.push_back(vertexdata.size() -1);				//v1
			indices.push_back(vertexdata.size()  - stacks-1);		//v2
 					 
			indices.push_back(vertexdata.size() - stacks-1);		//v2
			indices.push_back(vertexdata.size() - 1);				//v1
			indices.push_back(vertexdata.size() );			//v3
			 
		}
		
	}     
	
	//South pole
	t = glm::vec2 (0.5,0); 
	n = glm::vec3 (0,-1,0);
	p = glm::vec3 (0,-r,0);	
    tangent = glm::vec3(0,0,1);

	vertexdata.push_back(Vertex (p,n,t, tangent));

	int lastVertex=vertexdata.size()-1;

 	triangleID = 0 ;
	for ( ;triangleID <= stacks; triangleID++) 
	{
		indices.push_back(lastVertex);
		indices.push_back(vertexdata.size()-triangleID-1);
		indices.push_back(vertexdata.size()-triangleID-2);
	}
	
 
	bindVertexArrayObjects(geometrySphere,vertexdata,indices);

}

/// creates a cube with length a
void createCube(float a)
{
    std::vector<Vertex> vertexdata;
    std::vector<unsigned short> indices;

    glm::vec2 t;
    glm::vec3 n;
    glm::vec3 p;

    unsigned short index = 0;
    t = glm::vec2(0, 0);
    p = glm::vec3(a, a, -a);
    vertexdata.push_back(Vertex(p, n, t));
    indices.push_back(index++);
    t = glm::vec2(0, 1);
    p = glm::vec3(-a, a, -a);
    vertexdata.push_back(Vertex(p, n, t));
    indices.push_back(index++);
    t = glm::vec2(1, 1);
    p = glm::vec3(-a, -a, -a);
    vertexdata.push_back(Vertex(p, n, t));
    indices.push_back(index++);
    t = glm::vec2(1, 0);
    p = glm::vec3(a, -a, -a);
    vertexdata.push_back(Vertex(p, n, t));
    indices.push_back(index++);

    // +Z

    t = glm::vec2(0, 0);
    p = glm::vec3(a, a, a);
    vertexdata.push_back(Vertex(p, n, t));
    indices.push_back(index++);
    t = glm::vec2(0, 1);
    p = glm::vec3(a, -a, a);
    vertexdata.push_back(Vertex(p, n, t));
    indices.push_back(index++);
    t = glm::vec2(1, 1);
    p = glm::vec3(-a, -a, a);
    vertexdata.push_back(Vertex(p, n, t));
    indices.push_back(index++);
    t = glm::vec2(1, 0);
    p = glm::vec3(-a, a, a);
    vertexdata.push_back(Vertex(p, n, t));
    indices.push_back(index++);


    // +X     
    t = glm::vec2(0, 0);
    p = glm::vec3(a, a, -a);
    vertexdata.push_back(Vertex(p, n, t));
    indices.push_back(index++);
    t = glm::vec2(0, 1);
    p = glm::vec3(a, -a, -a);
    vertexdata.push_back(Vertex(p, n, t));
    indices.push_back(index++);
    t = glm::vec2(1, 1);
    p = glm::vec3(a, -a, a);
    vertexdata.push_back(Vertex(p, n, t));
    indices.push_back(index++);
    t = glm::vec2(1, 0);
    p = glm::vec3(a, a, a);
    vertexdata.push_back(Vertex(p, n, t));
    indices.push_back(index++);

    // -X      
    t = glm::vec2(0, 0);
    p = glm::vec3(-a, a, -a);
    vertexdata.push_back(Vertex(p, n, t));
    indices.push_back(index++);
    t = glm::vec2(0, 1);
    p = glm::vec3(-a, a, a);
    vertexdata.push_back(Vertex(p, n, t));
    indices.push_back(index++);
    t = glm::vec2(1, 1);
    p = glm::vec3(-a, -a, a);
    vertexdata.push_back(Vertex(p, n, t));
    indices.push_back(index++);
    t = glm::vec2(1, 0);
    p = glm::vec3(-a, -a, -a);
    vertexdata.push_back(Vertex(p, n, t));
    indices.push_back(index++);

    // +Y
    t = glm::vec2(0, 0);
    p = glm::vec3(-a, a, -a);
    vertexdata.push_back(Vertex(p, n, t));
    indices.push_back(index++);
    t = glm::vec2(0, 1);
    p = glm::vec3(a, a, -a);
    vertexdata.push_back(Vertex(p, n, t));
    indices.push_back(index++);
    t = glm::vec2(1, 1);
    p = glm::vec3(a, a, a);
    vertexdata.push_back(Vertex(p, n, t));
    indices.push_back(index++);
    t = glm::vec2(1, 0);
    p = glm::vec3(-a, a, a);
    vertexdata.push_back(Vertex(p, n, t));
    indices.push_back(index++);

    // +Y
    t = glm::vec2(0, 0);
    p = glm::vec3(-a, -a, -a);
    vertexdata.push_back(Vertex(p, n, t));
    indices.push_back(index++);
    t = glm::vec2(0, 1);
    p = glm::vec3(-a, -a, a);
    vertexdata.push_back(Vertex(p, n, t));
    indices.push_back(index++);
    t = glm::vec2(1, 1);
    p = glm::vec3(a, -a, a);
    vertexdata.push_back(Vertex(p, n, t));
    indices.push_back(index++);
    t = glm::vec2(1, 0);
    p = glm::vec3(a, -a, -a);
    vertexdata.push_back(Vertex(p, n, t));
    indices.push_back(index++);


    bindVertexArrayObjects(geometryCube, vertexdata, indices);
}

/// loads shuttle data from off-file and fills according GeometryData
void createShuttle()
{
    objA = new OffObject("./data/shuttle.off");

    std::vector<Vertex> vertexdata;        // store the vertices of the shuttle here
    std::vector<unsigned short> indices;    // store the according indices here

    geometryShuttle.numVertices = objA->noOfVertices;
    geometryShuttle.numIndices = objA->noOfFaces * 3;

    // TODO: Fill vertexdata
    for (int i = 0; i < geometryShuttle.numVertices; i++)
        vertexdata.push_back(Vertex(objA->vertexList[i], objA->normalsList[i]));

    // TODO: Fill indexData
    for (int i = 0; i< objA->noOfFaces; i++)
    {
        indices.push_back(objA->faceList[i].A);
        indices.push_back(objA->faceList[i].B);
        indices.push_back(objA->faceList[i].C);
    }

    bindVertexArrayObjects(geometryShuttle, vertexdata, indices);
}

void initTexture(GLint name, GLint w, GLint h, GLubyte *data) {
	  
	glBindTexture(GL_TEXTURE_2D, name);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);	
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, w,h,0,GL_RGBA,GL_UNSIGNED_BYTE, data);
}


void initGL() {
  glClearColor(0,0,0,0);   // set the clear color to black
  glEnable(GL_DEPTH_TEST); // turn on the depth test
  glEnable(GL_CULL_FACE);  // turn on backface culling
   
  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

  // set the camera:
  V = cam.getView();

  // create the shaders (the functions are defined in helper.h)

    createProgram_VF("sun_VS.glsl","sun_FS.glsl",&SunShader.Shader);
    //createProgram_VF("Light_and_Tex_VS.glsl","../../solution_new/Light_and_Tex_FS.glsl",&TexturePhongShader.Shader);  You don't need this here
    createProgram_VF("Light_and_Tex_VS.glsl", "Light_and_Tex_FS.glsl", &TexturePhongShader.Shader);
    createProgram_VF("BumpShader_VS.glsl","BumpShader_FS.glsl",&BumpShader.Shader);
    createProgram_VF("CoronaShader_VS.glsl", "CoronaShader_FS.glsl", &CoronaShader.Shader);
  
    // load, init and bind the textures:
    QImage earth, moon, saturn, back, earth_night, earth_norm, mask;

    mask.load(earthMask_filePath.c_str());
    mask = QGLWidget::convertToGLFormat(mask);
    glGenTextures(1, &earthMaskTex);
    initTexture(earthMaskTex, mask.width(), mask.height(), mask.bits());

    back.load(background_filePath.c_str());
    back = QGLWidget::convertToGLFormat(back);
    glGenTextures(1, &backgroundTex);
    initTexture(backgroundTex, back.width(), back.height(), back.bits());

    saturn.load(saturn_filePath.c_str());
    saturn = QGLWidget::convertToGLFormat(saturn);
    glGenTextures(1, &saturnTex);
    initTexture(saturnTex, saturn.width(), saturn.height(), saturn.bits());

    moon.load(moon_filePath.c_str());
    moon = QGLWidget::convertToGLFormat(moon);
    glGenTextures(1, &moonTex);
    initTexture(moonTex, moon.width(), moon.height(), moon.bits());

    earth.load(earthDay_filePath.c_str());
	earth = QGLWidget::convertToGLFormat(earth);
	glGenTextures(1,&earthTex);
	initTexture(earthTex, earth.width(), earth.height(), earth.bits());
	  
	earth_night.load(earthNight_filePath.c_str());
	earth_night = QGLWidget::convertToGLFormat(earth_night);
	glGenTextures(1,&earthTex_night);
	initTexture(earthTex_night, earth_night.width(),earth_night.height(),earth_night.bits());
	  
	earth_norm.load(earthNormals_filePath.c_str());
	earth_norm = QGLWidget::convertToGLFormat(earth_norm);
	glGenTextures(1,&earthTex_normal);
	initTexture(earthTex_normal, earth_norm.width(),earth_norm.height(),earth_norm.bits());

    glGenTextures(1, &blackMaskTex);
    initTexture(blackMaskTex, 1, 1, blackMask);

    // the space shuttle & other geometry:
    createCorona();
    createSphereWithTangents();
    createShuttle();
    createRings();
    createCube(spaceLength);

    printf("Init done!");
}
 
void reshape(int w, int h)
{
	glViewport(0,0,(GLsizei) w, (GLsizei) h);
 
  	P = glm::perspective(70.0f, (GLfloat) w/ (GLfloat) h, 10.0f, 800.0f);  

}

void onIdle()
{
    // set the camera:
    V = cam.getView();
    lightSource = V * glm::vec4(0, 0, 0, 1);

    t += speed;  // increase the time parameter

    glutPostRedisplay();
}

void display()
{ 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
 
    // bind Shader & bind Uniforms
    glUseProgram(SunShader.Shader);
    M = glm::mat4(1.0f) * glm::rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    SunShader.bindUniforms(M, V, P, lightSource, sunColor, t, 0, 0);
    glutSolidSphere(sunRadius, sunSlices, sunStacks);

    glUseProgram(TexturePhongShader.Shader);
    // background
    M = glm::mat4(1.0f);
    TexturePhongShader.bindUniforms(M, V, P, lightSource, backgroundColor, t, backgroundTex, blackMaskTex);
    glBindVertexArray(geometryCube.vao);
    glDrawElements(GL_QUADS, geometryCube.numIndices, GL_UNSIGNED_SHORT, (void*)0);
    glBindVertexArray(0);

    // sphere
    glBindVertexArray(geometrySphere.vao);

    glUseProgram(BumpShader.Shader);
    // earth
    M = glm::rotate(earthDegree * t, glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::translate(glm::vec3(50.0f + earthRadius, 0.0f, 0.0f))
        * glm::scale(glm::vec3(earthRadius))
        * glm::rotate(earthDegree * t, glm::vec3(0.0f, 1.0f, 0.0f));
    BumpShader.bindUniforms(M, V, P, lightSource, planetColor, t, earthTex, earthMaskTex, earthTex_night, "Texture_Night", earthTex_normal, "Texture_Normal");
    glDrawElements(GL_TRIANGLES, geometrySphere.numIndices, GL_UNSIGNED_SHORT, (void*)0);

    glUseProgram(TexturePhongShader.Shader);
    // moon
    M = glm::rotate(earthDegree * t, glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::translate(glm::vec3(50.0f + earthRadius, 0.0f, 0.0f))
        * glm::rotate(moonDegree * t, glm::vec3(0.0f, 0.0f, 1.0f))
        * glm::translate(glm::vec3(20.0f, 0.0f, 0.0f))
        * glm::scale(glm::vec3(moonRadius));
    TexturePhongShader.bindUniforms(M, V, P, lightSource, planetColor, t, moonTex, blackMaskTex);
    glDrawElements(GL_TRIANGLES, geometrySphere.numIndices, GL_UNSIGNED_SHORT, (void*)0);

    // saturn
    M = glm::rotate(saturnDegree * t, glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::translate(glm::vec3(100.0f + saturnRadius, 0.0f, 0.0f))
        * glm::scale(glm::vec3(saturnRadius));
    TexturePhongShader.bindUniforms(M, V, P, lightSource, planetColor, t, saturnTex, blackMaskTex);
    glDrawElements(GL_TRIANGLES, geometrySphere.numIndices, GL_UNSIGNED_SHORT, (void*)0);

    // unbind sphere
    glBindVertexArray(0);

    // rings
    M = glm::rotate(saturnDegree * t, glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::translate(glm::vec3(100.0f + saturnRadius, 0.0f, 0.0f))
        * glm::scale(glm::vec3(saturnRadius + distanceFromSaturn));

    glBindVertexArray(geometryRings.vao);
    for (int i = 0; i < numberOfRings; i++)
    {
        M = M * glm::scale(glm::vec3(1.0f + distanceBetweenRings));
        TexturePhongShader.bindUniforms(M, V, P, lightSource, ringsColor, t, 0, 0);
        glDrawElements(GL_LINE_LOOP, geometryRings.numIndices, GL_UNSIGNED_SHORT, (void*)0);
    }
    glBindVertexArray(0);

    // space shuttle
    M = glm::rotate(saturnDegree * -t, glm::vec3(0.0f, 0.0f, 1.0f))
        * glm::rotate(180.0f, glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::rotate(180.0f, glm::vec3(1.0f, 0.0f, 0.0f))
        * glm::translate(glm::vec3(100.0f + saturnRadius, 0.0f, 0.0f))
        * glm::rotate(90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    TexturePhongShader.bindUniforms(M, V, P, lightSource, shuttleColor, t, 0, blackMaskTex);
    glBindVertexArray(geometryShuttle.vao);
    glDrawElements(GL_TRIANGLES, geometryShuttle.numIndices, GL_UNSIGNED_SHORT, (void*)0);
    glBindVertexArray(0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(CoronaShader.Shader);
    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// TODO: Alpha blending
    // Draw the Corona geometry ! Triangle Fan !
    // 1. It must allway be perpendicular to the camera
    // 2. Bind the proper matrices to the CoronaShader before drawing
    // 3. Find the proper blending function
    glBindVertexArray(geometryCorona.vao);
    M = glm::mat4(1.0f);
    CoronaShader.bindUniforms(M, V, P, lightSource, sunColor, t, 0);
    glDrawElements(GL_TRIANGLE_FAN, geometryCorona.numIndices, GL_UNSIGNED_SHORT, (void*)0);
    glBindVertexArray(0);

    glUseProgram(0);
    glDisable(GL_BLEND);

	glutSwapBuffers();
}
 

void cleanUp() {
 
	glDeleteTextures(1, &earthTex);
	glDeleteTextures(1, &moonTex);
	glDeleteTextures(1, &saturnTex);   
	glDeleteTextures(1, &backgroundTex);
    glDeleteTextures(1, &earthMaskTex);
    glDeleteTextures(1, &blackMaskTex);
    glDeleteTextures(1, &earthTex_night);
    glDeleteTextures(1, &earthTex_normal);

	glDeleteProgram(SunShader.Shader);
    glDeleteProgram(BumpShader.Shader);
    glDeleteProgram(TexturePhongShader.Shader);
  
    glDeleteVertexArrays(1, &geometryShuttle.vao);
	glDeleteVertexArrays(1, &geometryCube.vao);
	glDeleteVertexArrays(1, &geometrySphere.vao); 
 
}

void onMouseDown(int button, int state, int x, int y)
{
    mouseStartPosition = glm::vec2(x, y);
}

void onMouseMove(int x, int y)
{
    int deltaX = x - mouseStartPosition.x;
    int deltaY = y - mouseStartPosition.y;

    cam.yaw(-deltaX * angleDelta);
    cam.pitch(-deltaY * angleDelta);

    mouseStartPosition = glm::vec2(x, y);
}

// the keyboard handler:
void keyboard(unsigned char key, int x, int y)
{

    switch (key)
    {

    case 27:
        cleanUp();
        exit(1);
        break;

    case 'p': // toggle polygon mode:	
        wireframe_mode = !wireframe_mode;
        if (wireframe_mode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;

        // increase / decrease the speed of the point light P:     
    case '+':
        speed += 0.01;
        break;
    case '-':
        speed -= 0.01;
        break;

    case 'w':
        cam.moveForward(forwardDelta);
        break;

    case 's':
        cam.moveBackward(forwardDelta);
        break;

    case 'a':
        cam.roll(angleDelta);
        break;

    case 'd':
        cam.roll(-angleDelta);
        break;
    }


    glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(1042, 1024);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	
	glutCreateWindow("Planet System");
  
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(onMouseMove);
	glutMouseFunc(onMouseDown);
	glutReshapeFunc(reshape);
	glutIdleFunc(onIdle);
	
	initGL();
  
	glutMainLoop();
}
