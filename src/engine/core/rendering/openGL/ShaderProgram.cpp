#include "ShaderProgram.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "GLHelpers.h"
#include "Config.h"

using namespace en;

namespace {

    static_assert(std::is_convertible_v<decltype(ResourceLoader<ShaderProgram>::load(std::declval<const std::string&>(), std::declval<const PreprocessorDefinitions&>())), std::shared_ptr<ShaderProgram>>);
    static_assert(std::is_convertible_v<decltype(ResourceLoader<ShaderProgram>::load(std::declval<const std::string&>())), std::shared_ptr<ShaderProgram>>);
    static_assert(std::is_convertible_v<decltype(ResourceLoader<ShaderProgram>::load(std::declval<const std::string&>(), std::declval<const std::string&>())), std::shared_ptr<ShaderProgram>>);
    static_assert(std::is_convertible_v<decltype(ResourceLoader<ShaderProgram>::load(config::SHADER_PATH + "blit.vs", config::SHADER_PATH + "blur.fs")), std::shared_ptr<ShaderProgram>>);
    static_assert(detail::has_valid_load_function_v<ResourceLoader<ShaderProgram>, const Name&>);
    static_assert(detail::has_valid_load_function_v<ResourceLoader<ShaderProgram>, const std::string&>);
    static_assert(detail::has_valid_load_function_v<ResourceLoader<ShaderProgram>, const Name&>);
    static_assert(detail::has_valid_load_function_v<ResourceLoader<ShaderProgram>, const std::string&, const PreprocessorDefinitions&>);
    static_assert(detail::has_valid_load_function_v<ResourceLoader<ShaderProgram>, const Name&, const PreprocessorDefinitions&>);

    std::optional<std::string> getSource(const std::string& filepath, const PreprocessorDefinitions& preprocessorDefinitions) {

        std::ifstream file(filepath, std::ios::in);
        if (!file.is_open()) {

            std::cout << "Error reading shader " << filepath << std::endl;
            return std::nullopt;
        }

        std::stringstream text;
        std::cout << "Reading shader file... " << filepath << std::endl;

        std::string line;
        if (getline(file, line)) {

            text << line;
            for (const auto& definition : preprocessorDefinitions) {

                text << "\n" << "#define " << definition.name;
                if (definition.value) {
                    text << *definition.value;
                }
            }
        }

        while (getline(file, line)) {

            text << "\n" << line;
        }

        file.close();

        return text.str();
    }

    void printProgramError(GLuint programId) {

        glCheckError();

        int infoLogLength;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
        glCheckError();
        auto errorMessage = std::make_unique<char[]>(infoLogLength + 1);

        glGetProgramInfoLog(programId, infoLogLength, nullptr, errorMessage.get());
        std::cerr << errorMessage.get() << std::endl << std::endl;
    }

    void printShaderError(GLuint shaderId) {

        GLint infoLogLength = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
        auto errorMessage = std::make_unique<char[]>(infoLogLength + 1);

        glGetShaderInfoLog(shaderId, infoLogLength, nullptr, errorMessage.get());
        std::cerr << errorMessage.get() << std::endl << std::endl;
    }
}

ShaderProgram::ShaderProgram() {

    m_programId = glCreateProgram();
    std::cout << std::endl << "Program created with id:" << m_programId << std::endl;
}

ShaderProgram::~ShaderProgram() {

    glDeleteProgram(m_programId);
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) :
    m_programId(std::exchange(other.m_programId, 0))
{}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) {

    if (m_programId && this != &other) {
        glDeleteProgram(m_programId);
    }

    m_programId = std::exchange(other.m_programId, 0);
    return *this;
}

bool ShaderProgram::addShader(GLuint shaderType, const std::string& filepath, const PreprocessorDefinitions& preprocessorDefinitions) {

    glCheckError();

    std::optional<std::string> shaderCode = getSource(filepath, preprocessorDefinitions);
    if (!shaderCode) {
        return false;
    }

    glCheckError();
    GLuint shaderId = compileShader(shaderType, *shaderCode);
    if (shaderId == 0) {
        return false;
    }

    m_shaderIds.push_back(shaderId);
    return true;
}

void ShaderProgram::finalize() {

    for (GLuint shaderId : m_shaderIds) {
        glAttachShader(m_programId, shaderId);
    }

    glLinkProgram(m_programId);

    // Check the program
    GLint didLinkSuccessfully = GL_FALSE;
    glGetProgramiv(m_programId, GL_LINK_STATUS, &didLinkSuccessfully);

    if (didLinkSuccessfully) {
        std::cout << "Program linked ok." << std::endl << std::endl;
    } else {
        std::cerr << "Program linkage error:" << std::endl;
        printProgramError(m_programId);
    }

    for (GLuint shaderId : m_shaderIds) {
        glDeleteShader(shaderId);
    }
}

void ShaderProgram::use() const {
    glUseProgram(m_programId);
}

GLint ShaderProgram::getUniformLocation(const std::string& uniformName) const {
    return glGetUniformLocation(m_programId, uniformName.c_str());
}

GLint ShaderProgram::getAttributeLocation(const std::string& attributeLocation) const {
    return glGetAttribLocation(m_programId, attributeLocation.c_str());
}

// compile the code, and detect errors.
GLuint ShaderProgram::compileShader(GLuint shaderType, const std::string& shaderSource) {

    std::cout << "Compiling shader... " << std::endl;

    GLuint shaderId = glCreateShader(shaderType);
    const char* const sourcePointer = shaderSource.c_str();
    glShaderSource(shaderId, 1, &sourcePointer, nullptr);
    glCompileShader(shaderId);

    GLint compilerResult = GL_FALSE;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compilerResult);
    if (compilerResult) {
        std::cout << "Shader compiled ok." << std::endl;
        return shaderId;
    }

    std::cout << "Shader error:" << std::endl;
    printShaderError(shaderId);
    return 0;
}

std::vector<UniformInfo> ShaderProgram::getAllUniforms() const {

    std::vector<UniformInfo> infos;

    GLint count;
    glGetProgramiv(m_programId, GL_ACTIVE_UNIFORMS, &count);

    for (GLint i = 0; i < count; ++i) {

        const GLsizei nameBufferSize = 128;
        GLchar nameBuffer[nameBufferSize];
        GLsizei nameLength;
        GLint size;
        GLenum uniformType;
        glGetActiveUniform(m_programId, (GLuint)i, nameBufferSize, &nameLength, &size, &uniformType, nameBuffer);

        auto name = std::string(nameBuffer, static_cast<std::size_t>(nameLength));
        infos.push_back({getUniformLocation(name), name});
    }

    return infos;
}
