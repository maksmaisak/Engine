//
// Created by Maksym Maisak on 12/11/19.
//

#include "Font.h"
#include <cassert>
#include <type_traits>

using namespace en;

namespace {

    void deleteLibrary(FT_Library library) {

        if (!library) {
            return;
        }

        //std::cout << "Freetype: deleting library at: " << library << std::endl;
        const FT_Error error = FT_Done_FreeType(library);
        assert(!error && "Freetype: failed to delete library");
    }

    void deleteFace(FT_Face face) {

        if (!face) {
            return;
        }

        //std::cout << "deleting Freetype face at: " << face << std::endl;
        const FT_Error error = FT_Done_Face(face);
        assert(!error && "Freetype: failed to delete face");
    }

    FT_Library getLibrary() {

        using library_t = std::remove_pointer_t<FT_Library>;
        static auto library = []() -> std::unique_ptr<library_t, decltype(&deleteLibrary)> {

            FT_Library lib = nullptr;
            const FT_Error error = FT_Init_FreeType(&lib);
            assert(!error && "Freetype: failed to init library");

            return {lib, deleteLibrary};
        }();

        return library.get();
    }

    std::shared_ptr<FT_FaceRec> makeFace(const std::string& filename) {

        FT_Face face;
        if (const FT_Error error = FT_New_Face(getLibrary(), filename.c_str(), 0, &face)) {

            std::cerr << "Freetype: Failed to load font: error" << FT_Error_String(error) << std::endl;
            return {nullptr, deleteFace};
        }

        return {face, deleteFace};
    }
}

Font::Font(const Name& filename) :
    m_filename(filename),
    m_face(makeFace(filename.getString()))
{
    assert(m_face);

    if (const FT_Error error = FT_Select_Charmap(m_face.get(), FT_ENCODING_UNICODE)) {
        std::cerr << "Freetype: failed to load font: failed to select unicode encoding: error " << FT_Error_String(error);
    }
}

const Texture& Font::getTexture(ft::fontsize_t characterSize) const {

    FontAtlas& atlas = getOrAddAtlasFor(characterSize);
    return atlas.getTexture();
}

const Glyph& Font::getGlyph(ft::codePoint_t codePoint, ft::fontsize_t characterSize) const {

    FontAtlas& atlas = getOrAddAtlasFor(characterSize);
    return atlas.getGlyph(codePoint);
}

FontAtlas& Font::getOrAddAtlasFor(ft::fontsize_t characterSize) const {

    const auto foundIt = m_atlases.find(characterSize);
    if (foundIt != m_atlases.end()) {
        return foundIt->second;
    }

    const auto [it, didEmplace] = m_atlases.emplace(characterSize, FontAtlas(characterSize, m_face));
    assert(didEmplace);
    return it->second;
}

float Font::getLineSpacing(ft::fontsize_t characterSize) const {

    if (m_face && ft::setCurrentSizeOf(m_face.get(), characterSize)) {
        return ft::posToFloat(m_face->size->metrics.height);
    }

    return 0.f;
}

float Font::getKerning(ft::codePoint_t first, ft::codePoint_t second, ft::fontsize_t characterSize) const {

    if (first == 0 || second == 0) {
        return 0.f;
    }

    if (!m_face || !FT_HAS_KERNING(m_face.get()) || !ft::setCurrentSizeOf(m_face.get(), characterSize)) {
        return 0.f;
    }

    const FT_UInt index1 = FT_Get_Char_Index(m_face.get(), first);
    const FT_UInt index2 = FT_Get_Char_Index(m_face.get(), second);
    FT_Vector kerning;
    if (FT_Get_Kerning(m_face.get(), index1, index2, FT_KERNING_DEFAULT, &kerning)) {
        return 0.f;
    }

    return FT_IS_SCALABLE(m_face.get()) ?
        ft::posToFloat(kerning.x) :
        static_cast<float>(kerning.x);
}

