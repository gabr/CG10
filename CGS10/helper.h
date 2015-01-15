#ifndef HELPER_H
#define HELPER_H 

#include <GL/glew.h> 
#include <GL/glut.h>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <math.h>

using namespace std;

// global consts
const double PI = 3.14159265359;

#include <math.h>

//***************************************************
// Vector class representing a 3D vector
//***************************************************

class vec3 {
  
  public:
    float x,y,z;
    
    vec3(float x, float y, float z) {
      this->x = x;
      this->y = y;
      this->z = z;
    }
    vec3() {
      x = 0.0;
      y = 0.0;
      z = 0.0;
    }
    inline float length() {
      return sqrt(x*x+y*y+z*z);
    }	
    
    inline void normalize() {
      
      float l=this->length();
      
      x /= l;
      y /= l;
      z /= l;
    }
    
    inline vec3 operator+(vec3 B) {
      
      vec3 C;
      C.x = x+B.x;
      C.y = y+B.y;
      C.z = z+B.z;
      return C;
    }	
    
    inline vec3 operator-(vec3 B) {
      
      vec3 C;
      C.x = x-B.x;
      C.y = y-B.y;
      C.z = z-B.z;
      return C;
    }	
    inline vec3 operator*(float a) {
      vec3 C;
      C.x = a*x;
      C.y = a*y;
      C.z = a*z;
      return C;
    }
    inline vec3 cross(vec3 B) {
      
      vec3 C;
      C.x = y*B.z - z*B.y;
      C.y = z*B.x - x*B.z;
      C.z = x*B.y - y*B.x;
      
      
      return C;
      
    }
};

//*******************************************************************
// A very small matrix class which represents a 3x3 rotation matrix
//*******************************************************************

class rotMat3x3 {
 
  public:
  
  float entries[3][3];
  
    //! default contructor creates identity matrix:
    rotMat3x3() {
      
      entries[0][0] = 1.0;
      entries[0][1] = 0.0;
      entries[0][2] = 0.0;
      entries[1][0] = 0.0;
      entries[1][1] = 1.0;
      entries[1][2] = 0.0;
      entries[2][0] = 0.0;
      entries[2][1] = 0.0;
      entries[2][2] = 1.0;    
    }
  
  //! contructor which creates a rotation matrix from an angle and an axis (rodrigues formula):
    rotMat3x3(vec3 axis, float angle) {
      
      float c = cos(angle*PI/180.0);
      float s = sin(angle*PI/180.0);
      
      entries[0][0] = c+ axis.x*axis.x*(1-c);
      entries[0][1] = axis.x*axis.y*(1-c)-axis.z*s;
      entries[0][2] = axis.y*s+axis.x*axis.z*(1-c);
      entries[1][0] = axis.z*s+axis.x*axis.y*(1-c);
      entries[1][1] = c+ axis.y*axis.y*(1-c);
      entries[1][2] = -axis.x*s+axis.y*axis.z*(1-c);
      entries[2][0] = -axis.y*s+axis.x*axis.z*(1-c);
      entries[2][1] = axis.x*s+axis.y*axis.z*(1-c);
      entries[2][2] = c+ axis.z*axis.z*(1-c); 
      
    }
  
};

//**********************************
// Inline-functions for general use
//**********************************

//! Matrix-Vector Multiplication:
inline vec3 operator*(rotMat3x3 A, vec3 b) {
 
  vec3 res;
  res.x = b.x*A.entries[0][0] + b.y*A.entries[0][1] + b.z*A.entries[0][2];
  res.y = b.x*A.entries[1][0] + b.y*A.entries[1][1] + b.z*A.entries[1][2];
  res.z = b.x*A.entries[2][0] + b.y*A.entries[2][1] + b.z*A.entries[2][2];
  
  return res;
}

//! The multiplication operator that allows the scalar value to preceed the vector.
inline vec3 operator*(float a, vec3 b)
{ return b*a; }


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



