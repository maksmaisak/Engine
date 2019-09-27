#include "Mesh.h"

#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <assimp/mesh.h>
#include <assimp/matrix4x4.h>
#include <assert.h>
#include <functional>

#include "utils/Meta.h"
#include "GLHelpers.h"
#include "Demangle.h"

using namespace en;

namespace {

	inline glm::mat4 toGlmMat4(const aiMatrix4x4& m) {

		return glm::mat4(
			m.a1, m.b1, m.c1, m.d1,
			m.a2, m.b2, m.c2, m.d2,
			m.a3, m.b3, m.c3, m.d3,
			m.a4, m.b4, m.c4, m.d4
		);
	}
}

std::shared_ptr<Mesh> Mesh::getQuad() {

    static std::shared_ptr<Mesh> quad;

    if (!quad) {

        quad = std::make_shared<Mesh>();
        quad->m_indices = {0, 3, 1, 0, 2, 3};
        quad->m_vertices = {{0, 0, 0}, {0, 1, 0}, {1, 0, 0}, {1, 1, 0}};
        quad->m_uvs = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
        quad->buffer();
    }

    return quad;
}

Mesh::Mesh(const aiMesh* aiMesh, const aiMatrix4x4& aiTransform) :
	m_indices   (aiMesh->mNumFaces * 3),
	m_vertices  (aiMesh->mNumVertices),
	m_uvs       (aiMesh->mNumVertices),
	m_normals   (aiMesh->mNumVertices),
	m_tangents  (aiMesh->mNumVertices),
	m_bitangents(aiMesh->mNumVertices)
{
	// Fill m_indices
	unsigned int counter = 0;
	for (unsigned int i = 0; i < aiMesh->mNumFaces; ++i) {
		const aiFace& face = aiMesh->mFaces[i];
		assert(face.mNumIndices == 3 && "Detected non-triangular face in mesh.");
		for (unsigned int j = 0; j < face.mNumIndices; ++j) {
			m_indices[counter++] = face.mIndices[j];
		}
	}

	const glm::mat4 transform = toGlmMat4(aiTransform);
	const glm::mat3 normalTransform = glm::mat3(glm::transpose(glm::inverse(transform)));

	const auto toGlmPosition = [&transform](const aiVector3D& vec) {return glm::vec3(transform * glm::vec4(vec.x, vec.y, vec.z, 1.f));};
	const auto toGlmNormal = [&normalTransform](const aiVector3D& vec) {return normalTransform * glm::vec3(vec.x, vec.y, vec.z);};
	static const auto toGlmUV = [](const aiVector3D& vec) {return glm::vec2(vec.x, vec.y);};

	if (aiMesh->HasPositions()) {
        std::transform(aiMesh->mVertices, aiMesh->mVertices + aiMesh->mNumVertices, m_vertices.begin(), toGlmPosition);
    }

	if (aiMesh->HasNormals()) {
        std::transform(aiMesh->mNormals, aiMesh->mNormals + aiMesh->mNumVertices, m_normals.begin(), toGlmNormal);
    }

	if (aiMesh->HasTextureCoords(0)) {
	    const aiVector3D* uv0 = aiMesh->mTextureCoords[0];
        std::transform(uv0, uv0 + aiMesh->mNumVertices, m_uvs.begin(), toGlmUV);
    }

	if (aiMesh->HasTangentsAndBitangents()) {
		std::transform(aiMesh->mTangents  , aiMesh->mTangents   + aiMesh->mNumVertices, m_tangents.begin()  , toGlmNormal);
		std::transform(aiMesh->mBitangents, aiMesh->mBitangents + aiMesh->mNumVertices, m_bitangents.begin(), toGlmNormal);
	} else {
		generateTangentsAndBitangents();
	}

	buffer();
}

void Mesh::render(GLint verticesAttrib, GLint normalsAttrib, GLint uvsAttrib, GLint tangentsAttrib, GLint bitangentsAttrib) const {

    m_vao.bind();

	static const auto tryEnableVertexAttribArray = [](GLint attributeLocation, const gl::VertexBufferObject& buffer, GLint numComponents, bool normalize) {

		if (attributeLocation > -1) {

		    assert(buffer.isValid() && "Mesh: no data for given vertex attribute");

            const auto location = static_cast<GLuint>(attributeLocation);

            buffer.bind(GL_ARRAY_BUFFER);
            glEnableVertexAttribArray(location);
            glVertexAttribPointer(location, numComponents, GL_FLOAT, normalize ? GL_TRUE : GL_FALSE, 0, nullptr);
            glCheckError();
        }
	};

	tryEnableVertexAttribArray(verticesAttrib  , m_vertexBuffer   , 3, false);
	tryEnableVertexAttribArray(normalsAttrib   , m_normalBuffer   , 3, true );
	tryEnableVertexAttribArray(uvsAttrib       , m_uvBuffer       , 2, false);
	tryEnableVertexAttribArray(tangentsAttrib  , m_tangentBuffer  , 3, false);
	tryEnableVertexAttribArray(bitangentsAttrib, m_bitangentBuffer, 3, false);
	m_indexBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);

	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, nullptr);
    glCheckError();

	if (bitangentsAttrib > -1) glDisableVertexAttribArray(static_cast<GLuint>(bitangentsAttrib));
	if (tangentsAttrib   > -1) glDisableVertexAttribArray(static_cast<GLuint>(tangentsAttrib  ));
	if (uvsAttrib        > -1) glDisableVertexAttribArray(static_cast<GLuint>(uvsAttrib       ));
	if (normalsAttrib    > -1) glDisableVertexAttribArray(static_cast<GLuint>(normalsAttrib   ));
	if (verticesAttrib   > -1) glDisableVertexAttribArray(static_cast<GLuint>(verticesAttrib  ));

	m_vao.unbind();
}

