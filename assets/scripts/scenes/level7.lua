require('assets/scripts/level/level')

local materials = {
    Game.makeMaterial {
        shader = 'pbr',
        albedo = 'objects/Player/Texture/Main_Character_01_AlbedoTransparency.png',
        metallicSmoothness = 'objects/Player/Texture/Main_Character_01_MetallicSmoothness.png',
        ao = 'objects/Player/Texture/Main_Character_01_AO.png',
        normal = 'objects/Player/Texture/Main_Character_01_Normal.png',
        smoothnessMultiplier = 1,
        aoMultiplier = 1,
    },
    Game.makeMaterial {
        shader = 'pbr',
        albedo = 'objects/tile/Blocks/FoundationBlock/Textures/AlbedoTransparency 1.png',
        metallicSmoothness = 'objects/tile/Blocks/FoundationBlock/Textures/MetallicSmoothness.png',
        ao = 'objects/tile/Blocks/FoundationBlock/Textures/Grayscale.png',
        normal = 'objects/tile/Blocks/FoundationBlock/Textures/Normal.png',
        smoothnessMultiplier = 1,
        aoMultiplier = 1,
    },
    Game.makeMaterial {
        shader = 'pbr',
        albedo = 'objects/ExitFinish/InsideExit_initialShadingGroup_AlbedoTransparency.png',
        metallicSmoothness = 'objects/ExitFinish/InsideExit_initialShadingGroup_MetallicSmoothness.png',
        smoothnessMultiplier = 1,
        aoMultiplier = 1,
    },
    Game.makeMaterial {
        shader = 'pbr',
        albedo = 'objects/ExitFinish/Textures/AlbedoTransparency.png',
        metallicSmoothness = 'objects/ExitFinish/Textures/MetallicSmoothness.png',
        ao = 'objects/ExitFinish/Textures/Grayscale.png',
        normal = 'objects/ExitFinish/Textures/Normal.png',
        smoothnessMultiplier = 1,
        aoMultiplier = 1,
    },
    Game.makeMaterial {
        shader = 'pbr',
        albedo = 'objects/tile/Blocks/TerrainBlockVar3/Texture/TerrainBlockV3_AlbedoTransparency.png',
        metallicSmoothness = 'objects/tile/Blocks/TerrainBlockVar3/Texture/TerrainBlockV3_MetallicSmoothness.png',
        ao = 'objects/tile/Blocks/TerrainBlockVar3/Texture/TerrainBlockV3_AO.png',
        normal = 'objects/tile/Blocks/TerrainBlockVar3/Texture/TerraainBlockV3.png',
        smoothnessMultiplier = 1,
        aoMultiplier = 1,
    },
    Game.makeMaterial {
        shader = 'pbr',
        albedo = 'objects/decorations/MenuFloor/Texture/UvMappedFlooring_initialShadingGroup_AlbedoTransparency.png',
        metallicSmoothness = 'objects/decorations/MenuFloor/Texture/UvMappedFlooring_initialShadingGroup_MetallicSmoothness.png',
        normal = 'objects/decorations/MenuFloor/Texture/UvMappedFlooring_initialShadingGroup_Normal.png',
        smoothnessMultiplier = 1,
        aoMultiplier = 1,
    },
    Game.makeMaterial {
        shader = 'pbr',
        albedo = 'objects/tile/Blocks/TerrainBlockVar1/Texture/TerrainBlockV1_AlbedoTransparency.png',
        metallicSmoothness = 'objects/tile/Blocks/TerrainBlockVar1/Texture/TerrainBlockV1_MetallicSmoothness.png',
        ao = 'objects/tile/Blocks/TerrainBlockVar1/Texture/TerrainBlockV1_AO.png',
        normal = 'objects/tile/Blocks/TerrainBlockVar1/Texture/TerraainBlockV1.png',
        smoothnessMultiplier = 1,
        aoMultiplier = 1,
    },
    Game.makeMaterial {
        shader = 'pbr',
        albedo = 'objects/tile/Blocks/TerrainBlockVar2/Texture/TerrainBlockV2_AlbedoTransparency.png',
        metallicSmoothness = 'objects/tile/Blocks/TerrainBlockVar2/Texture/TerrainBlockV2_MetallicSmoothness.png',
        ao = 'objects/tile/Blocks/TerrainBlockVar2/Texture/TerrainBlockV2_AO.png',
        smoothnessMultiplier = 1,
        aoMultiplier = 1,
    },
    Game.makeMaterial {
        shader = 'pbr',
        renderMode = 'cutout',
        albedo = 'objects/decorations/Plants/Material/Plant_All (3).tga',
        normal = 'objects/decorations/Plants/Material/Plant_All_NormalMap.png',
        metallicMultiplier = 0,
        smoothnessMultiplier = 0,
        aoMultiplier = 1,
    },
    Game.makeMaterial {
        shader = 'pbr',
        albedo = 'objects/tile/Blocks/TerrainBlockVar4/Texture/TerrainBlockV4_AlbedoTransparency.png',
        metallicSmoothness = 'objects/tile/Blocks/TerrainBlockVar4/Texture/TerrainBlockV4_MetallicSmoothness.png',
        ao = 'objects/tile/Blocks/TerrainBlockVar4/Texture/TerrainBlockV4_AO.png',
        normal = 'objects/tile/Blocks/TerrainBlockVar4/Texture/TerraainBlockV4.png',
        smoothnessMultiplier = 1,
        aoMultiplier = 1,
    },
    Game.makeMaterial {
        shader = 'pbr',
        albedo = 'objects/tile/ButtonTile/Texture/ButtonTile_AlbedoTransparency.png',
        metallicSmoothness = 'objects/tile/ButtonTile/Texture/ButtonTile_MetallicSmoothness.png',
        ao = 'objects/tile/ButtonTile/Texture/ButtonTile_AO.png',
        normal = 'objects/tile/ButtonTile/Texture/ButtonTile_Normal.png',
        smoothnessMultiplier = 1,
        aoMultiplier = 1,
    },
    Game.makeMaterial {
        shader = 'pbr',
        metallicMultiplier = 0,
        smoothnessMultiplier = 0.5,
        aoMultiplier = 1,
    },
    Game.makeMaterial {
        shader = 'pbr',
        albedo = 'objects/laser/Texture/AlbedoTransparency (1).png',
        metallicSmoothness = 'objects/laser/Texture/MetallicSmoothness.png',
        ao = 'objects/laser/Texture/Grayscale.png',
        normal = 'objects/laser/Texture/Normal.png',
        smoothnessMultiplier = 1,
        aoMultiplier = 1,
    },
    Game.makeMaterial {
        shader = 'pbr',
        albedo = 'objects/Door/Texture/Door_01_AlbedoTransparency 1.png',
        metallicSmoothness = 'objects/Door/Texture/Door_01_MetallicSmoothness.png',
        ao = 'objects/Door/Texture/Door_01_AO.png',
        normal = 'objects/Door/Texture/Door_01_Normal.png',
        smoothnessMultiplier = 1,
        aoMultiplier = 1,
    },
    Game.makeMaterial {
        shader = 'pbr',
        albedo = 'objects/Door/Texture/DoorPilars_01_AlbedoTransparency 1.png',
        metallicSmoothness = 'objects/Door/Texture/DoorPilars_01_MetallicSmoothness.png',
        ao = 'objects/Door/Texture/DoorPilars_01_AO.png',
        normal = 'objects/Door/Texture/DoorPilars_01_Normal.png',
        smoothnessMultiplier = 1,
        aoMultiplier = 1,
    },
}

