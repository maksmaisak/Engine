#version 330

uniform sampler2D mapDataTexture;
uniform sampler2D tileAtlas;

precision highp float;
precision highp vec2;
precision highp vec4;

in vec2 texCoord;
out vec4 fragmentColor;

vec2 getTileAtlasUV() {


    vec2 numTilesInMapDataTexture = vec2(40);
    vec2 invNumTilesInMapDataTexture = 1.f / numTilesInMapDataTexture;
    vec2 numTilesInAtlas = vec2(11, 6);
    vec2 invNumTilesInAtlas = 1.f / numTilesInAtlas;

    vec2 indicesInMapDataTexture = texCoord * numTilesInMapDataTexture;
    vec2 indicesInMapDataTextureFloored = floor(indicesInMapDataTexture);

    vec2 tileIndexOriginInAtlas = floor(255.f * texture(mapDataTexture, indicesInMapDataTextureFloored * invNumTilesInMapDataTexture).xy);
    vec2 tileIndexOffsetInAtlas = indicesInMapDataTexture - indicesInMapDataTextureFloored;

    return (tileIndexOriginInAtlas + tileIndexOffsetInAtlas) * invNumTilesInAtlas;

/*
    // hardcoded here, but could be passed as uniform just as well
    float cols = 10.0;
    float rows = 10.0;

    // texture coordinate is between (0, 1), round it off
    // to integer column/row
    vec2 tilePos = vec2(
        floor(texCoord.x * cols),
        floor(texCoord.y * rows)
    );
    // retrieve data for our current tile
    vec4 tileSpec = texture(mapDataTexture, tilePos / vec2(cols, rows));

    // our tile atlas here is 16x16 tiles
    float tilesPerSideX = 11.0;
    float tilesPerSideY = 6.0;

    // bottom-left corner of the tile in the atlas
    vec2 texPos = vec2(
        floor(tileSpec.x * tilesPerSideX) / tilesPerSideX,
        floor(tileSpec.y * tilesPerSideY) / tilesPerSideY
    );
    // 'where we are in the tile' - remember, we're doing
    // this work for a single pixel.
    vec2 texOffset = vec2(
        ((texCoord.x * cols) - tilePos.x) / tilesPerSideX,
        ((texCoord.y * rows) - tilePos.y) / tilesPerSideY
    );

    return texPos + texOffset;*/
}

void main() {

    fragmentColor = texture(tileAtlas, getTileAtlasUV());
    //fragmentColor = vec4(fragmentColor.rgb * 0.000001f, 1.f) + vec4(texture(mapDataTexture, texCoord).rgb, 1.f);
}
