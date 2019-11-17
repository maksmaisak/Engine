//
// Created by Maksym Maisak on 15/11/19.
//

#include "FontAtlas.h"
#include <cassert>
#include FT_GLYPH_H
#include "Defer.h"
#include "ScopedBind.h"
#include "FreetypeHelpers.h"

using namespace en;

namespace {

    Texture makeTexture(const Texture::Size& size) {

        Texture::CreationSettings settings {};
        return Texture(size, nullptr, settings);
    }

    Bounds<2, float> getGlyphBounds(FT_GlyphSlot glyphSlot) {

        assert(glyphSlot);

        Bounds<2, float> bounds;

        bounds.min = {
            ft::posToFloat(glyphSlot->metrics.horiBearingX),
            ft::posToFloat(glyphSlot->metrics.horiBearingY - glyphSlot->metrics.height)
        };

        bounds.max = {
            bounds.min.x + ft::posToFloat(glyphSlot->metrics.width),
            bounds.min.y + ft::posToFloat(glyphSlot->metrics.height),
        };

        return bounds;
    }

    void readFromBitmapIntoBuffer(std::vector<std::uint8_t>& pixelBuffer, const FT_Bitmap& bitmap, ft::pixelCoord_t padding) {

        struct Local {

            static std::uint8_t getAlpha(const std::uint8_t* pixelsRow, unsigned int column) {
                return pixelsRow[column];
            }

            // Pixels are 1 bit monochrome values, packed into bytes.
            static std::uint8_t getAlphaMonochrome(const std::uint8_t* pixelsRow, unsigned int column) {

                const bool isWhite = pixelsRow[column / 8] & (1 << (7 - (column % 8)));
                return isWhite ? 0xff : 0x00;
            }
        };

        const glm::vec<2, ft::pixelCoord_t> paddedBitmapSize {
            bitmap.width + padding * 2,
            bitmap.rows  + padding * 2
        };

        pixelBuffer.resize(paddedBitmapSize.x * paddedBitmapSize.y * 4);

        std::uint8_t* current = pixelBuffer.data();
        std::uint8_t* const end = current + pixelBuffer.size();
        while (current != end) {
            (*current++) = 0xff;
            (*current++) = 0xff;
            (*current++) = 0xff;
            (*current++) = 0x00;
        }

        const auto getAlpha = bitmap.pixel_mode == FT_PIXEL_MODE_MONO ? Local::getAlphaMonochrome : Local::getAlpha;

        std::uint8_t* pixelsRow = bitmap.buffer;
        for (ft::pixelCoord_t y = paddedBitmapSize.y - padding - 1; y >= padding; --y) {
            for (ft::pixelCoord_t x = padding; x < paddedBitmapSize.x - padding; ++x) {

                const std::size_t index = x + y * paddedBitmapSize.x;
                pixelBuffer.at(index * 4 + 3) = getAlpha(pixelsRow, x - padding);
            }
            pixelsRow += bitmap.pitch;
        }
    }

    void writeToTexture(Texture& texture, const std::vector<std::uint8_t>& pixelBuffer, Bounds<2, int> pixelBoundsInTexture) {

        assert(texture.isValid());
        assert(pixelBuffer.size() == pixelBoundsInTexture.area() * 4);

        const glm::vec<2, int> size = pixelBoundsInTexture.size();
        texture.updateData2D(
            pixelBuffer.data(), GL_UNSIGNED_BYTE,
            pixelBoundsInTexture.min.x, pixelBoundsInTexture.min.y, size.x, size.y
        );
    }
}

FontAtlas::FontAtlas(ft::fontsize_t characterSize, std::shared_ptr<FT_FaceRec> face) :
    m_characterSize(characterSize),
    m_face(std::move(face)),
    m_texture(makeTexture({128, 128}))
{
    assert(m_face);
}

const Texture& FontAtlas::getTexture() const {
    return m_texture;
}

const Glyph& FontAtlas::getGlyph(ft::codePoint_t codePoint) {

    const auto it = m_glyphs.find(codePoint);
    if (it != m_glyphs.end()) {
        return it->second;
    }

    return addGlyph(codePoint);
}

Glyph& FontAtlas::addGlyph(ft::codePoint_t codePoint) {

    const Glyph glyph = loadGlyph(codePoint);
    const auto [it, didEmplace] = m_glyphs.emplace(codePoint, glyph);
    assert(didEmplace);
    return it->second;
}

