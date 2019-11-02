//
// Created by Maksym Maisak on 2/11/19.
//

#ifndef ENGINE_FRAMEBUFFERBUNDLE_H
#define ENGINE_FRAMEBUFFERBUNDLE_H

#include "FramebufferObject.h"
#include "TextureObject.h"
#include "RenderbufferObject.h"

namespace gl {

    struct FramebufferBundle {

        gl::FramebufferObject framebuffer;
        gl::TextureObject colorTexture;
        gl::RenderbufferObject depthRenderbuffer;
    };
}

#endif //ENGINE_FRAMEBUFFERBUNDLE_H
