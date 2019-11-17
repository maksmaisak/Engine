//
// Created by Maksym Maisak on 15/11/19.
//

#ifndef ENGINE_FONTATLAS_H
#define ENGINE_FONTATLAS_H

#include <memory>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <unordered_map>
#include <vector>
#include "Texture.h"
#include "Name.h"
#include "Glyph.h"
#include "FreetypeHelpers.h"

namespace en {

    class FontAtlas {
    public:

        FontAtlas(ft::fontsize_t characterSize, std::shared_ptr<FT_FaceRec> face);
        const Glyph& getGlyph(ft::codePoint_t codePoint);

        const Texture& getTexture() const;

    private:
        struct Row {
            ft::pixelCoord_t bottom;
            glm::vec<2, ft::pixelCoord_t> size;
        };

        Glyph& addGlyph(ft::codePoint_t codePoint);
        Glyph loadGlyph(ft::codePoint_t codePoint);
        Bounds<2, int> addGlyphRect(const glm::vec<2, ft::pixelCoord_t>& size);

        ft::fontsize_t m_characterSize;
        std::shared_ptr<FT_FaceRec> m_face;

        Texture m_texture;
        std::unordered_map<std::uint64_t, Glyph> m_glyphs;
        std::vector<Row> m_rows;
        std::vector<std::uint8_t> m_pixelBuffer;
    };
}

#endif //ENGINE_FONTATLAS_H
