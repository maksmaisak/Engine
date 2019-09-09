#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <map>
#include <string>
#include <GL/glew.h>
#include "glm.hpp"
#include "ResourceLoader.h"
#include <array>

namespace en {

	/// A wrapper around a texture id.
	/// Manages the lifetime of said id.
	class Texture final {

	public:

		enum class Kind {
			None,
			Texture2D,
			TextureCube
		};

		using Size = glm::vec<2, GLsizei>;

		Texture(const std::string& path, GLint internalFormat = GL_SRGB_ALPHA);
		Texture(const std::array<std::string, 6>& cubeSidePaths, GLint internalFormat = GL_SRGB_ALPHA);
		~Texture();
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;
		Texture(Texture&&) noexcept;
		Texture& operator=(Texture&&) noexcept;

		GLuint getId() const;
		bool isValid() const;
		Kind getKind() const;
		Size getSize() const;

	private:

		// ID of the OpenGL Texture object
		GLuint m_id = 0;
		Kind m_kind = Kind::None;
		Size m_size = {0, 0};
	};
}

#endif // TEXTURE_HPP
