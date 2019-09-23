#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <map>
#include <string>
#include <GL/glew.h>
#include <array>
#include "glm.h"
#include "Resources.h"
#include "config.hpp"

namespace en {

	/// A wrapper around a texture id.
	/// Manages the lifetime of said id.
	class Texture final {

	public:

        using Size = glm::vec<2, GLsizei>;

        enum class Kind {
            None,
            Texture2D,
            TextureCube
        };

	    struct CreationSettings {

            CreationSettings();

	        Kind kind;
	        bool generateMipmaps;

	        GLint internalFormat;
	        GLint wrapS;
	        GLint wrapT;
	        GLint minFilter;
	        GLint magFilter;
	    };

		explicit Texture(const Size& size, const CreationSettings& settings = {});
        explicit Texture(const std::string& path, const CreationSettings& settings = {});
        /// Deprecated
		explicit Texture(const std::string& path, GLint internalFormat);
		explicit Texture(const std::array<std::string, 6>& cubeSidePaths, GLint internalFormat = GL_SRGB_ALPHA);

		~Texture();
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;
		Texture(Texture&&) noexcept;
		Texture& operator=(Texture&&) noexcept;

		void updateData2D(GLvoid* pixelData, GLenum dataFormat, GLint offsetX, GLint offsetY, GLsizei width, GLsizei height);
        void updateData2D(GLvoid* pixelData, GLenum dataFormat = GL_UNSIGNED_BYTE);

		GLuint getId() const;
		bool isValid() const;
		Kind getKind() const;
		Size getSize() const;

	private:

	    void createOpenGlTexture2D(const CreationSettings& settings = {}, const GLvoid* imageData = nullptr);

		// ID of the OpenGL Texture object
		GLuint m_id = 0;
		Kind m_kind = Kind::None;
		Size m_size = {0, 0};
	};

    struct Textures : Resources<Texture> {

        inline static std::shared_ptr<Texture> white() {return get(config::TEXTURE_PATH + "white.png");}
        inline static std::shared_ptr<Texture> black() {return get(config::TEXTURE_PATH + "black.png");}
        inline static std::shared_ptr<Texture> transparent() {return get(config::TEXTURE_PATH + "transparent.png");}
        inline static std::shared_ptr<Texture> defaultNormalMap() {return get(config::TEXTURE_PATH + "defaultNormalMap.png", GL_RGBA);}
    };
}

#endif // TEXTURE_HPP
