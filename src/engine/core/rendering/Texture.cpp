
#include "Texture.h"
#include <algorithm>
#include <utility>
#include <iostream>
#include <string>
#include <cassert>
#include <array>
#include <SFML/Graphics.hpp> // For sf::Image
#include "glm.h"
#include "GLHelpers.h"

using namespace en;

namespace {

    int getNumMipmaps(Texture::Size size) {

        int i = 1;
        while (true) {
            size.x = std::max(1, size.x / 2);
            size.y = std::max(1, size.y / 2);
            if (size.x == 1 && size.y == 1)
                break;

            ++i;
        }

        return i;
    }
}

Texture::CreationSettings::CreationSettings() :
    kind(Kind::Texture2D),
    internalFormat(GL_SRGB_ALPHA),
    wrapS(GL_REPEAT),
    wrapT(GL_REPEAT),
    minFilter(GL_LINEAR_MIPMAP_LINEAR),
    magFilter(GL_LINEAR)
{}

Texture::Texture(const Size& size, const CreationSettings& settings) :
    m_size(size),
    m_kind(settings.kind)
{
    createOpenGlTexture2D(settings);
}

Texture::Texture(const std::string& filename, GLint internalFormat) {

    // Load from file using sf::Image, then put the data in an openGL buffer.
    sf::Image image;
    if (!image.loadFromFile(filename))
        return;

    auto temp = image.getSize();
    m_size = {temp.x, temp.y};

    // 0, 0 in sf::Image is top left, but openGL expects 0,0 to be bottom left, flip to compensate.
    image.flipVertically();

    CreationSettings settings;
    settings.internalFormat = internalFormat;
    createOpenGlTexture2D(settings, image.getPixelsPtr());

    m_kind = Kind::Texture2D;
}

Texture::Texture(const std::array<std::string, 6>& cubeSidePaths, GLint internalFormat) {

    std::array<sf::Image, 6> images;
    for (GLuint i = 0; i < images.size(); ++i)
        if (!images[i].loadFromFile(cubeSidePaths[i]))
            return;

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
    {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        for (GLuint i = 0; i < 6; ++i) {
            const sf::Image& image = images[i];
            const auto temp = image.getSize();
            m_size = {temp.x, temp.y};
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
        }

        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    m_kind = Kind::TextureCube;
}

Texture::Texture(Texture&& other) noexcept : m_id(std::exchange(other.m_id, 0)) {}

Texture& Texture::operator=(Texture&& other) noexcept {
    m_id = std::exchange(other.m_id, 0);
    return *this;
}

Texture::~Texture() {
	glDeleteTextures(1, &m_id);
}

GLuint Texture::getId() const {
	return m_id;
}

bool Texture::isValid() const {
    return m_id != 0 && m_kind != Kind::None;
}

Texture::Kind Texture::getKind() const {
    return m_kind;
}

Texture::Size Texture::getSize() const {
    return m_size;
}

void Texture::createOpenGlTexture2D(const CreationSettings& settings, const GLvoid* imageData) {

    assert(m_id == 0);
    assert(settings.kind == Kind::Texture2D);

    glCheckError();
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, settings.wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, settings.wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, settings.minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, settings.magFilter);
        glTexImage2D(GL_TEXTURE_2D, 0, settings.internalFormat, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::updateData2D(GLvoid* imageData, GLenum dataFormat) {

    assert(isValid());
    assert(m_kind == Kind::Texture2D);

    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_size.x, m_size.y, GL_RGBA, dataFormat, imageData);
    glBindTexture(GL_TEXTURE_2D, 0);
}
