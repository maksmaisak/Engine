//
// Created by Maksym Maisak on 2/11/19.
//

#ifndef ENGINE_RENDERBUFFEROBJECT_H
#define ENGINE_RENDERBUFFEROBJECT_H

#include "GLWrapper.h"

namespace gl {

    GLuint makeRBO();
    void deleteRBO(GLuint id);

    class RenderbufferObject : public Wrapper<makeRBO, deleteRBO> {
    public:
        using Wrapper::Wrapper;
        void bind(GLenum target = GL_RENDERBUFFER) const;
        void unbind(GLenum target = GL_RENDERBUFFER) const;
    };
}


#endif //ENGINE_RENDERBUFFEROBJECT_H