void Mesh::add(const Mesh& mesh, const glm::mat4& transform) {

    const auto transformIndex = [offset = static_cast<unsigned int>(m_vertices.size())](unsigned int i) {return offset + i;};
    std::transform(mesh.m_indices.begin(), mesh.m_indices.end(), std::back_inserter(m_indices), transformIndex);

    const auto transformPosition = [&transform](const glm::vec3& position) {return transform * glm::vec4(position, 1.f);};
    std::transform(mesh.m_vertices.begin(), mesh.m_vertices.end(), std::back_inserter(m_vertices), transformPosition);

    std::copy(mesh.m_uvs.begin(), mesh.m_uvs.end(), std::back_inserter(m_uvs));

    const auto transformNormal = [matrix = glm::mat3(glm::transpose(glm::inverse(transform)))](const glm::vec3& normal) {return matrix * normal;};
    std::transform(mesh.m_normals   .begin(), mesh.m_normals   .end(), std::back_inserter(m_normals   ), transformNormal);
    std::transform(mesh.m_tangents  .begin(), mesh.m_tangents  .end(), std::back_inserter(m_tangents  ), transformNormal);
    std::transform(mesh.m_bitangents.begin(), mesh.m_bitangents.end(), std::back_inserter(m_bitangents), transformNormal);

    markDirty();
}

bool Mesh::updateBuffers() {

    if (m_buffersNeedUpdating) {

        buffer();
        m_buffersNeedUpdating = false;
        return true;
    }

    return false;
}


void Mesh::markDirty() {

    m_buffersNeedUpdating = true;
}

// Sets the [bi]tangents of each vertex to the average of the [bi]tangents of its triangles.
void Mesh::generateTangentsAndBitangents() {

	const std::size_t numVertices  = m_vertices.size();
	const std::size_t numTriangles = m_indices.size() / 3;

	m_tangents.clear();
	m_tangents.resize(numVertices, glm::vec3(0));
	m_bitangents.clear();
	m_bitangents.resize(numVertices, glm::vec3(0));

	// Compute the (bi)tangents for each triangle
	for (std::size_t i = 0; i < numTriangles; ++i) {

		const std::size_t index0 = m_indices[i * 3 + 0];
		const std::size_t index1 = m_indices[i * 3 + 1];
		const std::size_t index2 = m_indices[i * 3 + 2];
		const glm::vec3& v0 = m_vertices[index0];
		const glm::vec3& v1 = m_vertices[index1];
		const glm::vec3& v2 = m_vertices[index2];
		const glm::vec2& uv0 = m_uvs[index0];
		const glm::vec2& uv1 = m_uvs[index1];
		const glm::vec2& uv2 = m_uvs[index2];

		const glm::vec3 delta1 = v1 - v0;
		const glm::vec3 delta2 = v2 - v0;
		const glm::vec2 deltaUV1 = uv1 - uv0;
		const glm::vec2 deltaUV2 = uv2 - uv0;

		const float f = 1.f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

		const glm::vec3 tangent = glm::normalize(glm::vec3(
            f * (deltaUV2.y * delta1.x - deltaUV1.y * delta2.x),
            f * (deltaUV2.y * delta1.y - deltaUV1.y * delta2.y),
            f * (deltaUV2.y * delta1.z - deltaUV1.y * delta2.z)
        ));

		const glm::vec3 bitangent = glm::normalize(glm::vec3(
            f * (-deltaUV2.x * delta1.x + deltaUV1.x * delta2.x),
            f * (-deltaUV2.x * delta1.y + deltaUV1.x * delta2.y),
            f * (-deltaUV2.x * delta1.z + deltaUV1.x * delta2.z)
        ));

		// Add to the (bi)tangents of the vertices of the triangle
		m_tangents[index0] += tangent;
		m_tangents[index1] += tangent;
		m_tangents[index2] += tangent;
		m_bitangents[index0] += bitangent;
		m_bitangents[index1] += bitangent;
		m_bitangents[index2] += bitangent;
	}

	// Average the summed (bi)tangents
	for (std::size_t index = 0; index < numVertices; ++index) {
		m_tangents  [index] = glm::normalize(m_tangents  [index]);
		m_bitangents[index] = glm::normalize(m_bitangents[index]);
	}
}

void Mesh::buffer() {

    static const auto bufferVector = [](gl::VertexBufferObject& buffer, const auto& vec, GLenum bufferKind = GL_ARRAY_BUFFER) {

        using TElement = typename utils::remove_cvref_t<decltype(vec)>::value_type;

        buffer = {};
        buffer.create();
        buffer.bind(bufferKind);
        glBufferData(bufferKind, vec.size() * sizeof(TElement), vec.data(), GL_STATIC_DRAW);
    };

    bufferVector(m_indexBuffer, m_indices, GL_ELEMENT_ARRAY_BUFFER);

    bufferVector(m_vertexBuffer   , m_vertices  );
    bufferVector(m_normalBuffer   , m_normals   );
    bufferVector(m_uvBuffer       , m_uvs       );
    bufferVector(m_tangentBuffer  , m_tangents  );
    bufferVector(m_bitangentBuffer, m_bitangents);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (!m_vao) {
        m_vao.create();
    }
}