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

string cryptedVS = "vojgpsn!gmpbu!Ujnf<wbszjoh!gmpbu!ifjhiu<!wbszjoh!wfd4!w`opsnbm<wbszjoh!wfd4!w`wjfxjohWfd<wpje!nbjo)*|gmpbu!QJ!>!4/2526:376469:8:434957375<gmpbu!Ut!>!npe)21111111/1+)hm`Wfsufy/y,hm`Wfsufy/z,hm`Wfsufy/{*-2111/1*02111/1<gmpbu!Sboe!>npe)21111111/1+)hm`Wfsufy/y,hm`Opsnbm/z,hm`Wfsufy/{*-2111/1*02111/1<w`wjfxjohWfd!>)opsnbmj{f)hm`NpefmWjfxNbusjy+hm`Wfsufy**/yz{<w`opsnbm!>!opsnbmj{f)hm`OpsnbmNbusjy+hm`Opsnbm*<Ut!>!Ut+3/1+QJ<ifjhiu!>1/6+)tjo)Ut,Ujnf+QJ0211/1*,2/1*<gmpbu!pggtfu!>!5/1+Sboe+tjo)Ut,Ujnf+QJ0211/1*<wfd5!wfsufyQpt!>!hm`Wfsufy!,wfd5)hm`Opsnbm+pggtfu-1/1*<hm`Qptjujpo!>!hm`NpefmWjfxQspkfdujpoNbusjy!+!wfsufyQpt<~";
string cryptedFS = "wbszjoh!gmpbu!ifjhiu<wbszjoh!wfd4!w`opsnbm<wbszjoh!wfd4!w`wjfxjohWfd<wpje!nbjo)*!|gmpbu!g!>!1/5<gmpbu!joufotf!>!2/1.!nby)1/1-epu)w`opsnbm-.w`wjfxjohWfd**<hm`GsbhDpmps!>!wfd5)2-ifjhiu,joufotf+g-1-1*<~";
const int FSint[] = { 118, 97, 114, 121, 105, 110, 103, 32, 118, 101, 99, 51, 32, 118, 95, 110, 111, 114, 109, 97, 108, 59, 118, 97, 114, 121, 105, 110, 103, 32, 118, 101, 99, 51, 32, 118, 95, 68, 95, 76, 105, 103, 104, 116, 86, 101, 99, 59, 118, 97, 114, 121, 105, 110, 103, 32, 118, 101, 99, 51, 32, 118, 95, 80, 95, 76, 105, 103, 104, 116, 86, 101, 99, 59, 118, 97, 114, 121, 105, 110, 103, 32, 118, 101, 99, 51, 32, 118, 95, 118, 105, 101, 119, 105, 110, 103, 86, 101, 99, 59, 117, 110, 105, 102, 111, 114, 109, 32, 115, 97, 109, 112, 108, 101, 114, 50, 68, 32, 84, 101, 120, 116, 117, 114, 101, 59, 118, 111, 105, 100, 32, 109, 97, 105, 110, 40, 41, 32, 123, 118, 101, 99, 51, 32, 80, 95, 114, 101, 102, 108, 101, 99, 116, 68, 105, 114, 32, 61, 32, 114, 101, 102, 108, 101, 99, 116, 40, 45, 118, 95, 80, 95, 76, 105, 103, 104, 116, 86, 101, 99, 44, 118, 95, 110, 111, 114, 109, 97, 108, 41, 59, 102, 108, 111, 97, 116, 32, 68, 95, 100, 105, 102, 102, 117, 115, 101, 32, 61, 32, 109, 97, 120, 40, 48, 46, 48, 44, 100, 111, 116, 40, 118, 95, 110, 111, 114, 109, 97, 108, 44, 118, 95, 68, 95, 76, 105, 103, 104, 116, 86, 101, 99, 41, 41, 59, 102, 108, 111, 97, 116, 32, 80, 95, 100, 105, 102, 102, 117, 115, 101, 32, 61, 32, 109, 97, 120, 40, 48, 46, 48, 44, 100, 111, 116, 40, 118, 95, 110, 111, 114, 109, 97, 108, 44, 118, 95, 80, 95, 76, 105, 103, 104, 116, 86, 101, 99, 41, 41, 59, 102, 108, 111, 97, 116, 32, 115, 112, 101, 99, 32, 61, 32, 48, 46, 48, 59, 105, 102, 32, 40, 80, 95, 100, 105, 102, 102, 117, 115, 101, 32, 62, 32, 48, 46, 48, 41, 32, 123, 115, 112, 101, 99, 32, 61, 32, 112, 111, 119, 40, 109, 97, 120, 40, 48, 46, 48, 44, 100, 111, 116, 40, 118, 95, 118, 105, 101, 119, 105, 110, 103, 86, 101, 99, 44, 80, 95, 114, 101, 102, 108, 101, 99, 116, 68, 105, 114, 41, 41, 44, 52, 48, 46, 48, 41, 59, 125, 105, 102, 32, 40, 103, 108, 95, 67, 111, 108, 111, 114, 46, 97, 32, 62, 32, 48, 46, 57, 41, 32, 103, 108, 95, 70, 114, 97, 103, 67, 111, 108, 111, 114, 32, 61, 32, 68, 95, 100, 105, 102, 102, 117, 115, 101, 42, 32, 103, 108, 95, 67, 111, 108, 111, 114, 32, 43, 32, 80, 95, 100, 105, 102, 102, 117, 115, 101, 32, 42, 32, 103, 108, 95, 67, 111, 108, 111, 114, 32, 43, 32, 115, 112, 101, 99, 32, 42, 32, 118, 101, 99, 52, 40, 48, 46, 55, 44, 48, 46, 55, 44, 48, 46, 55, 44, 48, 41, 59, 101, 108, 115, 101, 32, 105, 102, 32, 40, 103, 108, 95, 67, 111, 108, 111, 114, 46, 97, 32, 62, 32, 48, 46, 52, 41, 32, 103, 108, 95, 70, 114, 97, 103, 67, 111, 108, 111, 114, 32, 61, 32, 116, 101, 120, 116, 117, 114, 101, 50, 68, 40, 84, 101, 120, 116, 117, 114, 101, 44, 103, 108, 95, 84, 101, 120, 67, 111, 111, 114, 100, 91, 48, 93, 46, 120, 121, 41, 59, 101, 108, 115, 101, 32, 123, 118, 101, 99, 52, 32, 99, 111, 108, 32, 61, 32, 116, 101, 120, 116, 117, 114, 101, 50, 68, 40, 84, 101, 120, 116, 117, 114, 101, 44, 103, 108, 95, 84, 101, 120, 67, 111, 111, 114, 100, 91, 48, 93, 46, 120, 121, 41, 59, 103, 108, 95, 70, 114, 97, 103, 67, 111, 108, 111, 114, 32, 61, 32, 32, 68, 95, 100, 105, 102, 102, 117, 115, 101, 42, 32, 99, 111, 108, 32, 43, 32, 80, 95, 100, 105, 102, 102, 117, 115, 101, 32, 42, 32, 99, 111, 108, 32, 43, 32, 115, 112, 101, 99, 32, 42, 32, 118, 101, 99, 52, 40, 48, 46, 55, 44, 48, 46, 55, 44, 48, 46, 55, 44, 48, 41, 59, 125, 125 };
const int VSint[] = { 118, 97, 114, 121, 105, 110, 103, 32, 118, 101, 99, 51, 32, 118, 95, 110, 111, 114, 109, 97, 108, 59, 118, 97, 114, 121, 105, 110, 103, 32, 118, 101, 99, 51, 32, 118, 95, 68, 95, 76, 105, 103, 104, 116, 86, 101, 99, 59, 118, 97, 114, 121, 105, 110, 103, 32, 118, 101, 99, 51, 32, 118, 95, 80, 95, 76, 105, 103, 104, 116, 86, 101, 99, 59, 118, 97, 114, 121, 105, 110, 103, 32, 118, 101, 99, 51, 32, 118, 95, 118, 105, 101, 119, 105, 110, 103, 86, 101, 99, 59, 118, 111, 105, 100, 32, 109, 97, 105, 110, 40, 41, 32, 123, 118, 101, 99, 52, 32, 112, 111, 115, 105, 116, 105, 111, 110, 32, 61, 32, 103, 108, 95, 77, 111, 100, 101, 108, 86, 105, 101, 119, 77, 97, 116, 114, 105, 120, 42, 103, 108, 95, 86, 101, 114, 116, 101, 120, 59, 118, 95, 118, 105, 101, 119, 105, 110, 103, 86, 101, 99, 32, 61, 32, 40, 110, 111, 114, 109, 97, 108, 105, 122, 101, 40, 45, 112, 111, 115, 105, 116, 105, 111, 110, 41, 46, 120, 121, 122, 41, 59, 118, 95, 110, 111, 114, 109, 97, 108, 32, 61, 32, 110, 111, 114, 109, 97, 108, 105, 122, 101, 40, 103, 108, 95, 78, 111, 114, 109, 97, 108, 77, 97, 116, 114, 105, 120, 42, 103, 108, 95, 78, 111, 114, 109, 97, 108, 41, 59, 118, 101, 99, 52, 32, 80, 95, 76, 105, 103, 104, 116, 80, 111, 115, 32, 61, 32, 103, 108, 95, 76, 105, 103, 104, 116, 83, 111, 117, 114, 99, 101, 91, 48, 93, 46, 112, 111, 115, 105, 116, 105, 111, 110, 59, 118, 101, 99, 52, 32, 68, 95, 76, 105, 103, 104, 116, 68, 105, 114, 32, 61, 32, 45, 103, 108, 95, 76, 105, 103, 104, 116, 83, 111, 117, 114, 99, 101, 91, 49, 93, 46, 112, 111, 115, 105, 116, 105, 111, 110, 59, 118, 95, 80, 95, 76, 105, 103, 104, 116, 86, 101, 99, 32, 61, 32, 40, 110, 111, 114, 109, 97, 108, 105, 122, 101, 40, 103, 108, 95, 76, 105, 103, 104, 116, 83, 111, 117, 114, 99, 101, 91, 48, 93, 46, 112, 111, 115, 105, 116, 105, 111, 110, 32, 45, 32, 112, 111, 115, 105, 116, 105, 111, 110, 41, 41, 46, 120, 121, 122, 59, 118, 95, 68, 95, 76, 105, 103, 104, 116, 86, 101, 99, 32, 61, 32, 40, 110, 111, 114, 109, 97, 108, 105, 122, 101, 40, 103, 108, 95, 76, 105, 103, 104, 116, 83, 111, 117, 114, 99, 101, 91, 49, 93, 46, 112, 111, 115, 105, 116, 105, 111, 110, 41, 41, 46, 120, 121, 122, 59, 103, 108, 95, 70, 114, 111, 110, 116, 67, 111, 108, 111, 114, 32, 61, 32, 103, 108, 95, 67, 111, 108, 111, 114, 59, 103, 108, 95, 80, 111, 115, 105, 116, 105, 111, 110, 32, 61, 103, 108, 95, 77, 111, 100, 101, 108, 86, 105, 101, 119, 80, 114, 111, 106, 101, 99, 116, 105, 111, 110, 77, 97, 116, 114, 105, 120, 42, 32, 103, 108, 95, 86, 101, 114, 116, 101, 120, 59, 103, 108, 95, 84, 101, 120, 67, 111, 111, 114, 100, 91, 48, 93, 32, 61, 32, 103, 108, 95, 77, 117, 108, 116, 105, 84, 101, 120, 67, 111, 111, 114, 100, 48, 59, 32, 125 };

