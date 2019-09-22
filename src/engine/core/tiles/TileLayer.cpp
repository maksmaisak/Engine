//
// Created by Maksym Maisak on 2019-09-22.
//

#include "TileLayer.h"

using namespace en;

TileLayerChunk::TileLayerChunk() {

    Tile tile;
    tile.atlasCoordinates = {1, 5};
    tiles.fill(tile);
}

Tile& TileLayer::at(const Coordinates& coordinates) {

    constexpr auto chunkSize = Coordinates::value_type(TileLayerChunk::ChunkSize);

    const Coordinates chunkCoordinates = {
        coordinates.x >= 0 ? coordinates.x / chunkSize : (coordinates.x + 1) / chunkSize - 1,
        coordinates.y >= 0 ? coordinates.y / chunkSize : (coordinates.y + 1) / chunkSize - 1
    };
    const TileLayerChunk& chunk = m_chunks[chunkCoordinates];
    const Coordinates tileCoordinatesInChunk = coordinates - chunkCoordinates * Coordinates(TileLayerChunk::ChunkSize);
    return chunk.at(tileCoordinatesInChunk.x, tileCoordinatesInChunk.y);
}