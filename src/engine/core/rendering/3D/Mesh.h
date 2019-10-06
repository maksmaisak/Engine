#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <GL/glew.h>
#include <assimp/matrix4x4.h>
#include <assimp/mesh.h>
#include "glm.h"
#include "Resources.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"

namespace en {

	/// Stores mesh data
	/// Owns OpenGL buffers and a VAO.
	/// Can render with given vertex attribute locations.
	class Mesh final {

	public:

	    static std::shared_ptr<Mesh> getQuad();

        Mesh() = default;
        /// Imports the mesh data using Assimp
		explicit Mesh(const aiMesh* aiMesh, const aiMatrix4x4& transform = {});

        /// Activates attributes at given attribute locations and assigns vertex attribute buffers to them.
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

		/// Update the OpenGL buffers, if needed
		bool updateBuffers();

		/// Indicates that the CPU-side data of the mesh is out of sync with the GPU-side data.
		void markDirty();

	private:

		void generateTangentsAndBitangents();
		void buffer();

		bool m_buffersNeedUpdating = false;

		gl::VertexArrayObject m_vao;

		// OpenGL vertex attribute buffers
		gl::VertexBufferObject m_indexBuffer;
		gl::VertexBufferObject m_vertexBuffer;
		gl::VertexBufferObject m_normalBuffer;
		gl::VertexBufferObject m_uvBuffer;
		gl::VertexBufferObject m_tangentBuffer;
		gl::VertexBufferObject m_bitangentBuffer;

        // CPU-side vertex data
        std::vector<unsigned>  m_indices;
        std::vector<glm::vec3> m_vertices;
        std::vector<glm::vec3> m_normals;
        std::vector<glm::vec2> m_uvs;
        std::vector<glm::vec3> m_tangents;
        std::vector<glm::vec3> m_bitangents;
    };
}

#endif // MESH_HPP