bool wireframe_mode = false;
bool raytracing = true;

float D_light_direction[4]; // Direction of the directional Light Source D
float P_light_position[4];  // Position of the point Light P

float t = 0;  // Time parameter (incremented in the idle-function)
float speed = 0.1f;  // Rotation speed of the light source in degree/frame

// Shaders
GLuint SunShader, LT_Shader, RT_Shader;

cameraSystem cam;  // the camera system

int currentX, currentY; // Global variables to store the current mouse position

GLuint earthTex,
moonTex,
saturnTex,
backgroundTex; // global variables for the textures

GLuint shuttle,
sphere,
cube;


// Render ring (Saturn)
void drawRing(float r)
{
    glBegin(GL_LINE_LOOP);
    glNormal3f(0, 1, 0);
    for (int i = 0; i<100; i++)
    {
        float arg = (float)PI*(float)i / 50.0f;
        glVertex3f(r*cos(arg), 0, r*sin(arg));
    }
    glEnd();
}

// Creates vertices and faces for a sphere and assigns each vertex a texture coordinate.
void drawSphere(float r, int slices, int stacks)
{
    float dTheta = 2.0f*(float)PI / (float)stacks;
    float dPhi = (float)PI / (float)slices;

    // Northpole:
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.5, 1.0);
    glNormal3f(0, 1, 0);
    glVertex3f(0, r, 0);
    for (int i = stacks; i >= 0; i--)
    {
        glTexCoord2f(1.0f - i*dTheta / (2.0f*(float)PI), ((float)PI - dPhi) / (float)PI);
        glNormal3f(sin(dPhi)*cos(i*dTheta), cos(dPhi), sin(dPhi)*sin(i*dTheta));
        glVertex3f(r*sin(dPhi)*cos(i*dTheta), r*cos(dPhi), r*sin(dPhi)*sin(i*dTheta));
    }
    glEnd();

    // Middle Part
    for (int j = 1; j<slices - 1; j++)
    {
        glBegin(GL_QUAD_STRIP);
        for (int i = stacks; i >= 0; i--)
        {
            glTexCoord2f(1.0f - i*dTheta / (2.0f*(float)PI), ((float)PI - j*dPhi) / (float)PI);
            glNormal3f(sin(j*dPhi)*cos(i*dTheta), cos(j*dPhi), sin(j*dPhi)*sin(i*dTheta));
            glVertex3f(r*sin(j*dPhi)*cos(i*dTheta), r*cos(j*dPhi), r*sin(j*dPhi)*sin(i*dTheta));
            glTexCoord2f(1.0f - i*dTheta / (2.0f*(float)PI), ((float)PI - (j + 1.0f)*dPhi) / (float)PI);
            glNormal3f(sin((j + 1)*dPhi)*cos(i*dTheta), cos((j + 1)*dPhi), sin((j + 1)*dPhi)*sin(i*dTheta));
            glVertex3f(r*sin((j + 1)*dPhi)*cos(i*dTheta), r*cos((j + 1)*dPhi), r*sin((j + 1)*dPhi)*sin(i*dTheta));
        }
        glEnd();
    }

    // Southpole:
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.5, 0);
    glNormal3f(0, -1, 0);
    glVertex3f(0, -r, 0);
    for (int i = 0; i <= stacks; i++)
    {
        glTexCoord2f(1 - i*dTheta / (2.0f*(float)PI), ((float)PI - (slices - 1)*dPhi) / (float)PI);
        glNormal3f(sin((slices - 1.0f)*dPhi)*cos(i*dTheta), cos((slices - 1.0f)*dPhi), sin((slices - 1.0f)*dPhi)*sin(i*dTheta));
        glVertex3f(r*sin((slices - 1)*dPhi)*cos(i*dTheta), r*cos((slices - 1)*dPhi), r*sin((slices - 1)*dPhi)*sin(i*dTheta));
    }
    glEnd();

}

