//
// Created by Maksym Maisak on 2019-09-22.
//

#include "TileLayer.h"

using namespace en;

TileLayerChunk::TileLayerChunk() {

    tiles.fill({});
}

Tile& TileLayer::at(const Coordinates& tileCoordinates) {

    const Coordinates chunkCoordinates = getChunkCoordinates(tileCoordinates);

    const TileLayerChunk& chunk = getOrMakeChunk(chunkCoordinates);

    const Coordinates tileCoordinatesInChunk = tileCoordinates - chunkCoordinates * Coordinates(TileLayerChunk::ChunkSize);
    return chunk.at(tileCoordinatesInChunk.x, tileCoordinatesInChunk.y);
}

TileLayer::Coordinates TileLayer::getChunkCoordinates(const Coordinates& tileCoordinates) {

    constexpr auto chunkSize = Coordinates::value_type(TileLayerChunk::ChunkSize);

    const Coordinates chunkCoordinates = {
        tileCoordinates.x >= 0 ? tileCoordinates.x / chunkSize : (tileCoordinates.x + 1) / chunkSize - 1,
        tileCoordinates.y >= 0 ? tileCoordinates.y / chunkSize : (tileCoordinates.y + 1) / chunkSize - 1
    };

    return chunkCoordinates;
}

TileLayerChunk& TileLayer::getOrMakeChunk(const TileLayer::Coordinates& chunkCoordinates) {

    const auto it = m_chunks.find(chunkCoordinates);
    if (it == m_chunks.end()) {
        return makeChunk(chunkCoordinates);
    }

    return it->second;
}

TileLayerChunk& TileLayer::makeChunk(const Coordinates& chunkCoordinates) {

    static const auto makeTileAt = [](const Coordinates& tileCoordinates) {

        Tile tile = {};

        if (glm::sin(tileCoordinates.x * 0.25f) * glm::sin(tileCoordinates.y * 0.25f) > 0.5f) {

            tile.atlasCoordinates = {1, 1};
            tile.isObstacle = true;
        }

        return tile;
    };

    TileLayerChunk& chunk = m_chunks[chunkCoordinates];
    const Coordinates coordinatesOffset = chunkCoordinates * TileLayerChunk::ChunkSize;
    for (std::size_t y = 0; y < TileLayerChunk::ChunkSize; ++y) {
        for (std::size_t x = 0; x < TileLayerChunk::ChunkSize; ++x) {
            chunk.at(x, y) = makeTileAt(coordinatesOffset + Coordinates(x, y));
        }
    }

    return chunk;
}
