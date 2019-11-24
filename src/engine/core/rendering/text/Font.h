//
// Created by Maksym Maisak on 12/11/19.
//

#ifndef ENGINE_FONT_H
#define ENGINE_FONT_H

#include <memory>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <unordered_map>
#include "Name.h"
#include "Glyph.h"
#include "FontAtlas.h"

namespace en {

    class Font {

    public:

        Font(const Name& filename);
        const Texture& getTexture(ft::fontsize_t characterSize = 32) const;
        const Glyph& getGlyph(ft::codePoint_t codePoint, ft::fontsize_t characterSize = 32) const;

        float getLineSpacing(ft::fontsize_t characterSize) const;
        float getKerning(ft::codePoint_t first, ft::codePoint_t second, ft::fontsize_t characterSize) const;

    private:
        FontAtlas& getOrAddAtlasFor(ft::fontsize_t characterSize) const;

        mutable std::unordered_map<ft::fontsize_t, FontAtlas> m_atlases;
        std::shared_ptr<FT_FaceRec> m_face;
        Name m_filename;
    };
}

#endif //ENGINE_FONT_H