Glyph FontAtlas::loadGlyph(ft::codePoint_t codePoint) {

    Glyph glyph {};

    if (!ft::setCurrentSizeOf(m_face.get(), m_characterSize)) {
        return glyph;
    }

    if (FT_Load_Char(m_face.get(), codePoint, FT_LOAD_TARGET_NORMAL | FT_LOAD_FORCE_AUTOHINT)) {
        return glyph;
    }

    FT_Glyph glyphDesc;
    if (FT_Get_Glyph(m_face->glyph, &glyphDesc)) {
        return glyph;
    }
    defer {
        FT_Done_Glyph(glyphDesc);
    };

    if (FT_Glyph_To_Bitmap(&glyphDesc, FT_RENDER_MODE_NORMAL, nullptr, true)) {
        return glyph;
    }

    glyph.advance = ft::posToFloat(m_face->glyph->metrics.horiAdvance);

    FT_Bitmap& bitmap = reinterpret_cast<FT_BitmapGlyph>(glyphDesc)->bitmap;
    const glm::vec<2, ft::pixelCoord_t> bitmapSize = {bitmap.width, bitmap.rows};
    if (bitmapSize.x > 0 && bitmapSize.y > 0) {

        constexpr ft::pixelCoord_t padding = 1;

        const glm::vec<2, ft::pixelCoord_t> paddedBitmapSize = bitmapSize + padding * 2;

        const Bounds<2, int> paddedBoundsInTexture = addGlyphRect(paddedBitmapSize);
        glyph.textureRect = paddedBoundsInTexture;
        glyph.textureRect.min += padding;
        glyph.textureRect.max -= padding;

        glyph.bounds = getGlyphBounds(m_face->glyph);

        readFromBitmapIntoBuffer(m_pixelBuffer, bitmap, padding);
        writeToTexture(m_texture, m_pixelBuffer, paddedBoundsInTexture);
    }

    return glyph;
}

Bounds<2, int> FontAtlas::addGlyphRect(const glm::vec<2, ft::pixelCoord_t>& rectSize) {

    Row* rowToInsert = [&]() -> Row* {

        float bestRatio = 0.f;
        Row* bestRow = nullptr;
        for (Row& row : m_rows) {

            const float heightRatio = static_cast<float>(rectSize.y) / row.size.y;
            if (heightRatio < 0.7f || heightRatio > 1.f) {
                continue;
            }

            // Check if there's enough horizontal space left in the row
            if (row.size.x + rectSize.x > m_texture.getSize().x) {
                continue;
            }

            // Make sure that this new row is the best found so far
            if (heightRatio < bestRatio) {
                continue;
            }

            // The current row passed all the tests: we can select it
            bestRow = &row;
            bestRatio = heightRatio;
        }

        return bestRow;
    }();

    if (!rowToInsert) {

        // make new row
        const ft::pixelCoord_t rowHeight = static_cast<ft::pixelCoord_t>(rectSize.y * 1.1f);
        const ft::pixelCoord_t nextRowBottom = m_rows.empty() ? 0 : m_rows.back().bottom + m_rows.back().size.y;
        const auto canFitIntoTextureOfSize = [&](const Texture::Size& size) {
            return nextRowBottom + rowHeight < size.y && rectSize.x < size.x;
        };

        const glm::vec<2, ft::pixelCoord_t> textureSize = m_texture.getSize();
        if (!canFitIntoTextureOfSize(textureSize)) {

            glm::vec<2, ft::pixelCoord_t> newTextureSize = textureSize;
            do {
                newTextureSize *= 2;
                if (newTextureSize.x > Texture::getMaxSize() || newTextureSize.y > Texture::getMaxSize()) {
                    std::cerr << "Failed to add a new character to the font: the maximum texture size has been reached\n";
                    return {{0, 0}, {2, 2}};
                }
            } while (!canFitIntoTextureOfSize(newTextureSize));

            Texture newTexture = makeTexture(newTextureSize);
            newTexture.updateData2D(m_texture);
            m_texture = std::move(newTexture);
        }

        m_rows.push_back({nextRowBottom, {0, rowHeight}});
        rowToInsert = &m_rows.back();
    }

    const glm::vec<2, ft::pixelCoord_t> rectMin = {rowToInsert->size.x, rowToInsert->bottom};
    rowToInsert->size.x += rectSize.x;
    return {rectMin, rectMin + rectSize};
}