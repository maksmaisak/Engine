
#include "Texture.h"
#include <algorithm>
#include <cassert>
#include <array>
#include <mutex>
#include "stb_image.h"
#include "ScopedBind.h"
#include "GLHelpers.h"
#include "FramebufferObject.h"
#include "PostProcessingUtilities.h"

using namespace en;

namespace {

    constexpr GLint DefaultMaxMipmapLevel = 1000;

    Texture::CreationSettings makeSettingsFromInternalFormat(GLint internalFormat) noexcept {

        Texture::CreationSettings settings;
        settings.internalFormat = internalFormat;
        return settings;
    }

    Texture::CreationSettings makeLinearColorSettings() noexcept {

        Texture::CreationSettings settings;
        settings.internalFormat = GL_RGBA8;
        return settings;
    }

    std::mutex cacheIdMutex;
    std::mutex getMaxSizeMutex;

    std::uint64_t getNewCacheId() {

        const std::unique_lock lock(cacheIdMutex);

        static std::uint64_t nextId = 1;
        return nextId++;
    }

    using imageData_t = std::unique_ptr<stbi_uc, decltype(&stbi_image_free)>;
    struct ImageDescription {

        imageData_t data = {nullptr, stbi_image_free};
        Texture::Size size = {0, 0};
        int numChannels = 0;
    };

    ImageDescription loadImage(const char* filename) {

        stbi_set_flip_vertically_on_load(1);

        Texture::Size size = {0, 0};
        int numChannels = 0;
        imageData_t data = {stbi_load(filename, &size.x, &size.y, &numChannels, 4), stbi_image_free};
        return {std::move(data), size, numChannels};
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

const Texture::CreationSettings Texture::CreationSettings::linearColorSettings = makeLinearColorSettings();

unsigned int Texture::getMaxSize() {

    const std::unique_lock lock(getMaxSizeMutex);

    static bool checked = false;
    static GLint maxSize = 0;
    if (!checked) {
        checked = true;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
    }

    return static_cast<unsigned int>(maxSize);
}

std::unique_ptr<Texture> Texture::load(const Name& path, const Texture::CreationSettings& settings) {

    const ImageDescription image = loadImage(path);
    if (!image.data) {
        return nullptr;
    }

    return std::make_unique<Texture>(image.size, static_cast<const GLvoid*>(image.data.get()), settings);
}

std::unique_ptr<Texture> Texture::load(const std::array<Name, 6>& cubeSidePaths, const Texture::CreationSettings& settings) {

    std::array<ImageDescription, 6> images {};
    for (std::size_t i = 0; i < images.size(); ++i) {
        images[i] = loadImage(cubeSidePaths[i]);
        if (!images[i].data) {
            return nullptr;
        }
    }

    std::array<const GLvoid*, 6> cubeSideData {};
    std::transform(images.begin(), images.end(), cubeSideData.begin(), [](const ImageDescription& image) {
        return static_cast<const GLvoid*>(image.data.get());
    });

    assert(std::all_of(images.begin(), images.end(), [&firstImage = images[0]](const ImageDescription& image) {
        return image.size == firstImage.size && image.numChannels == firstImage.numChannels;
    }));

    return std::make_unique<Texture>(images[0].size, cubeSideData, settings);
}

Texture::Texture(const Size& size, const GLvoid* imageData, const CreationSettings& settings) :
    m_glTexture(gl::ForceCreate),
    m_kind(Kind::Texture2D),
    m_size(size),
    m_cacheId(getNewCacheId())
{
    constexpr GLenum target = GL_TEXTURE_2D;
    const gl::ScopedBind bindTexture(m_glTexture, target);

    glTexParameteri(target, GL_TEXTURE_WRAP_S, settings.wrapS);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, settings.wrapT);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, settings.minFilter);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, settings.magFilter);

    // Mipmapping
    if (!settings.generateMipmaps) {

        glTexImage2D(target, 0, settings.internalFormat, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

    } else {

        glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, settings.numMipmapLevels - 1);

        const bool isDefaultMipmapLevelCount = settings.numMipmapLevels == DefaultMaxMipmapLevel + 1;
        if (isDefaultMipmapLevelCount) {
            glTexImage2D(target, 0, settings.internalFormat, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        } else {
            glTexStorage2D(target, settings.numMipmapLevels, settings.internalFormat, m_size.x, m_size.y);
            glTexSubImage2D(target, 0, 0, 0, m_size.x, m_size.y, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        }

        glGenerateMipmap(target);
        glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, DefaultMaxMipmapLevel);
    }
}

Texture::Texture(const Size& size, const std::array<const GLvoid*, 6>& cubeSidesImageData, const CreationSettings& settings) :
    m_glTexture(gl::ForceCreate),
    m_kind(Kind::TextureCube),
    m_size(size),
    m_cacheId(getNewCacheId())
{
    constexpr GLenum target = GL_TEXTURE_CUBE_MAP;
    const gl::ScopedBind bindTexture(m_glTexture, target);

    glTexParameteri(target, GL_TEXTURE_WRAP_S, settings.wrapS);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, settings.wrapT);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, settings.minFilter);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, settings.magFilter);

    for (GLuint i = 0; i < 6; ++i) {
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, settings.internalFormat,
            m_size.x, m_size.y, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, cubeSidesImageData[i]
        );
    }

    if (settings.generateMipmaps) {
        glGenerateMipmap(target);
    }
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

std::uint64_t Texture::getCacheId() const {
    return m_cacheId;
}

gl::TextureObject& Texture::getGLTextureObject() {
    return m_glTexture;
}

const gl::TextureObject& Texture::getGLTextureObject() const {
    return m_glTexture;
}

void Texture::updateData2D(const void* imageData, GLenum dataFormat, GLint offsetX, GLint offsetY, GLsizei width, GLsizei height) {

    assert(isValid());
    assert(m_kind == Kind::Texture2D);
    assert(0 <= offsetX && offsetX < m_size.x);
    assert(0 <= offsetY && offsetY < m_size.y);
    assert(0 <= offsetX + width && offsetX + width <= m_size.x);
    assert(0 <= offsetY + height && offsetY + height <= m_size.y);

    const gl::ScopedBind bindTexture(m_glTexture, GL_TEXTURE_2D);
    glTexSubImage2D(GL_TEXTURE_2D, 0, offsetX, offsetY, width, height, GL_RGBA, dataFormat, imageData);
    m_cacheId = getNewCacheId();
}

void Texture::updateData2D(const void* imageData, GLenum dataFormat) {
    updateData2D(imageData, dataFormat, 0, 0, m_size.x, m_size.y);
}

void Texture::updateData2D(const Texture& otherTexture) {

    if (!otherTexture.isValid()) {
        return;
    }

    assert(otherTexture.m_size.x <= m_size.x);
    assert(otherTexture.m_size.y <= m_size.y);

    const gl::FramebufferObject fbo(gl::ForceCreate);
    const gl::ScopedBind bindFBO(fbo, GL_FRAMEBUFFER);
    {
        const gl::ScopedBind bindTexture(m_glTexture, GL_TEXTURE_2D);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_glTexture, 0);
    }

    GLint oldViewport[4];
    glGetIntegerv(GL_VIEWPORT, oldViewport);
    glViewport(0, 0, otherTexture.m_size.x, otherTexture.m_size.y);

    gl::blit(otherTexture.getGLTextureObject(), fbo, true);

    glViewport(oldViewport[0], oldViewport[1], oldViewport[2], oldViewport[3]);
}