local map = Map:new {
    gridSize = {x = 5, y = 14},
    grid = {
        {{},{},{},{},{},{},{},{},{},{},{},{},{},{}},
        {{},{},{},{},{},{},{},{},{},{},{},{},{},{}},
        {{},{},{},{},{},{},{},{},{},{},{},{},{},{}},
        {{},{},{},{},{},{},{},{},{},{},{},{},{},{}},
        {{},{},{},{},{},{},{},{},{},{},{},{},{},{}},
    }
}

local grid = map.grid

grid[1][10].tile = {
    Name = "Tile 2 (2)",
    Transform = {
        position = {0, 0, 9},
        children = {
            {
                Name = "TerrainBlockV3",
                Transform = {
                    position = {0, -1, -0.337},
                    children = {
                        {
                            Name = "TerrainBlockV3LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar3/TerrainBlockV3.obj',
                                material = materials[5],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[1][11].tile = {
    Name = "Tile 2 (3)",
    Transform = {
        position = {0, 0, 10},
        children = {
            {
                Name = "TerrainBlockV3",
                Transform = {
                    position = {0, -1, -0.337},
                    children = {
                        {
                            Name = "TerrainBlockV3LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar3/TerrainBlockV3.obj',
                                material = materials[5],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[1][12].tile = {
    Name = "Tile 1",
    Transform = {
        position = {0, 0, 11},
        children = {
            {
                Name = "TerrainBlockV2",
                Transform = {
                    position = {0, -1, -0.148},
                    children = {
                        {
                            Name = "TerrainBlockV2LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar2/TerrainBlockV2.obj',
                                material = materials[8],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[1][13].tile = {
    Name = "Tile",
    Transform = {
        position = {0, 0, 12},
        children = {
            {
                Name = "TerrainBlockV1",
                Transform = {
                    position = {0, -1, -0.3},
                    children = {
                        {
                            Name = "TerrainBlockV1LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar1/TerrainBlockV1.obj',
                                material = materials[7],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[1][14].tile = {
    Name = "Tile",
    Transform = {
        position = {0, 0, 13},
        children = {
            {
                Name = "TerrainBlockV1",
                Transform = {
                    position = {0, -1, -0.3},
                    children = {
                        {
                            Name = "TerrainBlockV1LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar1/TerrainBlockV1.obj',
                                material = materials[7],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[2][7].tile = {
    Name = "Tile",
    Transform = {
        position = {1, -0.002000004, 6},
        children = {
            {
                Name = "TerrainBlockV1",
                Transform = {
                    position = {0, -1, -0.3},
                    children = {
                        {
                            Name = "TerrainBlockV1LowPoly1",
                            Transform = {
                            },
                        },
                    }
                },
            },
        }
    },
}

grid[2][8].tile = {
    Name = "Tile",
    Transform = {
        position = {1, 0, 7},
        children = {
            {
                Name = "TerrainBlockV1",
                Transform = {
                    position = {0, -1, -0.3},
                    children = {
                        {
                            Name = "TerrainBlockV1LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar1/TerrainBlockV1.obj',
                                material = materials[7],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[2][9].tile = {
    Name = "Tile 3",
    Transform = {
        position = {1, 0, 8},
        children = {
            {
                Name = "TerrainBlockV4",
                Transform = {
                    position = {0, -1, -0.146},
                    children = {
                        {
                            Name = "TerrainBlockV4LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar4/TerrainBlockV4.obj',
                                material = materials[10],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[2][10].tile = {
    Name = "Tile 3",
    Transform = {
        position = {1, 0, 9},
        children = {
            {
                Name = "TerrainBlockV4",
                Transform = {
                    position = {0, -1, -0.146},
                    children = {
                        {
                            Name = "TerrainBlockV4LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar4/TerrainBlockV4.obj',
                                material = materials[10],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[2][11].tile = {
    Name = "Tile 2",
    Transform = {
        position = {1, 0, 10},
        children = {
            {
                Name = "TerrainBlockV3",
                Transform = {
                    position = {0, -1, -0.337},
                    children = {
                        {
                            Name = "TerrainBlockV3LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar3/TerrainBlockV3.obj',
                                material = materials[5],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[2][12].tile = {
    Name = "Tile 1",
    Transform = {
        position = {1, 0, 11},
        children = {
            {
                Name = "TerrainBlockV2",
                Transform = {
                    position = {0, -1, -0.148},
                    children = {
                        {
                            Name = "TerrainBlockV2LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar2/TerrainBlockV2.obj',
                                material = materials[8],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[2][13].tile = {
    Name = "Tile",
    Transform = {
        position = {1, 0, 12},
        children = {
            {
                Name = "TerrainBlockV1",
                Transform = {
                    position = {0, -1, -0.3},
                    children = {
                        {
                            Name = "TerrainBlockV1LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar1/TerrainBlockV1.obj',
                                material = materials[7],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[2][14].tile = {
    Name = "Tile 3",
    Transform = {
        position = {1, 0, 13},
        children = {
            {
                Name = "TerrainBlockV4",
                Transform = {
                    position = {0, -1, -0.146},
                    children = {
                        {
                            Name = "TerrainBlockV4LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar4/TerrainBlockV4.obj',
                                material = materials[10],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[3][2].tile = {
    Name = "Tile",
    Transform = {
        position = {2, 0, 1},
        children = {
            {
                Name = "TerrainBlockV1",
                Transform = {
                    position = {0, -1, -0.3},
                    children = {
                        {
                            Name = "TerrainBlockV1LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar1/TerrainBlockV1.obj',
                                material = materials[7],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[3][3].tile = {
    Name = "Tile 3",
    Transform = {
        position = {2, 0, 2},
        children = {
            {
                Name = "TerrainBlockV4",
                Transform = {
                    position = {0, -1, -0.146},
                    children = {
                        {
                            Name = "TerrainBlockV4LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar4/TerrainBlockV4.obj',
                                material = materials[10],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[3][4].tile = {
    Name = "Tile 2",
    Transform = {
        position = {2, 0, 3},
        children = {
            {
                Name = "TerrainBlockV3",
                Transform = {
                    position = {0, -1, -0.337},
                    children = {
                        {
                            Name = "TerrainBlockV3LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar3/TerrainBlockV3.obj',
                                material = materials[5],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[3][5].tile = {
    Name = "Tile 3",
    Transform = {
        position = {2, 0, 4},
        children = {
            {
                Name = "TerrainBlockV4",
                Transform = {
                    position = {0, -1, -0.146},
                    children = {
                        {
                            Name = "TerrainBlockV4LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar4/TerrainBlockV4.obj',
                                material = materials[10],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[3][6].tile = {
    Name = "Tile",
    Transform = {
        position = {2, 0, 5},
        children = {
            {
                Name = "TerrainBlockV1",
                Transform = {
                    position = {0, -1, -0.3},
                    children = {
                        {
                            Name = "TerrainBlockV1LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar1/TerrainBlockV1.obj',
                                material = materials[7],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[3][7].tile = {
    Name = "Tile 2",
    Transform = {
        position = {2, 0, 6},
        children = {
            {
                Name = "TerrainBlockV3",
                Transform = {
                    position = {0, -1, -0.337},
                    children = {
                        {
                            Name = "TerrainBlockV3LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar3/TerrainBlockV3.obj',
                                material = materials[5],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[3][8].tile = {
    Name = "Tile 3",
    Transform = {
        position = {2, 0, 7},
        children = {
            {
                Name = "TerrainBlockV4",
                Transform = {
                    position = {0, -1, -0.146},
                    children = {
                        {
                            Name = "TerrainBlockV4LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar4/TerrainBlockV4.obj',
                                material = materials[10],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[3][9].tile = {
    Name = "Tile 2",
    Transform = {
        position = {2, 0, 8},
        children = {
            {
                Name = "TerrainBlockV3",
                Transform = {
                    position = {0, -1, -0.337},
                    children = {
                        {
                            Name = "TerrainBlockV3LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar3/TerrainBlockV3.obj',
                                material = materials[5],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[3][10].tile = {
    Name = "Tile 2 (1)",
    Transform = {
        position = {2, 0, 9},
        children = {
            {
                Name = "TerrainBlockV3",
                Transform = {
                    position = {0, -1, -0.337},
                    children = {
                        {
                            Name = "TerrainBlockV3LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar3/TerrainBlockV3.obj',
                                material = materials[5],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[3][11].tile = {
    Name = "Tile 2 (4)",
    Transform = {
        position = {2, 0, 10},
        children = {
            {
                Name = "TerrainBlockV3",
                Transform = {
                    position = {0, -1, -0.337},
                    children = {
                        {
                            Name = "TerrainBlockV3LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar3/TerrainBlockV3.obj',
                                material = materials[5],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[3][12].tile = {
    Name = "Tile 2",
    Transform = {
        position = {2, 0, 11},
        children = {
            {
                Name = "TerrainBlockV3",
                Transform = {
                    position = {0, -1, -0.337},
                    children = {
                        {
                            Name = "TerrainBlockV3LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar3/TerrainBlockV3.obj',
                                material = materials[5],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[3][13].tile = {
    Name = "Tile 1",
    Transform = {
        position = {2, 0, 12},
        children = {
            {
                Name = "TerrainBlockV2",
                Transform = {
                    position = {0, -1, -0.148},
                    children = {
                        {
                            Name = "TerrainBlockV2LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar2/TerrainBlockV2.obj',
                                material = materials[8],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[3][14].tile = {
    Name = "Tile 1",
    Transform = {
        position = {2, 0, 13},
        children = {
            {
                Name = "TerrainBlockV2",
                Transform = {
                    position = {0, -1, -0.148},
                    children = {
                        {
                            Name = "TerrainBlockV2LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar2/TerrainBlockV2.obj',
                                material = materials[8],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[4][1].tile = {
    Name = "Tile 3",
    Transform = {
        position = {3, 0, 0},
        children = {
            {
                Name = "TerrainBlockV4",
                Transform = {
                    position = {0, -1, -0.146},
                    children = {
                        {
                            Name = "TerrainBlockV4LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar4/TerrainBlockV4.obj',
                                material = materials[10],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[4][2].tile = {
    Name = "Tile 1",
    Transform = {
        position = {3, 0, 1},
        children = {
            {
                Name = "TerrainBlockV2",
                Transform = {
                    position = {0, -1, -0.148},
                    children = {
                        {
                            Name = "TerrainBlockV2LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar2/TerrainBlockV2.obj',
                                material = materials[8],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[4][3].tile = {
    Name = "Tile 1",
    Transform = {
        position = {3, 0, 2},
        children = {
            {
                Name = "TerrainBlockV2",
                Transform = {
                    position = {0, -1, -0.148},
                    children = {
                        {
                            Name = "TerrainBlockV2LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar2/TerrainBlockV2.obj',
                                material = materials[8],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[4][4].tile = {
    Name = "Tile 1",
    Transform = {
        position = {3, 0, 3},
        children = {
            {
                Name = "TerrainBlockV2",
                Transform = {
                    position = {0, -1, -0.148},
                    children = {
                        {
                            Name = "TerrainBlockV2LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar2/TerrainBlockV2.obj',
                                material = materials[8],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[4][5].tile = {
    Name = "Tile",
    Transform = {
        position = {3, -0.002000004, 4},
        children = {
            {
                Name = "TerrainBlockV1",
                Transform = {
                    position = {0, -1, -0.3},
                    children = {
                        {
                            Name = "TerrainBlockV1LowPoly1",
                            Transform = {
                            },
                        },
                    }
                },
            },
        }
    },
}

grid[4][6].tile = {
    Name = "Tile 3",
    Transform = {
        position = {3, 0, 5},
        children = {
            {
                Name = "TerrainBlockV4",
                Transform = {
                    position = {0, -1, -0.146},
                    children = {
                        {
                            Name = "TerrainBlockV4LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar4/TerrainBlockV4.obj',
                                material = materials[10],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[4][7].tile = {
    Name = "Tile 1",
    Transform = {
        position = {3, 0, 6},
        children = {
            {
                Name = "TerrainBlockV2",
                Transform = {
                    position = {0, -1, -0.148},
                    children = {
                        {
                            Name = "TerrainBlockV2LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar2/TerrainBlockV2.obj',
                                material = materials[8],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[4][8].tile = {
    Name = "Tile 1",
    Transform = {
        position = {3, 0, 7},
        children = {
            {
                Name = "TerrainBlockV2",
                Transform = {
                    position = {0, -1, -0.148},
                    children = {
                        {
                            Name = "TerrainBlockV2LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar2/TerrainBlockV2.obj',
                                material = materials[8],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[4][9].tile = {
    Name = "Tile",
    Transform = {
        position = {3, 0, 8},
        children = {
            {
                Name = "TerrainBlockV1",
                Transform = {
                    position = {0, -1, -0.3},
                    children = {
                        {
                            Name = "TerrainBlockV1LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar1/TerrainBlockV1.obj',
                                material = materials[7],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[5][3].tile = {
    Name = "Tile",
    Transform = {
        position = {4, 0, 2},
        children = {
            {
                Name = "TerrainBlockV1",
                Transform = {
                    position = {0, -1, -0.3},
                    children = {
                        {
                            Name = "TerrainBlockV1LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar1/TerrainBlockV1.obj',
                                material = materials[7],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[5][4].tile = {
    Name = "Tile 1",
    Transform = {
        position = {4, 0, 3},
        children = {
            {
                Name = "TerrainBlockV2",
                Transform = {
                    position = {0, -1, -0.148},
                    children = {
                        {
                            Name = "TerrainBlockV2LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar2/TerrainBlockV2.obj',
                                material = materials[8],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[5][5].tile = {
    Name = "Tile 2",
    Transform = {
        position = {4, 0, 4},
        children = {
            {
                Name = "TerrainBlockV3",
                Transform = {
                    position = {0, -1, -0.337},
                    children = {
                        {
                            Name = "TerrainBlockV3LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar3/TerrainBlockV3.obj',
                                material = materials[5],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[5][6].tile = {
    Name = "Tile",
    Transform = {
        position = {4, 0, 5},
        children = {
            {
                Name = "TerrainBlockV1",
                Transform = {
                    position = {0, -1, -0.3},
                    children = {
                        {
                            Name = "TerrainBlockV1LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar1/TerrainBlockV1.obj',
                                material = materials[7],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[5][7].tile = {
    Name = "Tile",
    Transform = {
        position = {4, 0, 6},
        children = {
            {
                Name = "TerrainBlockV1",
                Transform = {
                    position = {0, -1, -0.3},
                    children = {
                        {
                            Name = "TerrainBlockV1LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar1/TerrainBlockV1.obj',
                                material = materials[7],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[5][8].tile = {
    Name = "Tile 1",
    Transform = {
        position = {4, 0, 7},
        children = {
            {
                Name = "TerrainBlockV2",
                Transform = {
                    position = {0, -1, -0.148},
                    children = {
                        {
                            Name = "TerrainBlockV2LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar2/TerrainBlockV2.obj',
                                material = materials[8],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[5][9].tile = {
    Name = "Tile 1 (1)",
    Transform = {
        position = {4, 0, 8},
        children = {
            {
                Name = "TerrainBlockV2",
                Transform = {
                    position = {0, -1, -0.148},
                    children = {
                        {
                            Name = "TerrainBlockV2LowPoly1",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/tile/Blocks/TerrainBlockVar2/TerrainBlockV2.obj',
                                material = materials[8],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[1][10].obstacle = {
    Name = "PillarRight",
    Transform = {
        position = {0, 0, 9},
        children = {
            {
                Name = "DoorPilars_03",
                Transform = {
                    position = {1.281, 0, -0.003225},
                    children = {
                        {
                            Name = "DoorPilars_01",
                            Transform = {
                                rotation = {0, 89.99983, 0},
                            },
                            RenderInfo = {
                                mesh = 'objects/Door/DoorPilars_03.obj',
                                material = materials[15],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[1][11].obstacle = {
    Name = "Obstacle (1)",
    Transform = {
        position = {0, 0, 10},
        children = {
            {
                Name = "FoundationBlock",
                Transform = {
                    position = {0.006759644, 0.01687413, -0.008},
                    children = {
                        {
                            Name = "TerrainBlockV3",
                            Transform = {
                                position = {0, 0, -0.36},
                                children = {
                                    {
                                        Name = "TerrainBlockV3LowPoly1",
                                        Transform = {
                                            position = {-0.3820662, 0, 0.3415085},
                                            rotation = {0, 89.99983, 0},
                                        },
                                    },
                                }
                            },
                        },
                    }
                },
            },
        }
    },
}

grid[1][12].obstacle = {
    Name = "Obstacle (2)",
    Transform = {
        position = {0, 0, 11},
        children = {
            {
                Name = "FoundationBlock",
                Transform = {
                    position = {0.006759644, 0.01687413, -0.008},
                    children = {
                        {
                            Name = "TerrainBlockV3",
                            Transform = {
                                position = {0, 0, -0.36},
                                children = {
                                    {
                                        Name = "TerrainBlockV3LowPoly1",
                                        Transform = {
                                            position = {-0.3820662, 0, 0.3415085},
                                            rotation = {0, 89.99983, 0},
                                        },
                                    },
                                }
                            },
                        },
                    }
                },
            },
        }
    },
}

grid[1][13].obstacle = {
    Name = "Obstacle (3)",
    Transform = {
        position = {0, 0, 12},
        children = {
            {
                Name = "FoundationBlock",
                Transform = {
                    position = {0.006759644, 0.01687413, -0.008},
                    children = {
                        {
                            Name = "TerrainBlockV3",
                            Transform = {
                                position = {0, 0, -0.36},
                                children = {
                                    {
                                        Name = "TerrainBlockV3LowPoly1",
                                        Transform = {
                                            position = {-0.3820662, 0, 0.3415085},
                                            rotation = {0, 89.99983, 0},
                                        },
                                    },
                                }
                            },
                        },
                    }
                },
            },
        }
    },
}

grid[2][13].obstacle = {
    Name = "Obstacle (1)",
    Transform = {
        position = {1, 0, 12},
        rotation = {0, 179.9997, 0},
        children = {
            {
                Name = "FoundationBlock",
                Transform = {
                    position = {0.006759644, 0.01687413, -0.008},
                    children = {
                        {
                            Name = "TerrainBlockV3",
                            Transform = {
                                position = {0, 0, -0.36},
                                children = {
                                    {
                                        Name = "TerrainBlockV3LowPoly1",
                                        Transform = {
                                            position = {-0.3820662, 0, 0.3415085},
                                            rotation = {0, 89.99983, 0},
                                        },
                                    },
                                }
                            },
                        },
                    }
                },
            },
        }
    },
}

grid[3][10].obstacle = {
    Name = "PillarLeft",
    Transform = {
        position = {2, 0, 9},
        children = {
            {
                Name = "DoorPilars_02",
                Transform = {
                    position = {-1.305, -0.004, 0.02299982},
                    rotation = {0, 89.99984, 0},
                    children = {
                        {
                            Name = "DoorPilars_02",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/Door/DoorPilars_02.obj',
                                material = materials[15],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[3][11].obstacle = {
    Name = "Obstacle (4)",
    Transform = {
        position = {2, 0, 10},
        children = {
            {
                Name = "FoundationBlock",
                Transform = {
                    position = {0.006759644, 0.01687413, -0.008},
                    children = {
                        {
                            Name = "TerrainBlockV3",
                            Transform = {
                                position = {0, 0, -0.36},
                                children = {
                                    {
                                        Name = "TerrainBlockV3LowPoly1",
                                        Transform = {
                                            position = {-0.3820662, 0, 0.3415085},
                                            rotation = {0, 89.99983, 0},
                                        },
                                    },
                                }
                            },
                        },
                    }
                },
            },
        }
    },
}

grid[3][12].obstacle = {
    Name = "Obstacle (5)",
    Transform = {
        position = {2, 0, 11},
        children = {
            {
                Name = "FoundationBlock",
                Transform = {
                    position = {0.006759644, 0.01687413, -0.008},
                    children = {
                        {
                            Name = "TerrainBlockV3",
                            Transform = {
                                position = {0, 0, -0.36},
                                children = {
                                    {
                                        Name = "TerrainBlockV3LowPoly1",
                                        Transform = {
                                            position = {-0.3820662, 0, 0.3415085},
                                            rotation = {0, 89.99983, 0},
                                        },
                                    },
                                }
                            },
                        },
                    }
                },
            },
        }
    },
}

grid[3][13].obstacle = {
    Name = "Obstacle (6)",
    Transform = {
        position = {2, 0, 12},
        children = {
            {
                Name = "FoundationBlock",
                Transform = {
                    position = {0.006759644, 0.01687413, -0.008},
                },
            },
        }
    },
}

grid[4][3].obstacle = {
    Name = "Obstacle",
    Transform = {
        position = {3, 0, 2},
        children = {
            {
                Name = "FoundationBlock",
                Transform = {
                    position = {0.006759644, 0.01687413, -0.008},
                    children = {
                        {
                            Name = "TerrainBlockV3",
                            Transform = {
                                position = {0, 0, -0.36},
                                children = {
                                    {
                                        Name = "TerrainBlockV3LowPoly1",
                                        Transform = {
                                            position = {-0.3820662, 0, 0.3415085},
                                            rotation = {0, 89.99983, 0},
                                        },
                                        RenderInfo = {
                                            mesh = 'objects/tile/Blocks/FoundationBlock/TerrainBlockV3.obj',
                                            material = materials[2],
                                            isBatchingStatic = true
                                        }
                                    },
                                }
                            },
                        },
                    }
                },
            },
        }
    },
}

grid[4][7].obstacle = {
    Name = "Obstacle",
    Transform = {
        position = {3, 0, 5.96},
        rotation = {0, 179.9997, 0},
        children = {
            {
                Name = "FoundationBlock",
                Transform = {
                    position = {0.006759644, 0.01687413, -0.008},
                    children = {
                        {
                            Name = "TerrainBlockV3",
                            Transform = {
                                position = {0, 0, -0.36},
                                children = {
                                    {
                                        Name = "TerrainBlockV3LowPoly1",
                                        Transform = {
                                            position = {-0.3820662, 0, 0.3415085},
                                            rotation = {0, 89.99983, 0},
                                        },
                                    },
                                }
                            },
                        },
                    }
                },
            },
        }
    },
}

grid[2][12].goal = {
    startActive = false,
    light = {
        Name = "Point Light",
        Transform = {
            position = {0, 1, 1},
        },
        Light = {
            kind = 'point',
            intensity = 6.283185,
            range = 5.16487,
            falloff = {1, 1, 0},
            color = {0, 1, 0, 1},
        }
    },
    actor = {
        Name = "LevelGoal",
        Transform = {
            position = {1, 0, 11},
            children = {
                {
                    Name = "ExitFinishModelLow",
                    Transform = {
                        position = {0, 0, 1},
                        rotation = {0, 179.9997, 0},
                        children = {
                            {
                                Name = "ExitFinishModelLow1",
                                Transform = {
                                    children = {
                                        {
                                            Name = "InsideExit",
                                            Transform = {
                                                position = {-0.005870819, 4.905462E-05, 0},
                                                rotation = {0, 0, 359.6352},
                                                children = {
                                                    {
                                                        Name = "default",
                                                        Transform = {
                                                        },
                                                        RenderInfo = {
                                                            mesh = 'objects/ExitFinish/InsideExit.obj',
                                                            material = materials[3],
                                                            isBatchingStatic = true
                                                        }
                                                    },
                                                }
                                            },
                                        },
                                    }
                                },
                                RenderInfo = {
                                    mesh = 'objects/ExitFinish/ExitFinishModelLow.obj',
                                    material = materials[4],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
            }
        },
    }
}

grid[2][7].button = {
    targetPositions = {
        {x = 2, y = 10},
    },
    actor = {
        Name = "Button (1)",
        Transform = {
            position = {1, 0.198, 6},
            scale = {0.3, 0.2, 0.3},
            children = {
                {
                    Name = "ButtonTile (1)",
                    Transform = {
                        position = {0, -5.66, 0},
                        scale = {3.333333, 5, 3.333333},
                        children = {
                            {
                                Name = "ButtonTile:Group59432",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/tile/ButtonTile/ButtonTile.obj',
                                    material = materials[11],
                                }
                            },
                        }
                    },
                },
            }
        },
    }
}

grid[4][5].button = {
    targetPositions = {
        {x = 2, y = 12},
        {x = 4, y = 7},
    },
    actor = {
        Name = "Button",
        Transform = {
            position = {3, 0.198, 4},
            scale = {0.3, 0.2, 0.3},
            children = {
                {
                    Name = "ButtonTile (1)",
                    Transform = {
                        position = {0, -5.66, 0},
                        scale = {3.333333, 5, 3.333333},
                        children = {
                            {
                                Name = "ButtonTile:Group59432",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/tile/ButtonTile/ButtonTile.obj',
                                    material = materials[11],
                                }
                            },
                        }
                    },
                },
            }
        },
    }
}

grid[2][10].door = {
    actor = {
        Name = "Door",
        Transform = {
            position = {1, 0, 9},
        },
    },
    swingLeft = {
        Name = "SwingLeft",
        Transform = {
            position = {0.5, 0, 0},
            children = {
                {
                    Name = "Door_01",
                    Transform = {
                        position = {-0.497, 0, -4.3889E-07},
                        rotation = {0, 270.0002, 0},
                        scale = {1, 1, 0.6418437},
                        children = {
                            {
                                Name = "Door_01",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/Door/Door_01.obj',
                                    material = materials[14],
                                }
                            },
                        }
                    },
                },
            }
        },
    },
    swingRight = {
        Name = "SwingRight",
        Transform = {
            position = {-0.5, 0, 0},
            children = {
                {
                    Name = "Door_01 (1)",
                    Transform = {
                        position = {-0.04400003, 0, 0},
                        rotation = {0, 270.0002, 0},
                        scale = {1, 1, 0.6418437},
                        children = {
                            {
                                Name = "Door_01",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/Door/Door_01.obj',
                                    material = materials[14],
                                }
                            },
                        }
                    },
                },
            }
        },
    }
}

grid[4][7].laser = {
    direction = {x = 0, y = -1},
    actor = {
        Name = "Laser",
        Transform = {
            position = {3, 0, 5.96},
            rotation = {0, 179.9997, 0},
            children = {
                {
                    Name = "MirrorModel (1)",
                    Transform = {
                        rotation = {0, 270.0002, 0},
                        children = {
                            {
                                Name = "MirrorModelLow:Group56562",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/laser/MirrorModel.obj',
                                    material = materials[13],
                                }
                            },
                        }
                    },
                },
            }
        },
    },
    beam = {
        Name = "Beam",
        Transform = {
            position = {0, 0.5, -0.48},
            scale = {1, 1, 6},
            children = {
                {
                    Name = "cube_flat",
                    Transform = {
                        position = {0, 0.03, 0.5},
                        scale = {0.43, 0.4380845, 0.5},
                        children = {
                            {
                                Name = "default",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/tile/cube_flat.obj',
                                    material = materials[12],
                                }
                            },
                        }
                    },
                },
            }
        },
    }
}

grid[4][1].player = {
    Name = "Player",
    Transform = {
        position = {3, 0, 0},
        children = {
            {
                Name = "Main_Character",
                Transform = {
                    children = {
                        {
                            Name = "Main_Character2",
                            Transform = {
                            },
                            RenderInfo = {
                                mesh = 'objects/Player/Main_Character.obj',
                                material = materials[1],
                            }
                        },
                    }
                },
            },
        }
    },
}

local extras = {
    {
        Name = "Main Camera",
        Transform = {
            position = {-5.69, 11.13, -1.49},
            rotation = {315, 225, -2.414836E-06},
        },
        Camera = {
            isOrthographic = true,
            orthographicHalfSize = 5.3,
            nearPlaneDistance = 0.3,
            farPlaneDistance = 1000
        }
    },
    {
        Name = "Directional Light",
        Transform = {
            position = {0, 3, 0},
            rotation = {28.86, 124.461, 0},
        },
        Light = {
            kind = 'directional',
            intensity = 6.94292,
            falloff = {1, 1, 0},
            color = {1, 1, 1, 1},
        }
    },
    {
        Name = "LevelExporter",
        Transform = {
        },
    },
    {
        Name = "Tiles",
        Transform = {
        },
    },
    {
        Name = "Obstacles",
        Transform = {
        },
    },
    {
        Name = "Buttons",
        Transform = {
        },
    },
    {
        Name = "Portals",
        Transform = {
        },
    },
    {
        Name = "Decorations",
        Transform = {
            children = {
                {
                    Name = "MenuFLoor_01 (40)",
                    Transform = {
                        position = {0.5, -0.5, 4},
                        rotation = {0, 90, 0},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[6],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (61)",
                    Transform = {
                        position = {0, -0.5, 5.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[6],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (63)",
                    Transform = {
                        position = {1, -1.49, 0},
                        children = {
                            {
                                Name = "TerrainBlockV3",
                                Transform = {
                                    position = {0, 0, -0.36},
                                    children = {
                                        {
                                            Name = "TerrainBlockV3LowPoly1",
                                            Transform = {
                                                position = {-0.3820662, 0, 0.3415085},
                                                rotation = {0, 89.99983, 0},
                                            },
                                            RenderInfo = {
                                                mesh = 'objects/tile/Blocks/FoundationBlock/TerrainBlockV3.obj',
                                                material = materials[2],
                                                isBatchingStatic = true
                                            }
                                        },
                                    }
                                },
                            },
                        }
                    },
                },
                {
                    Name = "vine_02",
                    Transform = {
                        position = {-2.43, 4.65, 12.679},
                        rotation = {347.8719, 269.9996, 8.514329E-05},
                        scale = {1.225201, 1.2891, 1},
                        children = {
                            {
                                Name = "vine_02",
                                Transform = {
                                    position = {-2.253, -1.767, -1.788},
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/Plants/vine_02.obj',
                                    material = materials[9],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "Flower_01",
                    Transform = {
                        position = {0.44, -0.42, 4.49},
                        children = {
                            {
                                Name = "Flower_01",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/Plants/Flower_01.obj',
                                    material = materials[9],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "grass_02 (1)",
                    Transform = {
                        position = {3.68, -0.408, 7.82},
                        rotation = {0, 264.2545, 0},
                        children = {
                            {
                                Name = "grass_02",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/Plants/grass_02.obj',
                                    material = materials[9],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (62)",
                    Transform = {
                        position = {-1, -0.5, 8.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[6],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (41)",
                    Transform = {
                        position = {1.5, -0.5, 5},
                        rotation = {0, 90, 0},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[6],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (63)",
                    Transform = {
                        position = {0, -0.5, 7.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[6],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (64)",
                    Transform = {
                        position = {1, -0.5, 2.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[6],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (65)",
                    Transform = {
                        position = {2, -0.5, -0.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[6],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (66)",
                    Transform = {
                        position = {1, -0.5, 0.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[6],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (42)",
                    Transform = {
                        position = {3.5, -0.5, -1},
                        rotation = {0, 90, 0},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[6],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (67)",
                    Transform = {
                        position = {4, -0.5, 0.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[6],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (68)",
                    Transform = {
                        position = {5, -0.5, 1.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[6],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (69)",
                    Transform = {
                        position = {5, -0.5, 3.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[6],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (70)",
                    Transform = {
                        position = {5, -0.5, 5.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[6],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (43)",
                    Transform = {
                        position = {5.5, -0.5, 7},
                        rotation = {0, 90, 0},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[6],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (44)",
                    Transform = {
                        position = {5.5, -0.5, 8},
                        rotation = {0, 90, 0},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[6],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (45)",
                    Transform = {
                        position = {4.5, -0.5, 9},
                        rotation = {0, 90, 0},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[6],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (71)",
                    Transform = {
                        position = {3, -0.5, 9.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[6],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (72)",
                    Transform = {
                        position = {3, -0.5, 11.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[6],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (73)",
                    Transform = {
                        position = {4, -0.5, 10.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[6],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (74)",
                    Transform = {
                        position = {3, -0.5, 13.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[6],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (46)",
                    Transform = {
                        position = {1.5, -0.5, 14},
                        rotation = {0, 90, 0},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[6],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (47)",
                    Transform = {
                        position = {-0.5, -0.5, 14},
                        rotation = {0, 90, 0},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[6],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (75)",
                    Transform = {
                        position = {-1, -0.5, 10.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[6],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (76)",
                    Transform = {
                        position = {-1, -0.5, 12.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[6],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (64)",
                    Transform = {
                        position = {2, -1.49, -1},
                        children = {
                            {
                                Name = "TerrainBlockV3",
                                Transform = {
                                    position = {0, 0, -0.36},
                                    children = {
                                        {
                                            Name = "TerrainBlockV3LowPoly1",
                                            Transform = {
                                                position = {-0.3820662, 0, 0.3415085},
                                                rotation = {0, 89.99983, 0},
                                            },
                                            RenderInfo = {
                                                mesh = 'objects/tile/Blocks/FoundationBlock/TerrainBlockV3.obj',
                                                material = materials[2],
                                                isBatchingStatic = true
                                            }
                                        },
                                    }
                                },
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (65)",
                    Transform = {
                        position = {2, -1.49, 0},
                        children = {
                            {
                                Name = "TerrainBlockV3",
                                Transform = {
                                    position = {0, 0, -0.36},
                                    children = {
                                        {
                                            Name = "TerrainBlockV3LowPoly1",
                                            Transform = {
                                                position = {-0.3820662, 0, 0.3415085},
                                                rotation = {0, 89.99983, 0},
                                            },
                                            RenderInfo = {
                                                mesh = 'objects/tile/Blocks/FoundationBlock/TerrainBlockV3.obj',
                                                material = materials[2],
                                                isBatchingStatic = true
                                            }
                                        },
                                    }
                                },
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (66)",
                    Transform = {
                        position = {1, -1.49, 1},
                        children = {
                            {
                                Name = "TerrainBlockV3",
                                Transform = {
                                    position = {0, 0, -0.36},
                                    children = {
                                        {
                                            Name = "TerrainBlockV3LowPoly1",
                                            Transform = {
                                                position = {-0.3820662, 0, 0.3415085},
                                                rotation = {0, 89.99983, 0},
                                            },
                                            RenderInfo = {
                                                mesh = 'objects/tile/Blocks/FoundationBlock/TerrainBlockV3.obj',
                                                material = materials[2],
                                                isBatchingStatic = true
                                            }
                                        },
                                    }
                                },
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (67)",
                    Transform = {
                        position = {1, -1.49, 2},
                        children = {
                            {
                                Name = "TerrainBlockV3",
                                Transform = {
                                    position = {0, 0, -0.36},
                                    children = {
                                        {
                                            Name = "TerrainBlockV3LowPoly1",
                                            Transform = {
                                                position = {-0.3820662, 0, 0.3415085},
                                                rotation = {0, 89.99983, 0},
                                            },
                                            RenderInfo = {
                                                mesh = 'objects/tile/Blocks/FoundationBlock/TerrainBlockV3.obj',
                                                material = materials[2],
                                                isBatchingStatic = true
                                            }
                                        },
                                    }
                                },
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (68)",
                    Transform = {
                        position = {1, -1.49, 3},
                        children = {
                            {
                                Name = "TerrainBlockV3",
                                Transform = {
                                    position = {0, 0, -0.36},
                                    children = {
                                        {
                                            Name = "TerrainBlockV3LowPoly1",
                                            Transform = {
                                                position = {-0.3820662, 0, 0.3415085},
                                                rotation = {0, 89.99983, 0},
                                            },
                                            RenderInfo = {
                                                mesh = 'objects/tile/Blocks/FoundationBlock/TerrainBlockV3.obj',
                                                material = materials[2],
                                                isBatchingStatic = true
                                            }
                                        },
                                    }
                                },
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (69)",
                    Transform = {
                        position = {1, -1.49, 4},
                        children = {
                            {
                                Name = "TerrainBlockV3",
                                Transform = {
                                    position = {0, 0, -0.36},
                                    children = {
                                        {
                                            Name = "TerrainBlockV3LowPoly1",
                                            Transform = {
                                                position = {-0.3820662, 0, 0.3415085},
                                                rotation = {0, 89.99983, 0},
                                            },
                                            RenderInfo = {
                                                mesh = 'objects/tile/Blocks/FoundationBlock/TerrainBlockV3.obj',
                                                material = materials[2],
                                                isBatchingStatic = true
                                            }
                                        },
                                    }
                                },
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (70)",
                    Transform = {
                        position = {0, -1.49, 4},
                        children = {
                            {
                                Name = "TerrainBlockV3",
                                Transform = {
                                    position = {0, 0, -0.36},
                                    children = {
                                        {
                                            Name = "TerrainBlockV3LowPoly1",
                                            Transform = {
                                                position = {-0.3820662, 0, 0.3415085},
                                                rotation = {0, 89.99983, 0},
                                            },
                                            RenderInfo = {
                                                mesh = 'objects/tile/Blocks/FoundationBlock/TerrainBlockV3.obj',
                                                material = materials[2],
                                                isBatchingStatic = true
                                            }
                                        },
                                    }
                                },
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (71)",
                    Transform = {
                        position = {0, -1.49, 5},
                        children = {
                            {
                                Name = "TerrainBlockV3",
                                Transform = {
                                    position = {0, 0, -0.36},
                                    children = {
                                        {
                                            Name = "TerrainBlockV3LowPoly1",
                                            Transform = {
                                                position = {-0.3820662, 0, 0.3415085},
                                                rotation = {0, 89.99983, 0},
                                            },
                                            RenderInfo = {
                                                mesh = 'objects/tile/Blocks/FoundationBlock/TerrainBlockV3.obj',
                                                material = materials[2],
                                                isBatchingStatic = true
                                            }
                                        },
                                    }
                                },
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (72)",
                    Transform = {
                        position = {0, -1.49, 6},
                        children = {
                            {
                                Name = "TerrainBlockV3",
                                Transform = {
                                    position = {0, 0, -0.36},
                                    children = {
                                        {
                                            Name = "TerrainBlockV3LowPoly1",
                                            Transform = {
                                                position = {-0.3820662, 0, 0.3415085},
                                                rotation = {0, 89.99983, 0},
                                            },
                                            RenderInfo = {
                                                mesh = 'objects/tile/Blocks/FoundationBlock/TerrainBlockV3.obj',
                                                material = materials[2],
                                                isBatchingStatic = true
                                            }
                                        },
                                    }
                                },
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (73)",
                    Transform = {
                        position = {0, -1.49, 7},
                        children = {
                            {
                                Name = "TerrainBlockV3",
                                Transform = {
                                    position = {0, 0, -0.36},
                                    children = {
                                        {
                                            Name = "TerrainBlockV3LowPoly1",
                                            Transform = {
                                                position = {-0.3820662, 0, 0.3415085},
                                                rotation = {0, 89.99983, 0},
                                            },
                                            RenderInfo = {
                                                mesh = 'objects/tile/Blocks/FoundationBlock/TerrainBlockV3.obj',
                                                material = materials[2],
                                                isBatchingStatic = true
                                            }
                                        },
                                    }
                                },
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (74)",
                    Transform = {
                        position = {0, -1.49, 8},
                        children = {
                            {
                                Name = "TerrainBlockV3",
                                Transform = {
                                    position = {0, 0, -0.36},
                                    children = {
                                        {
                                            Name = "TerrainBlockV3LowPoly1",
                                            Transform = {
                                                position = {-0.3820662, 0, 0.3415085},
                                                rotation = {0, 89.99983, 0},
                                            },
                                            RenderInfo = {
                                                mesh = 'objects/tile/Blocks/FoundationBlock/TerrainBlockV3.obj',
                                                material = materials[2],
                                                isBatchingStatic = true
                                            }
                                        },
                                    }
                                },
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (75)",
                    Transform = {
                        position = {-1, -1.49, 8},
                        children = {
                            {
                                Name = "TerrainBlockV3",
                                Transform = {
                                    position = {0, 0, -0.36},
                                    children = {
                                        {
                                            Name = "TerrainBlockV3LowPoly1",
                                            Transform = {
                                                position = {-0.3820662, 0, 0.3415085},
                                                rotation = {0, 89.99983, 0},
                                            },
                                            RenderInfo = {
                                                mesh = 'objects/tile/Blocks/FoundationBlock/TerrainBlockV3.obj',
                                                material = materials[2],
                                                isBatchingStatic = true
                                            }
                                        },
                                    }
                                },
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (76)",
                    Transform = {
                        position = {-1, -1.49, 9},
                        children = {
                            {
                                Name = "TerrainBlockV3",
                                Transform = {
                                    position = {0, 0, -0.36},
                                    children = {
                                        {
                                            Name = "TerrainBlockV3LowPoly1",
                                            Transform = {
                                                position = {-0.3820662, 0, 0.3415085},
                                                rotation = {0, 89.99983, 0},
                                            },
                                            RenderInfo = {
                                                mesh = 'objects/tile/Blocks/FoundationBlock/TerrainBlockV3.obj',
                                                material = materials[2],
                                                isBatchingStatic = true
                                            }
                                        },
                                    }
                                },
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (77)",
                    Transform = {
                        position = {-1, -1.49, 10},
                        children = {
                            {
                                Name = "TerrainBlockV3",
                                Transform = {
                                    position = {0, 0, -0.36},
                                    children = {
                                        {
                                            Name = "TerrainBlockV3LowPoly1",
                                            Transform = {
                                                position = {-0.3820662, 0, 0.3415085},
                                                rotation = {0, 89.99983, 0},
                                            },
                                            RenderInfo = {
                                                mesh = 'objects/tile/Blocks/FoundationBlock/TerrainBlockV3.obj',
                                                material = materials[2],
                                                isBatchingStatic = true
                                            }
                                        },
                                    }
                                },
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (78)",
                    Transform = {
                        position = {-1, -1.49, 11},
                        children = {
                            {
                                Name = "TerrainBlockV3",
                                Transform = {
                                    position = {0, 0, -0.36},
                                    children = {
                                        {
                                            Name = "TerrainBlockV3LowPoly1",
                                            Transform = {
                                                position = {-0.3820662, 0, 0.3415085},
                                                rotation = {0, 89.99983, 0},
                                            },
                                            RenderInfo = {
                                                mesh = 'objects/tile/Blocks/FoundationBlock/TerrainBlockV3.obj',
                                                material = materials[2],
                                                isBatchingStatic = true
                                            }
                                        },
                                    }
                                },
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (79)",
                    Transform = {
                        position = {-1, -1.49, 12},
                        children = {
                            {
                                Name = "TerrainBlockV3",
                                Transform = {
                                    position = {0, 0, -0.36},
                                    children = {
                                        {
                                            Name = "TerrainBlockV3LowPoly1",
                                            Transform = {
                                                position = {-0.3820662, 0, 0.3415085},
                                                rotation = {0, 89.99983, 0},
                                            },
                                            RenderInfo = {
                                                mesh = 'objects/tile/Blocks/FoundationBlock/TerrainBlockV3.obj',
                                                material = materials[2],
                                                isBatchingStatic = true
                                            }
                                        },
                                    }
                                },
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (80)",
                    Transform = {
                        position = {-1, -1.49, 13},
                        children = {
                            {
                                Name = "TerrainBlockV3",
                                Transform = {
                                    position = {0, 0, -0.36},
                                    children = {
                                        {
                                            Name = "TerrainBlockV3LowPoly1",
                                            Transform = {
                                                position = {-0.3820662, 0, 0.3415085},
                                                rotation = {0, 89.99983, 0},
                                            },
                                            RenderInfo = {
                                                mesh = 'objects/tile/Blocks/FoundationBlock/TerrainBlockV3.obj',
                                                material = materials[2],
                                                isBatchingStatic = true
                                            }
                                        },
                                    }
                                },
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (81)",
                    Transform = {
                        position = {-1, -1.49, 14},
                        children = {
                            {
                                Name = "TerrainBlockV3",
                                Transform = {
                                    position = {0, 0, -0.36},
                                    children = {
                                        {
                                            Name = "TerrainBlockV3LowPoly1",
                                            Transform = {
                                                position = {-0.3820662, 0, 0.3415085},
                                                rotation = {0, 89.99983, 0},
                                            },
                                            RenderInfo = {
                                                mesh = 'objects/tile/Blocks/FoundationBlock/TerrainBlockV3.obj',
                                                material = materials[2],
                                                isBatchingStatic = true
                                            }
                                        },
                                    }
                                },
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (82)",
                    Transform = {
                        position = {3, -1.49, -1},
                        children = {
                            {
                                Name = "TerrainBlockV3",
                                Transform = {
                                    position = {0, 0, -0.36},
                                    children = {
                                        {
                                            Name = "TerrainBlockV3LowPoly1",
                                            Transform = {
                                                position = {-0.3820662, 0, 0.3415085},
                                                rotation = {0, 89.99983, 0},
                                            },
                                            RenderInfo = {
                                                mesh = 'objects/tile/Blocks/FoundationBlock/TerrainBlockV3.obj',
                                                material = materials[2],
                                                isBatchingStatic = true
                                            }
                                        },
                                    }
                                },
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (83)",
                    Transform = {
                        position = {4, -1.49, -1},
                        children = {
                            {
                                Name = "TerrainBlockV3",
                                Transform = {
                                    position = {0, 0, -0.36},
                                    children = {
                                        {
                                            Name = "TerrainBlockV3LowPoly1",
                                            Transform = {
                                                position = {-0.3820662, 0, 0.3415085},
                                                rotation = {0, 89.99983, 0},
                                            },
                                            RenderInfo = {
                                                mesh = 'objects/tile/Blocks/FoundationBlock/TerrainBlockV3.obj',
                                                material = materials[2],
                                                isBatchingStatic = true
                                            }
                                        },
                                    }
                                },
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (84)",
                    Transform = {
                        position = {4, -1.49, 0},
                        children = {
                            {
                                Name = "TerrainBlockV3",
                                Transform = {
                                    position = {0, 0, -0.36},
                                    children = {
                                        {
                                            Name = "TerrainBlockV3LowPoly1",
                                            Transform = {
                                                position = {-0.3820662, 0, 0.3415085},
                                                rotation = {0, 89.99983, 0},
                                            },
                                            RenderInfo = {
                                                mesh = 'objects/tile/Blocks/FoundationBlock/TerrainBlockV3.obj',
                                                material = materials[2],
                                                isBatchingStatic = true
                                            }
                                        },
                                    }
                                },
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (85)",
                    Transform = {
                        position = {5, -1.49, 1},
                        children = {
                            {
                                Name = "TerrainBlockV3",
                                Transform = {
                                    position = {0, 0, -0.36},
                                    children = {
                                        {
                                            Name = "TerrainBlockV3LowPoly1",
                                            Transform = {
                                                position = {-0.3820662, 0, 0.3415085},
                                                rotation = {0, 89.99983, 0},
                                            },
                                            RenderInfo = {
                                                mesh = 'objects/tile/Blocks/FoundationBlock/TerrainBlockV3.obj',
                                                material = materials[2],
                                                isBatchingStatic = true
                                            }
                                        },
                                    }
                                },
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (86)",
                    Transform = {
                        position = {6, -1.49, 7},
                        children = {
                            {
                                Name = "TerrainBlockV3",
                                Transform = {
                                    position = {0, 0, -0.36},
                                    children = {
                                        {
                                            Name = "TerrainBlockV3LowPoly1",
                                            Transform = {
                                                position = {-0.3820662, 0, 0.3415085},
                                                rotation = {0, 89.99983, 0},
                                            },
                                            RenderInfo = {
                                                mesh = 'objects/tile/Blocks/FoundationBlock/TerrainBlockV3.obj',
                                                material = materials[2],
                                                isBatchingStatic = true
                                            }
                                        },
                                    }
                                },
                            },
                        }
                    },
                },
                {
                    Name = "grass_02 (2)",
                    Transform = {
                        position = {1.68, -0.408, -1.637},
                        rotation = {0, 264.2545, 0},
                        children = {
                            {
                                Name = "grass_02",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/Plants/grass_02.obj',
                                    material = materials[9],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
            }
        },
    },
    {
        Name = "Doors",
        Transform = {
            children = {
                {
                    Name = "Door",
                    Transform = {
                        position = {1, 0, 9},
                    },
                },
            }
        },
    },
    {
        Name = "Lasers",
        Transform = {
        },
    },
}

return Level:new {
    map = map,
    extras = extras,
    nextLevelPath = 'assets/scripts/scenes/level8.lua',
    ambientLighting = {color = {1.010478, 1.854524, 2.270603, 1}},
    maxNumUndos = {
        threeStars = 2,
        twoStars = 4,
        oneStar = 7
    }
}