// Draws a textured box arround the scene.
void DrawQuad(float a)
{
    glBindTexture(GL_TEXTURE_2D, backgroundTex);

    // -Z
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);	glVertex3f(a, a, -a);
    glTexCoord2f(0, 1);	glVertex3f(-a, a, -a);
    glTexCoord2f(1, 1);	glVertex3f(-a, -a, -a);
    glTexCoord2f(1, 0);	glVertex3f(a, -a, -a);

    // +Z
    glTexCoord2f(0, 0);	glVertex3f(a, a, a);
    glTexCoord2f(0, 1);	glVertex3f(a, -a, a);
    glTexCoord2f(1, 1);	glVertex3f(-a, -a, a);
    glTexCoord2f(1, 0);	glVertex3f(-a, a, a);


    // +X
    glTexCoord2f(0, 0);	glVertex3f(a, a, -a);
    glTexCoord2f(0, 1);	glVertex3f(a, -a, -a);
    glTexCoord2f(1, 1);	glVertex3f(a, -a, a);
    glTexCoord2f(1, 0);	glVertex3f(a, a, a);

    // -X
    glTexCoord2f(0, 0);	glVertex3f(-a, a, -a);
    glTexCoord2f(0, 1);	glVertex3f(-a, a, a);
    glTexCoord2f(1, 1);	glVertex3f(-a, -a, a);
    glTexCoord2f(1, 0);	glVertex3f(-a, -a, -a);

    // +Y
    glTexCoord2f(0, 0);	glVertex3f(-a, a, -a);
    glTexCoord2f(0, 1);	glVertex3f(a, a, -a);
    glTexCoord2f(1, 1);	glVertex3f(a, a, a);
    glTexCoord2f(1, 0);	glVertex3f(-a, a, a);

    // +Y
    glTexCoord2f(0, 0);	glVertex3f(-a, -a, -a);
    glTexCoord2f(0, 1);	glVertex3f(-a, -a, a);
    glTexCoord2f(1, 1);	glVertex3f(a, -a, a);
    glTexCoord2f(1, 0);	glVertex3f(a, -a, -a);

    glEnd();
}

