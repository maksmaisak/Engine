//
// Created by Maksym Maisak on 2019-02-13.
//

#ifndef SAXION_Y2Q2_RENDERING_VERTEXRENDERER_H
#define SAXION_Y2Q2_RENDERING_VERTEXRENDERER_H

#include <GL/glew.h>
#include "glm.hpp"
#include <vector>
#include "Vertex.h"

namespace en {

    /// Owns the resources for rendering vertex arrays. Use for text, sprites, etc.
    class VertexRenderer {

    public:
        explicit VertexRenderer(std::size_t maxNumVerticesPerDrawCall = 4096);

        void renderVertices(const std::vector<Vertex>& vertices);

    private:

        std::size_t m_maxNumVerticesPerDrawCall = 0;

        GLuint m_vao = 0;
        GLuint m_vbo = 0;
    };
}


#endif //SAXION_Y2Q2_RENDERING_VERTEXRENDERER_H
