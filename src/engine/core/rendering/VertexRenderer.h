//
// Created by Maksym Maisak on 2019-02-13.
//

#ifndef ENGINE_VERTEXRENDERER_H
#define ENGINE_VERTEXRENDERER_H

#include <GL/glew.h>
#include "glm.h"
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


#endif //ENGINE_VERTEXRENDERER_H
