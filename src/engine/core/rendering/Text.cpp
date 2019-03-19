//
// Created by Maksym Maisak on 2019-02-15.
//

#include "Text.h"
#include <tuple>
#include "config.hpp"

using namespace en;

std::tuple<glm::vec2, glm::vec2> addGlyphQuad(std::vector<Vertex>& vertices, const glm::vec2& position, const sf::Glyph& glyph, const glm::vec2& atlasSize) {

    const float padding = 0.5f;

    const float left   =  glyph.bounds.left - padding;
    const float bottom = -glyph.bounds.top - glyph.bounds.height - padding;
    const float right  =  glyph.bounds.left + glyph.bounds.width + padding;
    const float top    = -glyph.bounds.top + padding;

    float u1 = static_cast<float>(glyph.textureRect.left) - padding;
    float v1 = static_cast<float>(glyph.textureRect.top  + glyph.textureRect.height) + padding;
    float u2 = static_cast<float>(glyph.textureRect.left + glyph.textureRect.width)  + padding;
    float v2 = static_cast<float>(glyph.textureRect.top ) - padding;
    const glm::vec2 multiplier = 1.f / atlasSize;
    u1 *= multiplier.x;
    u2 *= multiplier.x;
    v1 *= multiplier.y;
    v2 *= multiplier.y;

    glm::vec2 min = {position.x + left , position.y + bottom};
    glm::vec2 max = {position.x + right, position.y + top};

    vertices.push_back({{position.x + left , position.y + bottom, 0.f}, {u1, v1}});
    vertices.push_back({{position.x + right, position.y + top   , 0.f}, {u2, v2}});
    vertices.push_back({{position.x + left , position.y + top   , 0.f}, {u1, v2}});

    vertices.push_back({{position.x + left , position.y + bottom, 0.f}, {u1, v1}});
    vertices.push_back({{position.x + right, position.y + bottom, 0.f}, {u2, v1}});
    vertices.push_back({{position.x + right, position.y + top   , 0.f}, {u2, v2}});

    return {min, max};
}

const std::string& Text::getString() const {
    return m_string;
}

void Text::setString(const std::string& newString) {
    m_string = newString;
    m_needsGeometryUpdate = true;
}

const std::shared_ptr<Material>& Text::getMaterial() const {
    return m_material;
}

void Text::setMaterial(const std::shared_ptr<Material>& material) {

    if (m_material == material)
        return;

    m_material = material;
    m_needsGeometryUpdate = true;
}

const std::shared_ptr<sf::Font>& Text::getFont() const {
    return m_font;
}

void Text::setFont(const std::shared_ptr<sf::Font>& font) {

    if (m_font == font)
        return;

    m_font = font;
    m_needsGeometryUpdate = true;
}

const unsigned int Text::getCharacterSize() const {

    return m_characterSize;
}

void Text::setCharacterSize(unsigned int size) {

    if (m_characterSize == size)
        return;

    m_characterSize = size;
    m_needsGeometryUpdate = true;
}

const glm::vec4& Text::getColor() const {
    return m_color;
}

void Text::setColor(const glm::vec4& color) {

    m_color = color;
    if (m_material)
        m_material->setUniformValue("textColor", color);
}

const glm::vec2& Text::getAlignment() const {
    return m_alignment;
}

void Text::setAlignment(const glm::vec2& alignment) {
    m_alignment = alignment;
}

const std::vector<Vertex>& Text::getVertices() const {

    // Make it update no matter what since we can't be sure that
    // the atlas texture hasn't changed and invalidated our UVs,
    // because unlike sf::Text we have no access to m_font->getTexture(m_characterSize).m_cacheId
    m_needsGeometryUpdate = true;

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

        return;
//        int cacheId = m_font->getTexture(m_characterSize).m_cacheId; // can't access m_cacheId the way sf::Text does
//        if (cacheId == m_fontTextureCacheId)
//            return;
//
//        m_fontTextureCacheId = cacheId;
    }

    m_needsGeometryUpdate = false;
    m_vertices.clear();
    m_boundsMin = {0, 0};
    m_boundsMax = {0, 0};
    if (!m_font || !m_material)
        return;

    m_material->setUniformValue("fontAtlas", Material::FontAtlas{m_font, m_characterSize});
    m_material->setUniformValue("textColor", m_color);

    const auto temp = m_font->getTexture(m_characterSize).getSize();
    const glm::vec2 atlasSize = {temp.x, temp.y};
    const float whitespaceWidth = m_font->getGlyph(L' ', m_characterSize, false).advance;
    const float lineSpacing     = m_font->getLineSpacing(m_characterSize);

    glm::vec2 min = {0, 0};
    glm::vec2 max = {0, 0};
    glm::vec2 position = {0, 0};
    sf::Uint32 previousChar = 0;
    for (char c : m_string) {

        auto currentChar = (sf::Uint32)c;
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

        const sf::Glyph& glyph = m_font->getGlyph(currentChar, m_characterSize, false);
        auto[glyphMin, glyphMax] = addGlyphQuad(m_vertices, position, glyph, atlasSize);
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
        auto p = lua::PopperOnDestruct(lua);

        if (lua_isnil(lua, -1))
            return nullptr;

        if (lua.is<std::shared_ptr<Material>>())
            return lua.to<std::shared_ptr<Material>>();

        return std::make_shared<Material>(lua);
    }
};

Text& Text::addFromLua(Actor& actor, LuaState& lua) {
    auto& text = actor.add<Text>();

    if (auto material = readMaterial(lua))
        text.m_material = material;

    return text;
}

void Text::initializeMetatable(LuaState& lua) {

    lua::addProperty(lua, "string", lua::property(
        [](ComponentReference<Text>& ref) {return ref->getString();},
        [](ComponentReference<Text>& ref, const std::string& value) {ref->setString(value);}
    ));

    lua::addProperty(lua, "font", lua::writeonlyProperty(
        [](ComponentReference<Text>& ref, const std::string& filepath) {ref->setFont(Resources<sf::Font>::get(config::ASSETS_PATH + filepath));}
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