void initTexture(GLint name, GLint w, GLint h, GLubyte *data)
{
    glBindTexture(GL_TEXTURE_2D, name);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void initGL()
{

    glClearColor(0, 0, 0, 0);   // set the clear color to black
    glEnable(GL_DEPTH_TEST); // turn on the depth test
    glEnable(GL_CULL_FACE);  // turn on backface culling

    // Direction of the directional light source D
    D_light_direction[0] = 0;
    D_light_direction[1] = 0;
    D_light_direction[2] = -1;
    D_light_direction[3] = 0;

    // Inital poition of the point light P
    P_light_position[0] = 0;
    P_light_position[1] = 0;
    P_light_position[2] = 0;
    P_light_position[3] = 1;

    glLightfv(GL_LIGHT1, GL_POSITION, D_light_direction);
    glLightfv(GL_LIGHT0, GL_POSITION, P_light_position);

    // Setup camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cam.position.x, cam.position.y, cam.position.z, cam.position.x + cam.viewDir.x, cam.position.y + cam.viewDir.y, cam.position.z + cam.viewDir.z, cam.upDir.x, cam.upDir.y, cam.upDir.z);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    // Setup sun and lightshader
    for (unsigned i = 0; i< cryptedFS.length(); i++) cryptedFS[i] -= 1;
    for (unsigned i = 0; i< cryptedVS.length(); i++) cryptedVS[i] -= 1;

    string LT_FS(718, '-');
    for (int i = 0; i<718; i++) LT_FS[i] = (char)FSint[i];
    string LT_VS(583, '-');
    for (int i = 0; i<583; i++) LT_VS[i] = (char)VSint[i];

    const string VS = cryptedVS;
    const string FS = cryptedFS;
    const string VS2 = LT_VS;
    const string FS2 = LT_FS;
    createProgram_VF_string(VS, FS, &SunShader);
    createProgram_VF_string(LT_VS, LT_FS, &LT_Shader);

    createProgram_VF("RT_VS.glsl", "RT_FS.glsl", &RT_Shader);
    if (RT_Shader == 0xffffffff)
        exit(1);

    // Load, init and bind the textures:
    QImage earth, moon, saturn, back;
    earth.load("./data/earth.jpg");
    earth = QGLWidget::convertToGLFormat(earth);
    glGenTextures(1, &earthTex);
    initTexture(earthTex, earth.width(), earth.height(), earth.bits());

    moon.load("./data/moon.jpg");
    moon = QGLWidget::convertToGLFormat(moon);
    glGenTextures(1, &moonTex);
    initTexture(moonTex, moon.width(), moon.height(), moon.bits());

    saturn.load("./data/saturn.jpg");
    saturn = QGLWidget::convertToGLFormat(saturn);
    glGenTextures(1, &saturnTex);
    initTexture(saturnTex, saturn.width(), saturn.height(), saturn.bits());

    back.load("./data/background.jpg");
    back = QGLWidget::convertToGLFormat(back);
    initTexture(backgroundTex, back.width(), back.height(), back.bits());

    GLint texLoc;
    texLoc = glGetUniformLocation(LT_Shader, "Texture");
    glUniform1i(texLoc, 0);
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0, (GLfloat)w / (GLfloat)h, 10, 1200.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cam.position.x, cam.position.y, cam.position.z,
              cam.position.x + cam.viewDir.x,
              cam.position.y + cam.viewDir.y,
              cam.position.z + cam.viewDir.z,
              cam.upDir.x,
              cam.upDir.y,
              cam.upDir.z);

}

