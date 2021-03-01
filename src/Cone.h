#pragma once
#include "Shape.h"
class Cone : public Shape {
public:
    Cone(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    void SetIndexBuffer();
    void Bind();
    const unsigned short GetIndicies() const;
private:

    const std::vector<Vertex> m_Vertices = {

		{0.0f,1.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f}, //0

		 {0.5f, -0.5f, 0.0f,1.0f,1.0f,1.0f,0.0f,0.0f}, //1

		  {0.3f, -0.5f, -0.3f,1.0f,1.0f,1.0f,0.0f,0.0f},
		 {0.0f, -0.5f, -0.5f,1.0f,1.0f,1.0f,0.0f,0.0f},
		  {-0.3f, -0.5f, -0.3f,1.0f,1.0f,1.0f,0.0f,0.0f},
		 {-0.5f, -0.5f, 0.0f,1.0f,1.0f,1.0f,0.0f,0.0f},
		  {-0.3f, -0.5f, 0.3f,1.0f,1.0f,1.0f,0.0f,0.0f},
		 {0.0f, -0.5f, 0.5f,1.0f,1.0f,1.0f,0.0f,0.0f},
		  {0.3f, -0.5f, 0.3f,1.0f,1.0f,1.0f,0.0f,0.0f},
        
    };
    const std::vector<unsigned short> m_indices = {
       1,2,0,
       2,3,0,
       3,4,0,
       4,5,0,
       5,6,0,
       6,7,0,
       7,8,0,
       8,1,0
        
       
    };
};