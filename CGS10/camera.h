/// Implement the camera system entirely here, which means: don't do only the declaration here and 
/// create another file camera.cpp for the implementation. You might have been told that it is 
/// bad programming style to put everything in a .h file, but it's perfectly fine for such a small 
/// class! Plus, I don't want to open two files to read your solutions of this class! 

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>


class cameraSystem {
  
  public:
  
    glm::vec4 position;   // position-vector
    
    // orthonormal system of directions:
    glm::vec4 viewDir;    // viewing direction
    glm::vec4 upDir;      // up-vector
    glm::vec4 rightDir;   // right-vector (cross product of viewing- and up-direction)
  
  
    // initialize the system with values suitable for the underlying scene.
    cameraSystem() {
      position	= glm::vec4(50,80,-160,1);  
      viewDir	= glm::vec4(-0.240947, -0.428651, 0.870756,1);
      upDir		= glm::vec4(-0.0848183, 0.903053, 0.421079,1);
      rightDir	= glm::vec4(-0.96683, 0.0276017, -0.253944,1);
 
    }

    glm::mat4 getView()
    {
        return glm::lookAt(glm::vec3(position), glm::vec3(position + viewDir), glm::vec3(upDir));
    }
   
// move the system forward along the viewing direction   
   void moveForward(float delta) {
     position = position + (delta*viewDir);
   }
   
 //  
    void moveBackward(float delta) {
      position = position - (viewDir*delta);
   }

 
   void yaw(float angle) {
	glm::mat4 R = glm::rotate(angle, glm::vec3(upDir)); 

	viewDir = R*viewDir;
	rightDir = R*rightDir;
   }
   
   void pitch(float angle) {
		glm::mat4 R = glm::rotate(angle, glm::vec3(rightDir)); 
    
		viewDir = R*viewDir;
		upDir = R*upDir;
   }


   void roll(float angle) {
	   glm::mat4 R = glm::rotate(angle, glm::vec3(viewDir)); 
    
     rightDir = R*rightDir;
     upDir = R*upDir;
   }
   
};

