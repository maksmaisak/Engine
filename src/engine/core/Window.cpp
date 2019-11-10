//
// Created by Maksym Maisak on 9/11/19.
//

#include "Window.h"
#include "Receiver.h"

using namespace en;

namespace {

    template<typename TEvent, typename TFunction>
    inline void setCallback(TFunction&& setCallbackFunction, GLFWwindow* window) {

        setCallbackFunction(window, [](GLFWwindow* window, auto... args) {
            Receiver<TEvent>::broadcast({window, args...});
        });
    }
}

Window::Window() :
    m_window(nullptr)
{}

bool Window::create(int width, int height, const std::string& title, bool fullscreen) {

    if (!glfwInit()) {
        return false;
    }

    glfwSetErrorCallback([](int code, const char* description) {
        std::cerr << "GLFW error code " << code << ": " << description << std::endl;
    });

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_COCOA_GRAPHICS_SWITCHING, GLFW_FALSE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
#endif

    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    m_window = glfwCreateWindow(
        width, height,
        title.c_str(),
        fullscreen ? glfwGetPrimaryMonitor() : nullptr,
        nullptr
    );

    if (!m_window) {
        glfwTerminate();
        return false;
    }

    makeCurrent();

    setGLFWCallbacks();

    return true;
}

void Window::makeCurrent() {

    glfwMakeContextCurrent(m_window);
}

glm::u32vec2 Window::getFramebufferSize() const {

    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(m_window, &width, &height);
    return {static_cast<glm::u32vec2::value_type>(width), static_cast<glm::u32vec2::value_type>(height)};
}

glm::u32vec2 Window::getSize() const {

    int width = 0;
    int height = 0;
    glfwGetWindowSize(m_window, &width, &height);
    return {static_cast<glm::u32vec2::value_type>(width), static_cast<glm::u32vec2::value_type>(height)};
}

float Window::getAspectRatio() const {

    const glm::u32vec2 size = getSize();
    return static_cast<float>(size.x) / size.y;
}

bool Window::shouldClose() const {

    return glfwWindowShouldClose(m_window);
}

void Window::setGLFWCallbacks() {

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {

        switch (action) {
            case GLFW_PRESS:
                Receiver<MousePressed>::broadcast({window, button});
                break;
            case GLFW_RELEASE:
                Receiver<MouseReleased>::broadcast({window, button});
                break;
            default:
                break;
        }
    });

    setCallback<MouseScroll>(glfwSetScrollCallback, m_window);
    setCallback<FramebufferSizeEvent>(glfwSetFramebufferSizeCallback, m_window);
}