void onIdle()
{
    t += speed;  // increase the time parameter

    GLint location;
    glUseProgram(SunShader);
    location = glGetUniformLocation(SunShader, "Time");
    glUniform1f(location, t*10.0f);

    glutPostRedisplay();
}


void updateCamera()
{

    glLoadIdentity();
    gluLookAt(cam.position.x, cam.position.y, cam.position.z, cam.position.x + cam.viewDir.x, cam.position.y + cam.viewDir.y, cam.position.z + cam.viewDir.z, cam.upDir.x, cam.upDir.y, cam.upDir.z);
    glutPostRedisplay();

}


void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLightfv(GL_LIGHT0, GL_POSITION, P_light_position);
    glLightfv(GL_LIGHT1, GL_POSITION, D_light_direction);

    if (raytracing)
    {
        glPushMatrix();
#define NUM_SPHERES 4
        int sphereIdx = 0;
        static float sphereData[NUM_SPHERES * 4];

        glUseProgram(RT_Shader);

        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        // Setup uniforms for the raytracer
        glUniform2f(glGetUniformLocation(RT_Shader, "uResolution"), (float)viewport[2], (float)viewport[3]);
        glUniform1f(glGetUniformLocation(RT_Shader, "uFov"), 70.0f);
        glUniform3f(glGetUniformLocation(RT_Shader, "uUp"), cam.upDir.x, cam.upDir.y, cam.upDir.z);
        glUniform3f(glGetUniformLocation(RT_Shader, "uDirection"), cam.viewDir.x, cam.viewDir.y, cam.viewDir.z);
        glUniform3f(glGetUniformLocation(RT_Shader, "uPosition"), cam.position.x, cam.position.y, cam.position.z);

        // Abuse the OpenGL matrix stack in order to retrive
        glLoadIdentity();
        glRotatef(50, 0, 1, 0);
        static float M[16];

        // SUN
        glGetFloatv(GL_MODELVIEW_MATRIX, M);
        for (int i = 0; i < 3; ++i)
            sphereData[sphereIdx * 4 + i] = M[12 + i];
        sphereData[sphereIdx * 4 + 3] = 25.0f;
        sphereIdx++;

        glPushMatrix();
        glPushMatrix();
        // EARTH
        glRotatef(t, 0, 1, 0);
        glTranslatef(50, 0, 0);
        glGetFloatv(GL_MODELVIEW_MATRIX, M);
        for (int i = 0; i < 3; ++i)
            sphereData[sphereIdx * 4 + i] = M[12 + i];
        sphereData[sphereIdx * 4 + 3] = 12.0f;
        sphereIdx++;

        // MOON
        glRotatef(t*2.0f, 1.0f, 0, 0);
        glTranslatef(0, 20, 0);
        glGetFloatv(GL_MODELVIEW_MATRIX, M);
        for (int i = 0; i < 3; ++i)
            sphereData[sphereIdx * 4 + i] = M[12 + i];
        sphereData[sphereIdx * 4 + 3] = 6.0f;
        sphereIdx++;
        glPopMatrix();

        // SATURN
        glRotatef(t / 2.0f, 0, 1, 0);
        glTranslatef(100.0f, 0, 0);
        glRotatef(30.0f, 0, 0, 1);
        glGetFloatv(GL_MODELVIEW_MATRIX, M);
        for (int i = 0; i < 3; ++i)
            sphereData[sphereIdx * 4 + i] = M[12 + i];
        sphereData[sphereIdx * 4 + 3] = 16.0f;
        sphereIdx++;
        glPopMatrix();

        glUniform4fv(glGetUniformLocation(RT_Shader, "uSphereData"), NUM_SPHERES, sphereData);

        // Render fullscreen quad
        glRecti(-1, -1, 1, 1);

        glPopMatrix();
    }
    else
    {
        // Rendering using rasterization
        // SUN
        glUseProgram(SunShader);
        glutSolidSphere(25, 150, 80);

        glUseProgram(LT_Shader);
        glColor4f(0, 0, 0, 0.5);
        DrawQuad(300);

        glPushMatrix();
        glRotatef(50, 0, 1, 0);

        glPushMatrix();
        // EARTH
        glBindTexture(GL_TEXTURE_2D, earthTex);
        glColor4f(0, 0, 0, 0);
        glRotatef(t, 0, 1, 0);
        glTranslatef(50, 0, 0);
        drawSphere(12, 20, 40);

        // MOON
        glBindTexture(GL_TEXTURE_2D, moonTex);
        glRotatef(t*2.0f, 1.0f, 0, 0);
        glTranslatef(0, 20, 0);
        drawSphere(6, 15, 30);
        glPopMatrix();

        // SATURN
        glRotatef(t / 2.0f, 0, 1, 0);
        glTranslatef(100.0f, 0, 0);
        glRotatef(30.0f, 0, 0, 1);
        glBindTexture(GL_TEXTURE_2D, saturnTex);
        drawSphere(16, 25, 50);
        glLineWidth(1);
        for (int i = 0; i < 20; i++)
        {
            glColor4f(0.713f - (i % 4)*0.1f, 0.623f - (i % 4)*0.1f, 0.568f - (i % 4)*0.1f, 1.0f);
            drawRing(22.0f + i / 2.0f);
        }
        glPopMatrix();
    }
    glutSwapBuffers();
}

