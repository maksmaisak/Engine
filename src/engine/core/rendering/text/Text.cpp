//
// Created by Maksym Maisak on 2019-02-15.
//

#include "Text.h"
#include <tuple>
#include "Config.h"
#include "Resources.h"
#include "Font.h"

using namespace en;

namespace {

    Bounds2D addGlyphQuad(std::vector<Vertex>& vertices, const glm::vec2& position, const Glyph& glyph, const glm::vec2& atlasSize) {

        constexpr float padding = 0.5f;

        const glm::vec2 multiplier = 1.f / atlasSize;
        const glm::vec2 uvMin = (glm::vec2(glyph.textureRect.min) - padding) * multiplier;
        const glm::vec2 uvMax = (glm::vec2(glyph.textureRect.max) + padding) * multiplier;

        const Bounds2D glyphBounds = Bounds2D {
            position + glyph.bounds.min - padding,
            position + glyph.bounds.max + padding
        };

        vertices.push_back({{glyphBounds.min.x, glyphBounds.min.y, 0.f}, {uvMin.x, uvMin.y}});
        vertices.push_back({{glyphBounds.max.x, glyphBounds.max.y, 0.f}, {uvMax.x, uvMax.y}});
        vertices.push_back({{glyphBounds.min.x, glyphBounds.max.y, 0.f}, {uvMin.x, uvMax.y}});

        vertices.push_back({{glyphBounds.min.x, glyphBounds.min.y, 0.f}, {uvMin.x, uvMin.y}});
        vertices.push_back({{glyphBounds.max.x, glyphBounds.min.y, 0.f}, {uvMax.x, uvMin.y}});
        vertices.push_back({{glyphBounds.max.x, glyphBounds.max.y, 0.f}, {uvMax.x, uvMax.y}});

        return glyphBounds;
    }
}

Text::Text() :
    m_material(std::make_shared<Material>("text")),
    m_font(Resources<Font>::get(config::FONT_PATH + "arial.ttf")),
    m_color(1.f),
    m_characterSize(32),
    m_alignment(0.5f, 0.5f),
    m_needsGeometryUpdate(false),
    m_usedTextureCacheId(0),
    m_boundsMin(0, 0),
    m_boundsMax(0, 0)
{}

const std::string& Text::getString() const {
    return m_string;
}

Text& Text::setString(const std::string& newString) {

    m_string = newString;
    m_needsGeometryUpdate = true;

    return *this;
}

const std::shared_ptr<Material>& Text::getMaterial() const {
    return m_material;
}

Text& Text::setMaterial(const std::shared_ptr<Material>& material) {

    if (m_material == material) {
        return *this;
    }

    m_material = material;
    m_needsGeometryUpdate = true;

    return *this;
}

const std::shared_ptr<Font>& Text::getFont() const {
    return m_font;
}

Text& Text::setFont(const std::shared_ptr<Font>& font) {

    if (m_font == font) {
        return *this;
    }

    m_font = font;
    m_needsGeometryUpdate = true;

    return *this;
}

unsigned int Text::getCharacterSize() const {

    return m_characterSize;
}

Text& Text::setCharacterSize(unsigned int size) {

    if (m_characterSize == size) {
        return *this;
    }

    m_characterSize = size;
    m_needsGeometryUpdate = true;

    return *this;
}

const glm::vec4& Text::getColor() const {
    return m_color;
}

Text& Text::setColor(const glm::vec4& color) {

    m_color = color;
    if (m_material) {
        m_material->setUniformValue("textColor", color);
    }

    return *this;
}

const glm::vec2& Text::getAlignment() const {
    return m_alignment;
}

Text& Text::setAlignment(const glm::vec2& alignment) {

    m_alignment = alignment;

    return *this;
}

const std::vector<Vertex>& Text::getVertices() const {

    ensureGeometryUpdate();
    return m_vertices;
}

const glm::vec2& Text::getBoundsMin() const {

    ensureGeometryUpdate();
    return m_boundsMin;
}

const glm::vec2& Text::getBoundsMax() const {

    ensureGeometryUpdate();
    return m_boundsMax;
}

