#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <GL/glew.h>
#include <assimp/matrix4x4.h>
#include "glm.hpp"
#include "ResourceLoader.h"

struct aiMesh;

namespace en {

	/// Stores mesh data
	/// Owns OpenGL buffers and a VAO.
	/// Can render with given vertex attribute locations.
	class Mesh final {

	public:

		Mesh() = default;
		Mesh(const aiMesh* aiMesh, const aiMatrix4x4& transform = {});

		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;
		Mesh(Mesh&& other) noexcept;
		Mesh& operator=(Mesh&& other) noexcept;
		~Mesh();

        /// Activates attributes at given attribute locations and assign vertex attribute buffers to them.
        /// Then performs a draw call.
		void render(
			GLint verticesAttrib   = -1,
			GLint normalsAttrib    = -1,
			GLint uvsAttrib        = -1,
			GLint tangentsAttrib   = -1,
			GLint bitangentsAttrib = -1
		) const;

		/// Add the geometry of another mesh to this, optionally pretransforming it with a given transformation matrix.
		/// Useful for batching. Call updateBuffers after adding the data.
		void add(const Mesh& mesh, const glm::mat4& transform = glm::mat4(1));
		/// Update the OpenGL buffers.
		bool updateBuffers();

	private:

		void generateTangentsAndBitangents();
		void buffer();
		void deleteBuffers();

		bool m_buffersNeedUpdating = false;

		GLuint m_vao = 0;

		// IDs of vertex attribute buffers
		GLuint m_indexBufferId     = 0;
		GLuint m_vertexBufferId    = 0;
		GLuint m_normalBufferId    = 0;
		GLuint m_uvBufferId        = 0;
		GLuint m_tangentBufferId   = 0;
		GLuint m_bitangentBufferId = 0;

		// Vertex data
		std::vector<unsigned>  m_indices;
		std::vector<glm::vec3> m_vertices;
		std::vector<glm::vec3> m_normals;
		std::vector<glm::vec2> m_uvs;
		std::vector<glm::vec3> m_tangents;
		std::vector<glm::vec3> m_bitangents;
	};
}

#endif // MESH_HPP
