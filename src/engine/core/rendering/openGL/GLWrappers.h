//
// Created by Maksym Maisak on 2019-09-24.
//

#ifndef ENGINE_GLWRAPPERS_H
#define ENGINE_GLWRAPPERS_H

#include <GL/glew.h>
#include <assert.h>
#include "GLHelpers.h"
#include "glm.h"

namespace gl {

    struct CreateImmediately {};

    template<auto Create, auto Delete>
    class Wrapper {

    public:
        inline Wrapper() : m_id(0) {}
        inline Wrapper(CreateImmediately) : m_id(Create()) {}
        inline ~Wrapper() {

            if (m_id) {
                glCheckError();
                Delete(m_id);
                glCheckError();
            }
        }
        Wrapper(const Wrapper& other) = delete;
        Wrapper& operator=(const Wrapper& other) = delete;
        inline Wrapper(Wrapper&& other) noexcept : m_id(std::exchange(other.m_id, 0)) {}
        inline Wrapper& operator=(Wrapper&& other) noexcept {

            if (m_id && &other != this) {
                glCheckError();
                Delete(m_id);
                glCheckError();
            }

            m_id = std::exchange(other.m_id, 0);
            return *this;
        }

        inline void create() {

            assert(m_id == 0);
            glCheckError();
            m_id = Create();
            glCheckError();
        }

        inline bool isValid() const {return m_id != 0;}
        inline operator bool() const {return isValid();}

        inline GLuint getId() const {return m_id;}
        inline operator GLuint() const {return getId();}

    protected:
        GLuint m_id;
    };


    GLuint createVBO();
    void deleteVBO(GLuint id);

    class VertexBufferObject : public Wrapper<createVBO, deleteVBO> {
    public:
        using Wrapper::Wrapper;
        void bind(GLenum target) const;
        void unbind(GLenum target) const;
    };


    GLuint createTexture();
    void deleteTexture(GLuint id);

    class TextureObject : public Wrapper<createTexture, deleteTexture> {
    public:
        using Wrapper::Wrapper;
        void bind(GLenum target) const;
        void unbind(GLenum target) const;
    };


    GLuint createVAO();
    void deleteVAO(GLuint id);

    class VertexArrayObject : public Wrapper<createVAO, deleteVAO> {
    public:
        using Wrapper::Wrapper;
        void bind() const;
        void unbind() const;
    };
};

#endif //ENGINE_GLWRAPPERS_H
