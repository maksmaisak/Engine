//
// Created by Maksym Maisak on 17/11/19.
//

#include "FreetypeHelpers.h"
#include <iostream>

float ft::posToFloat(FT_Pos pos) {
    return static_cast<float>(pos) / static_cast<float>(1 << 6);
}

bool ft::setCurrentSizeOf(FT_Face face, fontsize_t characterSize) {

    const FT_UShort currentSize = face->size->metrics.x_ppem;
    if (currentSize == characterSize) {
        return true;
    }

    const FT_Error error = FT_Set_Pixel_Sizes(face, 0, characterSize);
    if (error == FT_Err_Invalid_Pixel_Size) {

        // Non-scalable fonts only support a fixed set of sizes, so the requested on may not be available.
        if (!FT_IS_SCALABLE(face)) {

            std::cerr << "Freetype: Failed to set bitmap font size to " << characterSize << std::endl;

            std::cerr << "Supported sizes: ";
            for (FT_Int i = 0; i < face->num_fixed_sizes; ++i) {
                const FT_Pos size = (face->available_sizes[i].y_ppem + 32) >> 6;
                std::cerr << size << " ";
            }
            std::cerr << std::endl;

        } else {
            std::cerr << "Freetype: Failed to set font size to " << characterSize << std::endl;
        }
    }

    return error == FT_Err_Ok;
}