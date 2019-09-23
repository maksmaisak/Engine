#version 330

precision highp float;
precision highp vec2;
precision highp vec4;

uniform sampler2D mapDataTexture;
uniform sampler2D tileAtlas;

uniform vec2 mapDataTextureResolution;
uniform vec2 invMapDataTextureResolution;
uniform vec2 numTilesInAtlas;
uniform vec2 invNumTilesInAtlas;
uniform vec2 atlasPixelSizeInUVSpaceRelativeToTile;

in vec2 texCoord;
out vec4 fragmentColor;

vec2 getTileAtlasUV() {

    vec2 indicesInMapDataTexture = texCoord * mapDataTextureResolution;
    vec2 indicesInMapDataTextureFloored = floor(indicesInMapDataTexture);

    vec2 tileOriginInAtlas = texture(mapDataTexture, (indicesInMapDataTextureFloored + 0.5f) * invMapDataTextureResolution).xy;
    vec2 tileIndexOriginInAtlas = floor(255.f * tileOriginInAtlas);

    vec2 tileIndexOffsetInAtlas = indicesInMapDataTexture - indicesInMapDataTextureFloored;
    // Prevent texture bleed in a tightly packed atlas.
    tileIndexOffsetInAtlas = atlasPixelSizeInUVSpaceRelativeToTile * 0.5f + tileIndexOffsetInAtlas * (1.f - atlasPixelSizeInUVSpaceRelativeToTile);

    return (tileIndexOriginInAtlas + tileIndexOffsetInAtlas) * invNumTilesInAtlas;
}

void main() {

    fragmentColor = texture(tileAtlas, getTileAtlasUV());
    //fragmentColor = vec4(fragmentColor.rgb * 0.000001f, 1.f) + vec4(texture(mapDataTexture, texCoord).rgb, 1.f);
}
