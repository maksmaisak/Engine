#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <map>
#include <memory>
#include <string>
#include <array>
#include <GL/glew.h>
#include "glm.h"
#include "Resources.h"
#include "Config.h"
#include "TextureObject.h"
#include "Name.h"

namespace en {

    /// Represents a texture resource.
    /// Imports texture data, creates the OpenGL texture, buffers data to the GPU.
    class Texture final {

    public:

        using Size = glm::vec<2, GLsizei>;

        enum class Kind {
            None,
            Texture2D,
            TextureCube
        };

        struct CreationSettings {

            static const CreationSettings linearColorSettings;

            CreationSettings();

            Kind kind;

            bool generateMipmaps;
            GLint numMipmapLevels;

            GLint internalFormat;
            GLint wrapS;
            GLint wrapT;
            GLint minFilter;
            GLint magFilter;
        };

        static unsigned int getMaxSize();

        static std::unique_ptr<Texture> load(const Name& path, const CreationSettings& settings = {});
        static std::unique_ptr<Texture> load(const std::array<Name, 6>& cubeSidePaths, const CreationSettings& settings = {});

        explicit Texture(const Size& size, const GLvoid* imageData = nullptr, const CreationSettings& settings = {});
        explicit Texture(const Size& size, const std::array<const GLvoid*, 6>& imageData, const CreationSettings& settings = {});

        void updateData2D(const void* pixelData, GLenum dataFormat, GLint offsetX, GLint offsetY, GLsizei width, GLsizei height);
        void updateData2D(const void* pixelData, GLenum dataFormat = GL_UNSIGNED_BYTE);
        void updateData2D(const Texture& other);

        gl::TextureObject& getGLTextureObject();
        const gl::TextureObject& getGLTextureObject() const;
        bool isValid() const;
        Kind getKind() const;
        Size getSize() const;
        std::uint64_t getCacheId() const;

    private:

        gl::TextureObject m_glTexture = {};
        Kind m_kind = Kind::None;
        Size m_size = {0, 0};
        std::uint64_t m_cacheId;
    };

    struct Textures : Resources<Texture> {

        inline static std::shared_ptr<Texture> white() {return get(Name(config::TEXTURE_PATH + "white.png"));}
        inline static std::shared_ptr<Texture> black() {return get(config::TEXTURE_PATH + "black.png");}
        inline static std::shared_ptr<Texture> transparent() {return get(config::TEXTURE_PATH + "transparent.png");}
        inline static std::shared_ptr<Texture> defaultNormalMap() {
            Texture::CreationSettings settings;
            settings.internalFormat = GL_RGBA8;
            return get(config::TEXTURE_PATH + "defaultNormalMap.png", settings);
        }
    };
}

#endif // TEXTURE_HPP
