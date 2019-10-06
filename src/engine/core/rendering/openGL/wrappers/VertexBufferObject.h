//
// Created by Maksym Maisak on 5/10/19.
//

#ifndef ENGINE_VERTEXBUFFEROBJECT_H
#define ENGINE_VERTEXBUFFEROBJECT_H

#include "GLWrapper.h"

namespace gl {

    GLuint createVBO();
    void deleteVBO(GLuint id);

    class VertexBufferObject : public Wrapper<createVBO, deleteVBO> {
    public:
        using Wrapper::Wrapper;
        void bind(GLenum target) const;
        void unbind(GLenum target) const;
    };
}


#endif //ENGINE_VERTEXBUFFEROBJECT_H
