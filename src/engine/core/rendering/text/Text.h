//
// Created by Maksym Maisak on 2019-02-15.
//

#ifndef ENGINE_TEXT_H
#define ENGINE_TEXT_H

#include <string>
#include <vector>
#include "Vertex.h"
#include "Material.h"
#include "ComponentsToLua.h"

namespace en {

    class Text {

        LUA_TYPE(Text);

    public:
        static Text& addFromLua(Actor& actor, LuaState& lua);
        static void initializeMetatable(LuaState& lua);

        Text();

        const std::string& getString() const;
        Text& setString(const std::string& newString);

        const std::shared_ptr<Material>& getMaterial() const;
        Text& setMaterial(const std::shared_ptr<Material>& material);

        const std::shared_ptr<class Font>& getFont() const;
        Text& setFont(const std::shared_ptr<class Font>& font);

        unsigned int getCharacterSize() const;
        Text& setCharacterSize(unsigned int size);

        const glm::vec4& getColor() const;
        Text& setColor(const glm::vec4& color);

        const glm::vec2& getAlignment() const;
        Text& setAlignment(const glm::vec2& alignment);

        const std::vector<Vertex>& getVertices() const;
        const glm::vec2& getBoundsMin() const;
        const glm::vec2& getBoundsMax() const;

    private:

        void ensureGeometryUpdate() const;

        std::string m_string;
        std::shared_ptr<Material> m_material;
        std::shared_ptr<class Font> m_font;
        glm::vec4 m_color;
        unsigned int m_characterSize;
        glm::vec2 m_alignment;

        mutable bool m_needsGeometryUpdate;
        mutable std::uint64_t m_usedTextureCacheId;
        mutable std::vector<Vertex> m_vertices;
        mutable glm::vec2 m_boundsMin;
        mutable glm::vec2 m_boundsMax;
    };
}

#endif //ENGINE_TEXT_H
