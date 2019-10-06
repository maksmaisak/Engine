//
// Created by Maksym Maisak on 5/10/19.
//

#ifndef ENGINE_VERTEXARRAYOBJECT_H
#define ENGINE_VERTEXARRAYOBJECT_H

#include "GLWrapper.h"

namespace gl {

    GLuint createVAO();
    void deleteVAO(GLuint id);

    class VertexArrayObject : public Wrapper<createVAO, deleteVAO> {
    public:
        using Wrapper::Wrapper;
        void bind() const;
        void unbind() const;
    };
}

#endif //ENGINE_VERTEXARRAYOBJECT_H
