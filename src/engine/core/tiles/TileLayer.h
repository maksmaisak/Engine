//
// Created by Maksym Maisak on 2019-09-21.
//

#ifndef ENGINE_TILELAYER_H
#define ENGINE_TILELAYER_H

#include <array>
#include <unordered_map>
#include "glm.h"

// Implementation of hash for vec2 so we can use it as key in unordered_map
namespace std {

    template<typename genType>
    struct hash<glm::vec<2, genType>> {

        using argument_type = glm::vec<2, genType>;
        using result_type = std::size_t;

        result_type operator()(const argument_type& vector) const noexcept {

            const result_type hashX = std::hash<genType>{}(vector.x);
            const result_type hashY = std::hash<genType>{}(vector.y);
            return hashX ^ (hashY << 1);
        }
    };
}

namespace en {

    struct Tile final {

        glm::vec<2, int32_t> atlasCoordinates = {0, 0};
        bool testData = false;
    };

    struct TileLayerChunk final {

        static constexpr std::size_t ChunkSize = 16;

        TileLayerChunk();

        inline Tile& at(std::size_t x, std::size_t y) const {
            return const_cast<Tile&>(tiles.at(x + ChunkSize * y));
        }

        std::array<Tile, ChunkSize * ChunkSize> tiles;
    };

    class TileLayer final {

    public:

        using Coordinates = glm::vec<2, int64_t>;

        Tile& at(const Coordinates& coordinates);

    private:

        std::unordered_map<Coordinates, TileLayerChunk> m_chunks;
    };
}

#endif //ENGINE_TILELAYER_H
