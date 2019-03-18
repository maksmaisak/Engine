//
// Created by Maksym Maisak on 2019-02-15.
//

#ifndef SAXION_Y2Q2_RENDERING_TEXT_H
#define SAXION_Y2Q2_RENDERING_TEXT_H

#include <string>
#include <vector>
#include <SFMl/Graphics.hpp>
#include "Vertex.h"
#include "Material.h"
#include "ComponentsToLua.h"
#include "Resources.h"

namespace en {

    class Text {

        LUA_TYPE(Text);

    public:
        static Text& addFromLua(Actor& actor, LuaState& lua);
        static void initializeMetatable(LuaState& lua);

        const std::string& getString() const;
        void setString(const std::string& newString);

        const std::shared_ptr<Material>& getMaterial() const;
        void setMaterial(const std::shared_ptr<Material>& material);

        const std::shared_ptr<sf::Font>& getFont() const;
        void setFont(const std::shared_ptr<sf::Font>& font);

        const unsigned int getCharacterSize() const;
        void setCharacterSize(unsigned int size);

        const glm::vec4& getColor() const;
        void setColor(const glm::vec4& color);

        const glm::vec2& getAlignment() const;
        void setAlignment(const glm::vec2& alignment);

        const std::vector<Vertex>& getVertices() const;
        const glm::vec2& getBoundsMin() const;
        const glm::vec2& getBoundsMax() const;

    private:

        void ensureGeometryUpdate() const;

        std::string m_string;
        std::shared_ptr<Material> m_material = std::make_shared<Material>("text");
        std::shared_ptr<sf::Font> m_font = Resources<sf::Font>::get(config::FONT_PATH + "arial.ttf");
        glm::vec4 m_color = glm::vec4(1);
        unsigned int m_characterSize = 30;
        glm::vec2 m_alignment = {0.5f, 0.5f};

        mutable bool m_needsGeometryUpdate = false;
        mutable std::vector<Vertex> m_vertices;
        mutable glm::vec2 m_boundsMin;
        mutable glm::vec2 m_boundsMax;
    };
}

#endif //SAXION_Y2Q2_RENDERING_TEXT_H
