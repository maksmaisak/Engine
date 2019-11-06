//
// Created by Maksym Maisak on 2019-09-24.
//

#ifndef ENGINE_GLWRAPPER_H
#define ENGINE_GLWRAPPER_H

#include <GL/glew.h>
#include <cassert>
#include "GLHelpers.h"
#include "glm.h"

namespace gl {

    inline static struct {} ForceCreate;

    /// An abstract RAII wrapper around an openGL object.
    /// Destroys the object in its destructor.
    template<auto Create, auto Delete>
    class Wrapper {

    public:
        inline Wrapper() : m_id(0) {}
        inline explicit Wrapper(decltype(ForceCreate)) : m_id(Create()) {}
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
};

#endif //ENGINE_GLWRAPPER_H