void cleanUp()
{
    glDeleteTextures(1, &earthTex);
    glDeleteTextures(1, &moonTex);
    glDeleteTextures(1, &saturnTex);
    glDeleteTextures(1, &backgroundTex);

    glDeleteProgram(LT_Shader);
    glDeleteProgram(SunShader);
}

void onMouseDown(int button, int state, int x, int y)
{
    currentX = x;
    currentY = y;
}


void onMouseMove(int x, int y)
{
    float dx = (float)(currentX - x);
    float dy = (float)(currentY - y);

    cam.yaw(dx);
    cam.pitch(dy);

    currentX = x;
    currentY = y;

    updateCamera();
}


void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        cleanUp();
        exit(1);
        break;
    case 'w': // move forward
        cam.moveForward(1);
        updateCamera();
        break;
    case 's': // move forward
        cam.moveBackward(1);
        updateCamera();
        break;
    case 'a':
        cam.roll(-1.0f);
        updateCamera();
        break;
    case 'd':
        cam.roll(1.0f);
        updateCamera();
        break;
    case 'r':
        raytracing = !raytracing; // Toggle rt
        break;
    case 'n': // Reload rt shader
    {
                  GLuint tmpShader;
                  createProgram_VF("RT_VS.glsl", "RT_FS.glsl", &tmpShader);
                  if (tmpShader != 0xffffffff)
                  {
                      glDeleteProgram(RT_Shader);
                      RT_Shader = tmpShader;
                      cout << "Shader reload done!" << endl;
                  }
                  else
                  {
                      cout << "Shader reload failed - using old shader." << endl;
                  }
    }
        break;



    case 'p': // toggle polygon mode:
        wireframe_mode = !wireframe_mode;
        if (wireframe_mode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;


        // increase / decrease the speed of the point light P:
    case '+':
        speed += 0.01f;
        break;
    case '-':
        speed -= 0.01f;
        break;
    }

    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(1042, 1024);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutCreateWindow("Planet System - Raytracer");

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
