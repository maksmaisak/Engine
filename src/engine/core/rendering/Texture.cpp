
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

    constexpr GLint DefaultMaxMipmapLevel = 1000;

    Texture::CreationSettings makeSettingsFromInternalFormat(GLint internalFormat) {

        Texture::CreationSettings settings;
        settings.internalFormat = internalFormat;
        return settings;
    }
}

Texture::CreationSettings::CreationSettings() :
    kind(Kind::Texture2D),
    generateMipmaps(true),
    numMipmapLevels(DefaultMaxMipmapLevel + 1),
    internalFormat(GL_SRGB8_ALPHA8),
    wrapS(GL_REPEAT),
    wrapT(GL_REPEAT),
    minFilter(GL_LINEAR_MIPMAP_LINEAR),
    magFilter(GL_LINEAR)
{}

Texture::Texture(const Size& size, const CreationSettings& settings) :
    m_size(size),
    m_kind(settings.kind)
{
    setUpOpenGLTexture2D(settings);
}

Texture::Texture(const std::string& filename, const CreationSettings& settings) :
    m_kind(Kind::Texture2D)
{
    assert(m_kind == settings.kind);

    // Load from file using sf::Image, then put the data in an openGL buffer.
    sf::Image image;
    if (!image.loadFromFile(filename)) {
        return;
    }

    auto temp = image.getSize();
    m_size = {temp.x, temp.y};

    // 0, 0 in sf::Image is top left, but openGL expects 0,0 to be bottom left, flip to compensate.
    image.flipVertically();
    setUpOpenGLTexture2D(settings, image.getPixelsPtr());
}

Texture::Texture(const std::array<std::string, 6>& cubeSidePaths, const CreationSettings& settings) :
    m_kind(Kind::TextureCube)
{

    std::array<sf::Image, 6> images;
    for (GLuint i = 0; i < images.size(); ++i)
        if (!images[i].loadFromFile(cubeSidePaths[i]))
            return;

    m_glTexture.create();
    m_glTexture.bind(GL_TEXTURE_CUBE_MAP);
    {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, settings.wrapS);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, settings.wrapT);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, settings.minFilter);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, settings.magFilter);

        for (GLuint i = 0; i < 6; ++i) {
            const sf::Image& image = images[i];
            const auto temp = image.getSize();
            m_size = {temp.x, temp.y};
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, settings.internalFormat, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
        }

        if (settings.generateMipmaps) {
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        }
    }
    m_glTexture.unbind(GL_TEXTURE_CUBE_MAP);
}

/// DEPRECATED
Texture::Texture(const std::string& filename, GLint internalFormat) :
    Texture(filename, makeSettingsFromInternalFormat(internalFormat))
{}

/// DEPRECATED
Texture::Texture(const std::array<std::string, 6>& cubeSidePaths, GLint internalFormat) :
    Texture(cubeSidePaths, makeSettingsFromInternalFormat(internalFormat))
{}

GLuint Texture::getId() const {
	return m_glTexture.getId();
}

bool Texture::isValid() const {
    return m_glTexture.isValid() && m_kind != Kind::None;
}

Texture::Kind Texture::getKind() const {
    return m_kind;
}

Texture::Size Texture::getSize() const {
    return m_size;
}

void Texture::setUpOpenGLTexture2D(const CreationSettings& settings, const GLvoid* imageData) {

    assert(settings.kind == Kind::Texture2D);

    constexpr GLenum Target = GL_TEXTURE_2D;

    m_glTexture.create();
    m_glTexture.bind(Target);
    {
        glTexParameteri(Target, GL_TEXTURE_WRAP_S, settings.wrapS);
        glTexParameteri(Target, GL_TEXTURE_WRAP_T, settings.wrapT);
        glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, settings.minFilter);
        glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, settings.magFilter);

        if (!settings.generateMipmaps) {

            glTexImage2D(Target, 0, settings.internalFormat, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

        } else {

            glTexParameteri(Target, GL_TEXTURE_BASE_LEVEL, 0);
            glTexParameteri(Target, GL_TEXTURE_MAX_LEVEL, settings.numMipmapLevels - 1);

            if (settings.numMipmapLevels == DefaultMaxMipmapLevel + 1) {

                glTexImage2D(Target, 0, settings.internalFormat, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

            } else {

                glTexStorage2D(GL_TEXTURE_2D, settings.numMipmapLevels, settings.internalFormat, m_size.x, m_size.y);
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_size.x, m_size.y, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
            }

            glGenerateMipmap(Target);
            glTexParameteri(Target, GL_TEXTURE_MAX_LEVEL, DefaultMaxMipmapLevel);
        }
    }
    m_glTexture.unbind(Target);
}

void Texture::updateData2D(GLvoid* imageData, GLenum dataFormat, GLint offsetX, GLint offsetY, GLsizei width, GLsizei height) {

    assert(isValid());
    assert(m_kind == Kind::Texture2D);
    assert(0 <= offsetX && offsetX < m_size.x);
    assert(0 <= offsetY && offsetY < m_size.y);
    assert(0 <= offsetX + width && offsetX + width <= m_size.x);
    assert(0 <= offsetY + height && offsetY + height <= m_size.y);

    constexpr GLenum Target = GL_TEXTURE_2D;
    m_glTexture.bind(Target);
    glTexSubImage2D(Target, 0, offsetX, offsetY, width, height, GL_RGBA, dataFormat, imageData);
    m_glTexture.unbind(Target);
}

void Texture::updateData2D(GLvoid* imageData, GLenum dataFormat) {

    updateData2D(imageData, dataFormat, 0, 0, m_size.x, m_size.y);
}