// this function creates Shader Program which consists of both, a vertex and a fragment shader

  void createProgram_VF(const char *VSfile, const char *FSfile, GLuint *handle) {
    
    GLint compiled;
    char infoLog[4096];
    int infoLogLength;
    
    filebuf *pbuf_VS,*pbuf_FS;
    ifstream filestr_VS,filestr_FS;
    int size_VS,size_FS;
    char *buffer_VS,*buffer_FS;
    
    filestr_VS.open(VSfile);
    filestr_FS.open(FSfile);
    
    
    
    // get pointer to associated buffer object
    pbuf_VS=filestr_VS.rdbuf();
    pbuf_FS=filestr_FS.rdbuf();
    size_VS=pbuf_VS->pubseekoff (0,ios::end,ios::in);
    size_FS=pbuf_FS->pubseekoff(0,ios::end,ios::in);
    
    pbuf_VS->pubseekpos (0,ios::in);
    pbuf_FS->pubseekpos (0,ios::in);
  
    // allocate memory to contain file data
    buffer_VS=new char[size_VS];
    buffer_FS=new char[size_FS];
    pbuf_VS->sgetn(buffer_VS,size_VS);
    pbuf_FS->sgetn(buffer_FS,size_FS);
    
    const char *VshaderCode = buffer_VS;
    const char *FshaderCode = buffer_FS;
    
    filestr_VS.close();
    filestr_FS.close();
    
    //compile vertex shader: 	
    GLuint Vshader= glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(Vshader,1,&VshaderCode,&size_VS);
    glCompileShader(Vshader);
    glGetShaderiv(Vshader,GL_COMPILE_STATUS, &compiled);
    if ( !compiled) {
      // Print out the info log
      glGetShaderInfoLog(Vshader, sizeof(infoLog), &infoLogLength, infoLog);
      if(infoLogLength > 0)
      {
	printf("CompileShader() infoLog for Vertex Shader %s \n%s\n",VSfile, infoLog);
	exit(1);
      }
    }	

  //compile Fragment shader: 	
    GLuint Fshader= glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(Fshader,1,&FshaderCode,&size_FS);
    glCompileShader(Fshader);
    glGetShaderiv(Fshader,GL_COMPILE_STATUS, &compiled);
    if ( !compiled) {
      // Print out the info log
      glGetShaderInfoLog(Fshader, sizeof(infoLog), &infoLogLength, infoLog);
      if(infoLogLength > 0)
      {
	printf("CompileShader() infoLog for Fragment Shader %s\n%s\n", FSfile, infoLog);
	exit(1);
      }
    }	

    *handle = glCreateProgram();
    glAttachShader(*handle, Vshader);
    glAttachShader(*handle, Fshader);
    
    
    glDeleteShader(Vshader);
    glDeleteShader(Fshader);
    delete buffer_VS;
    delete buffer_FS;
    glLinkProgram(*handle);
  }

// this function creates Shader Program which consists only of a vertex shader

  void createProgram_V(const char *VSfile, GLuint *handle) {
    
    GLint compiled;
    char infoLog[4096];
    int infoLogLength;
    
    filebuf *pbuf;
    ifstream filestr;
    int size;
    char *buffer;
    
    filestr.open(VSfile);
    
    // get pointer to associated buffer object
    pbuf=filestr.rdbuf();
    size=pbuf->pubseekoff (0,ios::end,ios::in);
    pbuf->pubseekpos (0,ios::in);
    
    // allocate memory to contain file data
    buffer=new char[size];
    pbuf->sgetn(buffer,size);
    
    const char *VshaderCode = buffer;
    
    filestr.close();
    
    //compile vertex shader: 	
    GLuint Vshader= glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(Vshader,1,&VshaderCode,&size);
    glCompileShader(Vshader);
    glGetShaderiv(Vshader,GL_COMPILE_STATUS, &compiled);
    if ( !compiled) {
      // Print out the info log
      glGetShaderInfoLog(Vshader, sizeof(infoLog), &infoLogLength, infoLog);
      if(infoLogLength > 0)
      {
	printf("CompileShader() infoLog %s \n%s\n",VSfile, infoLog);
	exit(1);
      }
    }	
    
    *handle = glCreateProgram();
    glAttachShader(*handle, Vshader);
    
    glDeleteShader(Vshader);
    delete buffer;
    glLinkProgram(*handle);
  }

#endif