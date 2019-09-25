//
// Created by Maksym Maisak on 2019-09-24.
//

#ifndef ENGINE_GLWRAPPERS_H
#define ENGINE_GLWRAPPERS_H

#include <GL/glew.h>
#include "glm.h"

namespace gl {

    GLuint createVBO();
    void deleteVBO(GLuint id);

    GLuint createVAO();
    void deleteVAO(GLuint id);

    GLuint createTexture();
    void deleteTexture(GLuint id);

    struct DeferCreation {};

    template<auto Create, auto Delete>
    class Wrapper {

    public:
        inline Wrapper() : m_id(Create()) {}
        inline Wrapper(DeferCreation) : m_id(0) {}
        inline ~Wrapper() {Delete(m_id);}
        Wrapper(Wrapper& other) = delete;
        Wrapper& operator=(Wrapper& other) = delete;
        inline Wrapper(Wrapper&& other) noexcept : m_id(std::exchange(other.m_id, 0)) {}
        inline Wrapper& operator=(Wrapper&& other) noexcept {

            if (m_id != 0 && &other != this) {
                Delete(m_id);
            }

            m_id = std::exchange(other.m_id, 0);
            return *this;
        };

        inline void create() {assert(m_id == 0); m_id = Create();}

        inline bool isValid() const {return m_id != 0;}
        inline operator bool() const {return isValid();}

        inline GLuint getId() const {return m_id;}
        inline operator GLuint() const {return getId();}

    protected:
        GLuint m_id;
    };

    class VertexBufferObject : public Wrapper<createVBO, deleteVBO> {
    public:
        using Wrapper::Wrapper;
        void bind(GLenum target);
        void unbind(GLenum target);
    };

    class TextureObject : public Wrapper<createTexture, deleteTexture> {
    public:
        using Wrapper::Wrapper;
        void bind(GLenum target);
        void unbind(GLenum target);
    };

    class VertexArrayObject : Wrapper<createVAO, deleteVAO> {
    public:
        using Wrapper::Wrapper;
        void bind();
        void unbind();
    };
};

#endif //ENGINE_GLWRAPPERS_H
