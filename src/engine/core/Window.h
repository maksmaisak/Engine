//
// Created by Maksym Maisak on 9/11/19.
//

#ifndef ENGINE_WINDOW_H
#define ENGINE_WINDOW_H

#include "glm.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Bounds.h"

struct GLFWwindow;

namespace en {

    class Window {

        struct WindowEvent {GLFWwindow* window = nullptr;};
        struct MouseButtonEvent : public WindowEvent {int button = GLFW_MOUSE_BUTTON_LEFT;};

    public:
        struct MousePressed : public MouseButtonEvent {};
        struct MouseReleased : public MouseButtonEvent {};
        struct MouseScroll : public WindowEvent {double offsetX = 0.0; double offsetY = 0.0;};
        struct FramebufferSizeEvent : public WindowEvent {int width = 0; int height = 0;};

        Window();
        bool create(int width, int height, const std::string& title = "Engine", bool fullscreen = false);
        void makeCurrent();
        void setViewport();
        void setViewport(const Bounds2D& normalizedBounds);

        glm::u32vec2 getFramebufferSize() const;
        glm::u32vec2 getSize() const;
        float getAspectRatio() const;
        bool shouldClose() const;

        inline GLFWwindow* getUnderlyingWindow() const {return m_window;}
        inline bool isValid() const {return m_window;}

    private:
        void setGLFWCallbacks();

        mutable GLFWwindow* m_window;
    };
}

#endif //ENGINE_WINDOW_H
