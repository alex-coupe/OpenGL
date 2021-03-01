#pragma once
#include "Shape.h"

class Prism : public Shape {
public:
	Prism(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	void SetIndexBuffer();
	void Bind();
private:
    const std::vector<Vertex> m_Vertices = {

       {-0.5f, -0.5f, -0.5f, 1.0f,1.0f,1.0f,  1.0f, 0.0f},
       {0.5f, -0.5f, -0.5f ,1.0f,1.0f,1.0f,0.0f, 0.0f},
       {0.0f, 0.5f, -0.5f,1.0f,1.0f,1.0f, 0.5f, 1.0f },

        {-0.5f, -0.5f, 0.5f, 1.0f,1.0f,1.0f,  1.0f, 0.0f},
       {0.5f, -0.5f, 0.5f ,1.0f,1.0f,1.0f,0.0f, 0.0f},
       {0.0f, 0.5f, 0.5f,1.0f,1.0f,1.0f, 0.5f, 1.0f },

        {0.5f, -0.5f, -0.5f ,1.0f,1.0f,1.0f,0.0f, 0.0f},
        {0.5f, -0.5f, 0.5f ,1.0f,1.0f,1.0f,1.0f, 0.0f},
        {0.0f, 0.5f, 0.5f,1.0f,1.0f,1.0f, 1.0f, 1.0f },
        {0.0f, 0.5f, -0.5f,1.0f,1.0f,1.0f, 0.0f, 1.0f },

       {-0.5f, -0.5f, 0.5f, 1.0f,1.0f,1.0f,  0.0f, 0.0f},
        {-0.5f, -0.5f, -0.5f, 1.0f,1.0f,1.0f,  1.0f, 0.0f},
          {0.0f, 0.5f, -0.5f,1.0f,1.0f,1.0f, 1.0f, 1.0f },
          {0.0f, 0.5f, 0.5f,1.0f,1.0f,1.0f, 0.0f, 1.0f },

          {0.5f, -0.5f, -0.5f ,1.0f,1.0f,1.0f,0.0f, 0.0f},
            {0.5f, -0.5f, 0.5f ,1.0f,1.0f,1.0f,1.0f, 0.0f},
         {-0.5f, -0.5f, 0.5f, 1.0f,1.0f,1.0f,  1.0f, 1.0f},
          {-0.5f, -0.5f, -0.5f, 1.0f,1.0f,1.0f,  0.0f, 1.0f}

    };

    const std::vector<unsigned short> m_indices = {

        0,1,2,
        3,4,5,
        6,7,8,
        6,8,9,
        10,11,12,
        10,12,13,
        14,15,16,
        14,16,17

    };
};