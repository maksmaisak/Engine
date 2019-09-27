//
// Created by Maksym Maisak on 11/12/18.
//

#ifndef SAXION_Y2Q1_RENDERING_GLHELPERS_H
#define SAXION_Y2Q1_RENDERING_GLHELPERS_H

#include <GL/glew.h>
#include <iostream>
#include <string>

inline GLenum glCheckError_(const char *file, int line) {
    GLenum lastErrorCode = GL_NO_ERROR;
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        lastErrorCode = errorCode;

        std::string error;
        switch (errorCode) {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cerr << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return lastErrorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

#define checkBeforeAfter(e) glCheckError(); e; glCheckError();

#endif //SAXION_Y2Q1_RENDERING_GLHELPERS_H
