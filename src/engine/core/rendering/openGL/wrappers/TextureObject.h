//
// Created by Maksym Maisak on 5/10/19.
//

#ifndef ENGINE_TEXTUREOBJECT_H
#define ENGINE_TEXTUREOBJECT_H

#include "GLWrapper.h"

namespace gl {

    GLuint createTexture();
    void deleteTexture(GLuint id);

    class TextureObject : public Wrapper<createTexture, deleteTexture> {
    public:
        using Wrapper::Wrapper;
        void bind(GLenum target) const;
        void unbind(GLenum target) const;
    };
}

#endif //ENGINE_TEXTUREOBJECT_H