void Text::ensureGeometryUpdate() const {

    if (!m_needsGeometryUpdate) {

        const std::uint64_t cacheId = m_font->getTexture(m_characterSize).getCacheId();
        if (cacheId == m_usedTextureCacheId) {
            return;
        }

        m_usedTextureCacheId = cacheId;
    }

    m_needsGeometryUpdate = false;
    m_vertices.clear();
    m_boundsMin = {0, 0};
    m_boundsMax = {0, 0};
    if (!m_font || !m_material) {
        return;
    }

    m_material->setUniformValue("fontAtlas", Material::FontAtlas{m_font, m_characterSize});
    m_material->setUniformValue("textColor", m_color);

    const glm::vec2 atlasSize = m_font->getTexture(m_characterSize).getSize();
    const float whitespaceWidth = m_font->getGlyph(L' ', m_characterSize).advance;
    const float lineSpacing     = m_font->getLineSpacing(m_characterSize);

    glm::vec2 min {0, 0};
    glm::vec2 max {0, 0};
    glm::vec2 position {0, 0};
    ft::codePoint_t previousChar = 0;
    for (char c : m_string) {

        const auto currentChar = static_cast<ft::codePoint_t>(c);
        position.x += m_font->getKerning(previousChar, currentChar, m_characterSize);
        previousChar = currentChar;

        if (c == ' ' || c == '\n' || c == '\t') {

            // Update the current bounds (min coordinates)
            min.x = std::min(min.x, position.x);
            min.y = std::min(min.y, position.y);

            switch (c) {
                case ' ':  position.x += whitespaceWidth;             break;
                case '\t': position.x += whitespaceWidth * 4;         break;
                case '\n': position.y -= lineSpacing; position.x = 0; break;
                default: break;
            }

            // Update the current bounds (max coordinates)
            max.x = std::max(max.x, position.x);
            max.y = std::max(max.y, position.y);

            // Skip making a glyph for this
            continue;
        }

        const Glyph& glyph = m_font->getGlyph(currentChar, m_characterSize);
        auto [glyphMin, glyphMax] = addGlyphQuad(m_vertices, position, glyph, atlasSize);
        min.x = std::min(min.x, glyphMin.x);
        min.y = std::min(min.y, glyphMin.y);
        max.x = std::max(max.x, glyphMax.x);
        max.y = std::max(max.y, glyphMax.y);

        position.x += glyph.advance;
    }

    m_boundsMin = min;
    m_boundsMax = max;
}

namespace {

    std::shared_ptr<Material> readMaterial(LuaState& lua) {

        lua_getfield(lua, -1, "material");
        const lua::PopperOnDestruct p(lua);

        if (lua_isnil(lua, -1)) {
            return nullptr;
        }

        if (lua.is<std::shared_ptr<Material>>()) {
            return lua.to<std::shared_ptr<Material>>();
        }

        return std::make_shared<Material>(lua);
    }
};

Text& Text::addFromLua(Actor& actor, LuaState& lua) {

    auto& text = actor.add<Text>();

    if (const std::shared_ptr<Material> material = readMaterial(lua)) {
        text.m_material = material;
    }

    return text;
}

void Text::initializeMetatable(LuaState& lua) {

    lua::addProperty(lua, "string", lua::property(
        [](ComponentReference<Text>& ref) {return ref->getString();},
        [](ComponentReference<Text>& ref, const std::string& value) {ref->setString(value);}
    ));

    lua::addProperty(lua, "font", lua::writeonlyProperty(
        [](ComponentReference<Text>& ref, const std::string& filepath) {ref->setFont(Resources<Font>::get(config::ASSETS_PATH + filepath));}
    ));

    lua::addProperty(lua, "fontSize", lua::property(
        [](ComponentReference<Text>& ref) {return ref->getCharacterSize();},
        [](ComponentReference<Text>& ref, unsigned int size) {ref->setCharacterSize(size);}
    ));

    lua::addProperty(lua, "color", lua::property(
        [](ComponentReference<Text>& ref) {return ref->getColor();},
        [](ComponentReference<Text>& ref, const glm::vec4& color) {ref->setColor(color);}
    ));

    lua::addProperty(lua, "alignment", lua::property(
        [](ComponentReference<Text>& ref) {return ref->getAlignment();},
        [](ComponentReference<Text>& ref, const glm::vec2& alignment) {ref->setAlignment(alignment);}
    ));
}
