#pragma once
#include "Shape.h"

class Cube : public Shape {
public:
	Cube(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    void SetIndexBuffer();
    void Bind();
    const unsigned short GetIndicies() const;
private:
    
    const std::vector<Vertex> m_Vertices = {
        //front
        {-0.5f, -0.5f, -0.5f, 1.0f,1.0f,1.0f,  0.0f, 0.0f}, //bottom left
        {0.5f, -0.5f, -0.5f ,1.0f,1.0f,1.0f,1.0f, 0.0f}, //bottom right
        {0.5f, 0.5f, -0.5f,1.0f,1.0f,1.0f, 1.0f, 1.0f }, //top right
        {-0.5f, 0.5f,-0.5f,1.0f,1.0f,1.0f, 0.0f, 1.0f}, //top left

        //back
        {-0.5f, -0.5f, 0.5f, 1.0f,1.0f,1.0f, 0.0f, 0.0f}, //bottom left
        {0.5f, -0.5f, 0.5f, 1.0f,1.0f,1.0f, 1.0f, 0.0f}, //bottom right
        {0.5f, 0.5f, 0.5f,1.0f,1.0f,1.0f, 1.0f, 1.0f }, //top right
        {-0.5f, 0.5f,0.5f, 1.0f,1.0f,1.0f, 0.0f, 1.0f}, //top left

        //right
        {0.5f, -0.5f, -0.5f,1.0f,1.0f,1.0f, 0.0f, 0.0f}, //bottom left
        {0.5f, -0.5f, 0.5f,1.0f,1.0f,1.0f, 1.0f, 0.0f}, //bottom right
        {0.5f, 0.5f, 0.5f,1.0f,1.0f,1.0f, 1.0f, 1.0f }, //top right
        {0.5f, 0.5f, -0.5f, 1.0f,1.0f,1.0f, 0.0f, 1.0f }, //top left

        //left
        {-0.5f, -0.5f, 0.5f,1.0f,1.0f,1.0f,  0.0f, 0.0f}, //bottom left
        {-0.5f, -0.5f, -0.5f,1.0f,1.0f,1.0f, 1.0f, 0.0f}, //bottom right
        {-0.5f, 0.5f, -0.5f, 1.0f,1.0f,1.0f, 1.0f, 1.0f}, //top right
        {-0.5f, 0.5f, 0.5f, 1.0f,1.0f,1.0f,  0.0f, 1.0f}, //top left

        //top
        {-0.5f, 0.5f, -0.5f, 1.0f,1.0f,1.0f, 0.0f, 0.0f}, //bottom left
        {0.5f, 0.5f,  -0.5f,1.0f,1.0f,1.0f,  1.0f, 0.0f }, //bottom right
        {0.5f, 0.5f, 0.5f,  1.0f,1.0f,1.0f,  1.0f, 1.0f }, //top right
        {-0.5f, 0.5f,0.5f, 1.0f,1.0f,1.0f,  0.0f, 1.0f}, //top left

        //bottom
         {-0.5f, -0.5f, 0.5f,1.0f,1.0f,1.0f,  0.0f, 0.0f}, //bottom left
        {0.5f, -0.5f, 0.5f,  1.0f,1.0f,1.0f,  1.0f, 0.0f}, //bottom right
        {0.5f, -0.5f, -0.5f, 1.0f,1.0f,1.0f,  1.0f, 1.0f}, //top right
         {-0.5f, -0.5f, -0.5f,1.0f,1.0f,1.0f, 0.0f, 1.0f},  //top left
    };
	const std::vector<unsigned short> m_indices = {
        //front face
        0,1,2,
        0,2,3,
        //back face
        4,5,6,
        4,6,7,
        //right face
        8,9,10,
        8,10,11,
        //left face
        12,13,14,
        12,14,15,
        //top
        16,17,18,
        16,18,19,
        //bottom
        20,21,22,
        20,22,23
    };

};