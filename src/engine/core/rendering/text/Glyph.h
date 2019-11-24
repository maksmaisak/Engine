//
// Created by Maksym Maisak on 12/11/19.
//

#ifndef ENGINE_GLYPH_H
#define ENGINE_GLYPH_H

#include "Bounds.h"

namespace en {

    struct Glyph {

        float advance = 0.f;        /// Offset to move horizontally to the next character.
        Bounds<2, float> bounds;    /// Bounding rectangle relative to the baseline
        Bounds<2, int> textureRect; /// Texture coordinates of the glyph inside the font's texture
    };
}

#endif //ENGINE_GLYPH_H
