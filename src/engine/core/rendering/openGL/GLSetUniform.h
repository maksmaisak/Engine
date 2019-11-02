//
// Created by Maksym Maisak on 2019-01-09.
//

#ifndef ENGINE_GLSETUNIFORM_H
#define ENGINE_GLSETUNIFORM_H

#include "glm.h"
#include <GL/glew.h>
#include "Texture.h"

/// A wrapper around some of the glUniform* functions.
namespace gl {

    template<typename T>
    void setUniform(GLint location, const T& value);

    void setUniform(GLint location, const class TextureObject&, GLenum textureNum = 0, GLenum target = GL_TEXTURE_2D);
    void setUniform(GLint location, const en::Texture* texture, GLenum textureNum = 0, GLenum target = GL_TEXTURE_2D);
}

#endif //ENGINE_GLSETUNIFORM_H
