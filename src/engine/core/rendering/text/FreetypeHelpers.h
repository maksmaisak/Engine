//
// Created by Maksym Maisak on 17/11/19.
//

#ifndef ENGINE_FREETYPEHELPERS_H
#define ENGINE_FREETYPEHELPERS_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <utility>

namespace ft {

    using pixelCoord_t = unsigned int;
    using fontsize_t = std::uint32_t;
    using codePoint_t = std::uint32_t;

    float posToFloat(FT_Pos pos);
    bool setCurrentSizeOf(FT_Face face, fontsize_t characterSize);
};

#endif //ENGINE_FREETYPEHELPERS_H
