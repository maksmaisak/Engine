#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include <GL/glew.h>
#include <vector>
#include <string>
#include <memory>
#include <vector>
#include <optional>
#include <utils/Exception.h>
#include "Resources.h"
#include "config.hpp"
#include "GLSetUniform.h"

namespace en {

	struct PreprocessorDefinition {
		std::string name;
		std::optional<std::string> value = std::nullopt;
	};

	using PreprocessorDefinitions = std::vector<PreprocessorDefinition>;

	struct UniformInfo {
		GLint location = -1;
		std::string name;
	};

    /// Generic shader program to which you can add separate shaders.
    /// Usage:
    ///  - Create
    ///  - Add shaders using addShader
    ///  - Call finalize()
    ///  - Use
	class ShaderProgram {

	public:

		ShaderProgram();
		ShaderProgram(const ShaderProgram& other) = delete;
		ShaderProgram& operator=(const ShaderProgram& other) = delete;
		ShaderProgram(ShaderProgram&& other) = default;
		ShaderProgram& operator=(ShaderProgram&& other) = default;
		virtual ~ShaderProgram();

		/// Add a shader of a specific type (eg GL_VERTEX_SHADER / GL_FRAGMENT_SHADER)
		bool addShader(GLuint shaderType, const std::string& filepath, const PreprocessorDefinitions& preprocessorDefinitions = {});
		/// Link and compile all added shaders
		void finalize();

		void use() const;

		GLint getUniformLocation(const std::string& pName) const;
		GLint getAttribLocation (const std::string& pName) const;

		template<typename T>
		void setUniformValue(const std::string& name, T&& value);
		std::vector<UniformInfo> getAllUniforms() const;

	private:

		GLuint compileShader(GLuint shaderType, const std::string& shaderSource);

		GLuint m_programId = 0;
		std::vector<GLuint> m_shaderIds;
	};

	template<typename T>
	inline void ShaderProgram::setUniformValue(const std::string& name, T&& value) {

		GLint location = getUniformLocation(name);
		if (location == -1)
			throw utils::Exception("Material: No such uniform: " + name);

		gl::setUniform(location, std::forward<T>(value));
	}

	template<>
	struct ResourceLoader<ShaderProgram> {

        inline static std::shared_ptr<ShaderProgram> load(
        	const std::string& vertexShaderPath,
        	const std::string& fragmentShaderPath,
			const PreprocessorDefinitions& preprocessorDefinitions = {}
		) {

            auto shader = std::make_shared<ShaderProgram>();
            shader->addShader(GL_VERTEX_SHADER  , vertexShaderPath  , preprocessorDefinitions);
            shader->addShader(GL_FRAGMENT_SHADER, fragmentShaderPath, preprocessorDefinitions);
            shader->finalize();
            return shader;
        }

		inline static std::shared_ptr<ShaderProgram> load(
			const std::string& vertexShaderPath,
			const std::string& fragmentShaderPath,
			const std::string& geometryShaderPath,
			const PreprocessorDefinitions& preprocessorDefinitions = {}
		) {

			auto shader = std::make_shared<ShaderProgram>();
			shader->addShader(GL_VERTEX_SHADER  , vertexShaderPath  , preprocessorDefinitions);
			shader->addShader(GL_GEOMETRY_SHADER, geometryShaderPath, preprocessorDefinitions);
			shader->addShader(GL_FRAGMENT_SHADER, fragmentShaderPath, preprocessorDefinitions);
			shader->finalize();
			return shader;
		}

        inline static std::shared_ptr<ShaderProgram> load(
        	const std::string& name,
        	const PreprocessorDefinitions& preprocessorDefinitions = {}
		) {

        	std::string prefix = config::SHADER_PATH + name;
            return load(
                prefix + ".vs",
                prefix + ".fs",
                prefix + ".gs",
				preprocessorDefinitions
			);
        }
    };
}

#endif // SHADERPROGRAM_HPP

