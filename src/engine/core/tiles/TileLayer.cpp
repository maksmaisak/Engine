//
// Created by Maksym Maisak on 2019-09-22.
//

#include "TileLayer.h"

using namespace en;

TileLayerChunk::TileLayerChunk() {

    tiles.fill({});
}

Tile& TileLayer::at(const GridPosition& tileCoordinates) {

    const GridPosition chunkCoordinates = getChunkCoordinates(tileCoordinates);

    const TileLayerChunk& chunk = getOrMakeChunk(chunkCoordinates);

    const GridPosition tileCoordinatesInChunk = tileCoordinates - chunkCoordinates * GridPosition(TileLayerChunk::ChunkSize);
    return chunk.at(tileCoordinatesInChunk.x, tileCoordinatesInChunk.y);
}

GridPosition TileLayer::getChunkCoordinates(const GridPosition& tileCoordinates) {

    constexpr auto chunkSize = GridCoordinate(TileLayerChunk::ChunkSize);

    const GridPosition chunkCoordinates {
        tileCoordinates.x >= 0 ? tileCoordinates.x / chunkSize : (tileCoordinates.x + 1) / chunkSize - 1,
        tileCoordinates.y >= 0 ? tileCoordinates.y / chunkSize : (tileCoordinates.y + 1) / chunkSize - 1
    };

    return chunkCoordinates;
}

TileLayerChunk& TileLayer::getOrMakeChunk(const GridPosition& chunkCoordinates) {

    const auto it = m_chunks.find(chunkCoordinates);
    if (it == m_chunks.end()) {
        return makeChunk(chunkCoordinates);
    }

    return it->second;
}

TileLayerChunk& TileLayer::makeChunk(const GridPosition& chunkCoordinates) {

    static const auto makeTileAt = [](const GridPosition& tileCoordinates) {

        Tile tile = {};

        if (glm::sin(tileCoordinates.x * 0.25f) * glm::sin(tileCoordinates.y * 0.25f) > 0.5f) {

            tile.atlasCoordinates = {1, 1};
            tile.isObstacle = true;
        }

        return tile;
    };

    TileLayerChunk& chunk = m_chunks[chunkCoordinates];
    const GridPosition coordinatesOffset = chunkCoordinates * TileLayerChunk::ChunkSize;
    for (std::size_t y = 0; y < TileLayerChunk::ChunkSize; ++y) {
        for (std::size_t x = 0; x < TileLayerChunk::ChunkSize; ++x) {
            chunk.at(x, y) = makeTileAt(coordinatesOffset + GridPosition(x, y));
        }
    }

    return chunk;
}
