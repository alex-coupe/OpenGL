#pragma once
#include "Shape.h"

class Pyramid : public Shape {
public:
	Pyramid(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	void SetIndexBuffer();
	void Bind();
    const unsigned short GetIndicies() const;
private:
    const std::vector<Vertex> m_Vertices = {
       
        {-0.5f, -0.5f, -0.5f, 1.0f,1.0f,1.0f,  1.0f, 0.0f}, 
        {0.5f, -0.5f, -0.5f ,1.0f,1.0f,1.0f,0.0f, 0.0f}, 
        {0.0f, 0.5f, 0.0f,1.0f,1.0f,1.0f, 0.5f, 1.0f }, 

        {0.5f, -0.5f, -0.5f ,1.0f,1.0f,1.0f,1.0f, 0.0f},
        {0.5f, -0.5f, 0.5f, 1.0f,1.0f,1.0f,  0.0f, 0.0f},
        {0.0f, 0.5f, 0.0f,1.0f,1.0f,1.0f, 0.5f, 1.0f },

        {0.5f, -0.5f, 0.5f, 1.0f,1.0f,1.0f,  1.0f, 0.0f},
        {-0.5f, -0.5f, 0.5f, 1.0f,1.0f,1.0f,  0.0f, 0.0f},
        {0.0f, 0.5f, 0.0f,1.0f,1.0f,1.0f, 0.5f, 1.0f },

        {-0.5f, -0.5f, 0.5f, 1.0f,1.0f,1.0f,  1.0f, 0.0f},
        {-0.5f, -0.5f, -0.5f, 1.0f,1.0f,1.0f,  0.0f, 0.0f},
        {0.0f, 0.5f, 0.0f,1.0f,1.0f,1.0f, 0.5f, 1.0f },

         {-0.5f, -0.5f, 0.5f, 1.0f,1.0f,1.0f,  0.0f, 0.0f},
        {0.5f, -0.5f, 0.5f, 1.0f,1.0f,1.0f,  0.0f, 1.0f},
         {0.5f, -0.5f, -0.5f ,1.0f,1.0f,1.0f, 1.0f, 1.0f},
           {-0.5f, -0.5f, -0.5f, 1.0f,1.0f,1.0f,  1.0f, 0.0f}
    };

    const std::vector<unsigned short> m_indices = {
     
        0,1,2,
        3,4,5,
        6,7,8,
        9,10,11,
        12,13,14,
        12,14,15
        
    };
};