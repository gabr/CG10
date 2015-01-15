#include <fstream>
#include <vector>

#include <glm/glm.hpp>
#include "helper.h"

using namespace std;

// simple structure to hold three integer-values (used for triangle indicies)
struct tri {
  int A;
  int B;
  int C;
};


// Declaration:

class OffObject 
{

public: 
  	vector<glm::vec3> vertexList;
	vector<glm::vec3> normalsList;
	vector<tri> faceList;
	
	int noOfFaces;
	int noOfVertices;
	
	OffObject(string filename);
};



//TODO: IMPLEMENTATION:

// the constuctor recieves the filename (an .off file) and parses it. The vertices, normals and triangles
// are pushed back into the respective container whereas the NORMALS have to be explicitly computed for each
// vertex. 

OffObject::OffObject(string filename) {
 
	std::ifstream inFile(filename.c_str());
	char tmp[20];

	inFile >> tmp;
	inFile >> noOfVertices;
	inFile >> noOfFaces;
	inFile >> tmp;

	// Read Vertex Data and initialize the normals:	
    for (int i=0; i<noOfVertices; i++) 
	{
		glm::vec3 vertex;

        inFile >> vertex.x;
        inFile >> vertex.y;
        inFile >> vertex.z;

        // initalize the normal with (0,0,0)
		// add vertex and normal
        vertexList.push_back(vertex);
        normalsList.push_back(glm::vec3(0.0f));
    }


	// Read Triangle Data:
    tri T;
    glm::vec3 vertexCurrent;
    glm::vec3 vertexNext;

    for (int i=0; i<noOfFaces; i++) 
	{
        // TODO
        inFile >> tmp;
        inFile >> T.A;
        inFile >> T.B;
        inFile >> T.C;

        faceList.push_back(T);
        // probably helpful: glm::cross(..,..);	//CHECK DOCUMENTATION!!!

        int indices[] = { T.A, T.B, T.C };
        
        for (int i = 0; i < 3; i++)
        {
            vertexCurrent = vertexList[indices[i]];
            vertexNext = vertexList[indices[i] % 3];

            normalsList[indices[i]].x = normalsList[indices[i]].x
                + ((vertexCurrent.y - vertexNext.y) * (vertexCurrent.z + vertexNext.z));
            normalsList[indices[i]].y = normalsList[indices[i]].y
                + ((vertexCurrent.z - vertexNext.z) * (vertexCurrent.x + vertexNext.x));
            normalsList[indices[i]].z = normalsList[indices[i]].z
                + ((vertexCurrent.x - vertexNext.x) * (vertexCurrent.y + vertexNext.y));
        }
    }
    
    //normalize:
    for (int i=0; i<noOfVertices; i++) 
	{
        // TODO
        // probably helpful: glm::normalize(..);	//CHECK DOCUMENTATION!!!
        normalsList[i] = glm::normalize(normalsList[i]);
    }      


}

