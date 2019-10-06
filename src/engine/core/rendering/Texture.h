#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <map>
#include <string>
#include <GL/glew.h>
#include <array>
#include "glm.h"
#include "Resources.h"
#include "Config.h"
#include "TextureObject.h"

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

		explicit Texture(const Size& size, const CreationSettings& settings = {});
        explicit Texture(const std::string& path, const CreationSettings& settings = {});
        explicit Texture(const std::array<std::string, 6>& cubeSidePaths, const CreationSettings& settings = {});

		void updateData2D(GLvoid* pixelData, GLenum dataFormat, GLint offsetX, GLint offsetY, GLsizei width, GLsizei height);
        void updateData2D(GLvoid* pixelData, GLenum dataFormat = GL_UNSIGNED_BYTE);

        GLuint getId() const;
        bool isValid() const;
        Kind getKind() const;
        Size getSize() const;

	private:

	    void setUpOpenGLTexture2D(const CreationSettings& settings = {}, const GLvoid* imageData = nullptr);

		gl::TextureObject m_glTexture = {};
		Kind m_kind = Kind::None;
		Size m_size = {0, 0};
	};

    struct Textures : Resources<Texture> {

        inline static std::shared_ptr<Texture> white() {return get(config::TEXTURE_PATH + "white.png");}
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
