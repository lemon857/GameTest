#pragma once

#include "EngineCore/IGameObject.h"

#include <memory>
#include <string>

#include <glad/glad.h>

namespace RenderEngine
{
	class Material;
    class VertexBuffer;
}

class Cube : public IGameObject
{
public:
	Cube(std::shared_ptr<RenderEngine::Material> pMaterial);

    ~Cube() = default;

    Cube(Cube&) = delete;
    Cube(Cube&&) = delete;

    Cube& operator=(Cube&) = delete;
    Cube& operator=(Cube&&) = delete;
private:
    std::shared_ptr<RenderEngine::Material> m_pMaterial;

    static constexpr GLfloat m_vertexCoords[] {
        // FRONT
        -1.f, -1.f, -1.f,//1
        -1.f,  1.f, -1.f,//2
         1.f,  1.f,  -1.f,//3
         1.f, -1.f,  -1.f,//4
         // BACK
         1.f, -1.f, 1.f,//5
         1.f,  1.f, 1.f,//6
         -1.f,  1.f,  1.f,//7
         -1.f, -1.f,  1.f,//8
         // RIGHT
         1.f,  -1.f, -1.f,//9
          1.f,  1.f, -1.f,//10
          1.f,  1.f,  1.f,//11
         1.f,  -1.f,  1.f,//12
         // LEFT
         -1.f, -1.f, 1.f,//13
         -1.f,  1.f, 1.f,//14
         -1.f,  1.f, -1.f,//15
         -1.f, -1.f, -1.f,//16
         // TOP
         -1.f,  1.f,  -1.f,//17
         -1.f,  1.f,  1.f,//18
          1.f,  1.f,  1.f,//19
          1.f,  1.f,  -1.f,//20
          // BOTTOM
          1.f, -1.f, -1.f,//21
          1.f, -1.f, 1.f,//22
          -1.f, -1.f, 1.f,//23
          -1.f, -1.f, -1.f //24
    };

    static constexpr GLfloat m_normalCoords[] {
        // FRONT // 4
        0.f, 0.f, -1.f,
        0.f, 0.f, -1.f,
        0.f, 0.f, -1.f,
        0.f, 0.f, -1.f,
        // BACK // 3
        0.f, 0.f, 1.f,
        0.f, 0.f, 1.f,
        0.f, 0.f, 1.f,
        0.f, 0.f, 1.f,
        // RIGHT // 2
        1.f,  0.f, 0.f,
        1.f,  0.f, 0.f,
        1.f,  0.f, 0.f,
        1.f,  0.f, 0.f,
        // LEFT // 1
        -1.f, 0.f, 0.f,
        -1.f, 0.f, 0.f,
        -1.f, 0.f, 0.f,
        -1.f, 0.f, 0.f,
        // TOP // 6
        0.f, 1.f,  0.f,
        0.f, 1.f,  0.f,
        0.f, 1.f,  0.f,
        0.f, 1.f,  0.f,
        // BOTTOM // 5
        0.f, -1.f, 0.f,
        0.f, -1.f, 0.f,
        0.f, -1.f, 0.f,
        0.f, -1.f, 0.f
    };

    static constexpr GLfloat m_textureCoords[]{
        // FRONT
        1.f, 0.f,
        1.f, 1.f,
        0.f, 1.f,
        0.f, 0.f,
        // BACK
        1.f, 0.f,
        1.f, 1.f,
        0.f, 1.f,
        0.f, 0.f,
        // RIGHT
        1.f, 0.f,
        1.f, 1.f,
        0.f, 1.f,
        0.f, 0.f,
        // LEFT
        1.f, 0.f,
        1.f, 1.f,
        0.f, 1.f,
        0.f, 0.f,
        // TOP
        1.f, 0.f,
        1.f, 1.f,
        0.f, 1.f,
        0.f, 0.f,
        // BOTTOM
        1.f, 0.f,
        1.f, 1.f,
        0.f, 1.f,
        0.f, 0.f,
    };

    static constexpr GLuint m_indexes[] {
        //front
        0, 1, 2, 2, 3, 0,
        // back
        4, 5, 6, 6, 7, 4,
        // right
        8, 9, 10, 10, 11, 8,
        // left
        12, 13, 14, 14, 15, 12,
        // top
        16, 17, 18, 18, 19, 16,
        // bottom
        20, 21, 22, 22, 23, 20
    };
};