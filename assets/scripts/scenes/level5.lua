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
        renderMode = 'cutout',
        albedo = 'objects/decorations/Plants/Material/Plant_All (3).tga',
        normal = 'objects/decorations/Plants/Material/Plant_All_NormalMap.png',
        metallicMultiplier = 0,
        smoothnessMultiplier = 0,
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
        albedo = 'objects/tile/Blocks/TerrainBlockVar4/Texture/TerrainBlockV4_AlbedoTransparency.png',
        metallicSmoothness = 'objects/tile/Blocks/TerrainBlockVar4/Texture/TerrainBlockV4_MetallicSmoothness.png',
        ao = 'objects/tile/Blocks/TerrainBlockVar4/Texture/TerrainBlockV4_AO.png',
        normal = 'objects/tile/Blocks/TerrainBlockVar4/Texture/TerraainBlockV4.png',
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
        albedo = 'objects/decorations/Broeknfloor4/Texture/Flooring_01_broken_04_Mossy_AlbedoTransparency.png',
        metallicSmoothness = 'objects/decorations/Broeknfloor4/Texture/Flooring_01_broken_04_Mossy_MetallicSmoothness.png',
        ao = 'objects/decorations/Broeknfloor4/Texture/Flooring_01_broken_04_Mossy_AO.png',
        normal = 'objects/decorations/Broeknfloor4/Texture/Flooring_01_broken_04_Mossy_Normal.png',
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
        albedo = 'objects/portal/PortalIn_AlbedoTransparency (7).png',
        metallicSmoothness = 'objects/portal/PortalIn_MetallicSmoothness.png',
        ao = 'objects/portal/PortalIn_AO.png',
        normal = 'objects/portal/PortalIn_Normal.png',
        smoothnessMultiplier = 0,
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
    Game.makeMaterial {
        shader = 'pbr',
        albedo = 'objects/tile/ButtonTile/Texture/ButtonTile_AlbedoTransparency.png',
        metallicSmoothness = 'objects/tile/ButtonTile/Texture/ButtonTile_MetallicSmoothness.png',
        ao = 'objects/tile/ButtonTile/Texture/ButtonTile_AO.png',
        normal = 'objects/tile/ButtonTile/Texture/ButtonTile_Normal.png',
        smoothnessMultiplier = 1,
        aoMultiplier = 1,
    },
}

local map = Map:new {
    gridSize = {x = 4, y = 16},
    grid = {
        {{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{}},
        {{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{}},
        {{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{}},
        {{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{}},
    }
}

local grid = map.grid

grid[1][3].tile = {
    Name = "Tile 1",
    Transform = {
        position = {0, 0, 2},
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
                                material = materials[9],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[1][4].tile = {
    Name = "Tile 3",
    Transform = {
        position = {0, 0, 3},
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

grid[1][8].tile = {
    Name = "Tile 2",
    Transform = {
        position = {0, 0, 7},
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

grid[1][9].tile = {
    Name = "Tile 3",
    Transform = {
        position = {0, 0, 8},
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
                        },
                    }
                },
            },
        }
    },
}

grid[1][10].tile = {
    Name = "Tile 1",
    Transform = {
        position = {0, 0, 9},
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
                                material = materials[9],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[2][1].tile = {
    Name = "Tile 3",
    Transform = {
        position = {1, 0, 0},
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

grid[2][2].tile = {
    Name = "Tile 3",
    Transform = {
        position = {1, 0, 1},
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

grid[2][3].tile = {
    Name = "Tile 2",
    Transform = {
        position = {1, 0, 2},
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

grid[2][4].tile = {
    Name = "Tile 2",
    Transform = {
        position = {1, 0, 3},
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

grid[2][5].tile = {
    Name = "Tile",
    Transform = {
        position = {0.947, -0.02699998, 3.969357},
        rotation = {0, 179.9997, 0},
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

grid[2][7].tile = {
    Name = "Tile",
    Transform = {
        position = {1, 0, 6},
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
    Name = "Tile 3",
    Transform = {
        position = {1, 0, 7},
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

grid[2][9].tile = {
    Name = "Tile",
    Transform = {
        position = {1, 0, 8},
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
                                material = materials[11],
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

grid[2][11].tile = {
    Name = "Tile 3",
    Transform = {
        position = {1, 0, 10},
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
                                material = materials[9],
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
    Name = "Tile 1",
    Transform = {
        position = {1, 0, 12},
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
                                material = materials[9],
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
    Name = "Tile 1 (1)",
    Transform = {
        position = {1, 0, 13},
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
                                material = materials[9],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[2][15].tile = {
    Name = "Tile 1 (4)",
    Transform = {
        position = {1, 0, 14},
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
                                material = materials[9],
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
    Name = "Tile",
    Transform = {
        position = {2, 0, 2},
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
                                material = materials[11],
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
    Name = "Tile 3",
    Transform = {
        position = {2, 0, 3},
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

grid[3][8].tile = {
    Name = "Tile 2",
    Transform = {
        position = {2, 0, 7},
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

grid[3][9].tile = {
    Name = "Tile 1",
    Transform = {
        position = {2, 0, 8},
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
                                material = materials[9],
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
    Name = "Tile 1",
    Transform = {
        position = {2, 0, 9},
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
                                material = materials[9],
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
    Name = "Tile",
    Transform = {
        position = {2, 0, 10},
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
                                material = materials[11],
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
    Name = "Tile 2",
    Transform = {
        position = {2, 0, 12},
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

grid[3][14].tile = {
    Name = "Tile 2 (1)",
    Transform = {
        position = {2, 0, 13},
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

grid[3][15].tile = {
    Name = "Tile 2 (2)",
    Transform = {
        position = {2, 0, 14},
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

grid[4][8].tile = {
    Name = "Tile 2",
    Transform = {
        position = {3, 0, 7},
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

grid[4][9].tile = {
    Name = "Tile 1",
    Transform = {
        position = {3, 0, 8},
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
                                material = materials[9],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[4][10].tile = {
    Name = "Tile 3",
    Transform = {
        position = {3, 0, 9},
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

grid[4][11].tile = {
    Name = "Tile 3",
    Transform = {
        position = {3, 0, 10},
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

grid[4][12].tile = {
    Name = "Tile 1",
    Transform = {
        position = {3, 0, 11},
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
                                material = materials[9],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[4][13].tile = {
    Name = "Tile 1",
    Transform = {
        position = {3, 0, 12},
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
                                material = materials[9],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[4][14].tile = {
    Name = "Tile 1 (2)",
    Transform = {
        position = {3, 0, 13},
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
                                material = materials[9],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[4][15].tile = {
    Name = "Tile 1 (3)",
    Transform = {
        position = {3, 0, 14},
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
                                material = materials[9],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[2][11].obstacle = {
    Name = "PillarRight",
    Transform = {
        position = {1, 0, 10},
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
                                material = materials[14],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[2][13].obstacle = {
    Name = "Obstacle",
    Transform = {
        position = {1.000006, 0, 11.99999},
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

grid[3][14].obstacle = {
    Name = "Obstacle (1)",
    Transform = {
        position = {2, 0, 13},
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

grid[4][11].obstacle = {
    Name = "PillarLeft",
    Transform = {
        position = {3, 0, 10},
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
                                material = materials[14],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[4][13].obstacle = {
    Name = "Obstacle (2)",
    Transform = {
        position = {3.000006, 0, 12.00001},
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

grid[3][13].goal = {
    startActive = true,
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
            position = {2, 0, 12},
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

grid[1][9].button = {
    targetPositions = {
        {x = 3, y = 11},
    },
    actor = {
        Name = "Button",
        Transform = {
            position = {0, 0.198, 8},
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
                                    material = materials[15],
                                }
                            },
                        }
                    },
                },
            }
        },
    }
}

grid[2][5].portal = {
    teleportPosition = {x = 2, y = 7},
    actor = {
        Name = "Portal",
        Transform = {
            position = {0.98, 0.473, 3.969357},
            rotation = {0, 179.9997, 0},
            children = {
                {
                    Name = "PortalTileIn",
                    Transform = {
                        position = {-0.049, -1.413, 0},
                        children = {
                            {
                                Name = "PortalTileOut:Group41898",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/portal/PortalTileIn.obj',
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

grid[2][7].portal = {
    teleportPosition = {x = 2, y = 5},
    actor = {
        Name = "Portal (1)",
        Transform = {
            position = {0.973, 0.473, 6.025},
            children = {
                {
                    Name = "PortalTileIn",
                    Transform = {
                        position = {-0.049, -1.413, 0},
                        children = {
                            {
                                Name = "PortalTileOut:Group41898",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/portal/PortalTileIn.obj',
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

grid[3][11].door = {
    actor = {
        Name = "Door",
        Transform = {
            position = {2, 0, 10},
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
                                    material = materials[13],
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
                                    material = materials[13],
                                }
                            },
                        }
                    },
                },
            }
        },
    }
}

grid[2][1].player = {
    Name = "Player",
    Transform = {
        position = {1, 0, 0},
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
            position = {-6.5, 10.6, -0.48},
            rotation = {315, 225, -2.414836E-06},
        },
        Camera = {
            isOrthographic = true,
            orthographicHalfSize = 6.4,
            nearPlaneDistance = 0.3,
            farPlaneDistance = 1000
        }
    },
    {
        Name = "Flooring_01_broken_04",
        Transform = {
            position = {-0.549, -0.5, 6.027},
            rotation = {0, 270, 0},
            children = {
                {
                    Name = "Flooring_01_broken_09",
                    Transform = {
                    },
                    RenderInfo = {
                        mesh = 'objects/decorations/Broeknfloor4/Flooring_01_broken_04.OBJ',
                        material = materials[10],
                        isBatchingStatic = true
                    }
                },
            }
        },
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
                    Name = "MenuFLoor_01 (29)",
                    Transform = {
                        position = {-0.5, -0.5, 4},
                        rotation = {0, 90, 0},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[7],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (47)",
                    Transform = {
                        position = {-1, -0.5, 2.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[7],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (46)",
                    Transform = {
                        position = {-1, -1.49, 7},
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
                    Name = "MenuFLoor_01 (30)",
                    Transform = {
                        position = {-0.5, -0.5, 1},
                        rotation = {0, 90, 0},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[7],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (48)",
                    Transform = {
                        position = {0, -0.5, -1.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[7],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (31)",
                    Transform = {
                        position = {-0.5, -0.5, 0},
                        rotation = {0, 90, 0},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[7],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (32)",
                    Transform = {
                        position = {1.5, -0.5, -1},
                        rotation = {0, 90, 0},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[7],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (49)",
                    Transform = {
                        position = {2, -0.5, 0.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[7],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (50)",
                    Transform = {
                        position = {3, -0.5, 1.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[7],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (33)",
                    Transform = {
                        position = {2.5, -0.5, 3},
                        rotation = {0, 90, 0},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[7],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (34)",
                    Transform = {
                        position = {2.5, -0.5, 4},
                        rotation = {0, 90, 0},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[7],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (47)",
                    Transform = {
                        position = {-1, -1.49, 4},
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
                    Name = "FoundationBlock (48)",
                    Transform = {
                        position = {-1, -1.49, 3},
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
                    Name = "FoundationBlock (49)",
                    Transform = {
                        position = {-1, -1.49, 2},
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
                    Name = "FoundationBlock (50)",
                    Transform = {
                        position = {-1, -1.49, 1},
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
                    Name = "FoundationBlock (51)",
                    Transform = {
                        position = {-1, -1.49, 0},
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
                    Name = "FoundationBlock (52)",
                    Transform = {
                        position = {0, -1.49, -1},
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
                    Name = "FoundationBlock (53)",
                    Transform = {
                        position = {0, -1.49, -2},
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
                    Name = "FoundationBlock (54)",
                    Transform = {
                        position = {0, -1.49, 0},
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
                    Name = "FoundationBlock (55)",
                    Transform = {
                        position = {1, -1.49, -1},
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
                    Name = "FoundationBlock (56)",
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
                    Name = "FoundationBlock (57)",
                    Transform = {
                        position = {3, -1.49, 1},
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
                    Name = "MenuFLoor_01 (36)",
                    Transform = {
                        position = {2.5, -0.5, 6},
                        rotation = {0, 90, 0},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[7],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (58)",
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
                    Name = "FoundationBlock (59)",
                    Transform = {
                        position = {-1, -1.49, 6},
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
                    Name = "FoundationBlock (60)",
                    Transform = {
                        position = {2, -1.49, 6},
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
                    Name = "FoundationBlock (61)",
                    Transform = {
                        position = {3, -1.49, 6},
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
                    Name = "FoundationBlock (62)",
                    Transform = {
                        position = {1, -1.49, 6},
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
                    Name = "MenuFLoor_01 (51)",
                    Transform = {
                        position = {-1, -0.5, 7.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[7],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (63)",
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
                    Name = "MenuFLoor_01 (52)",
                    Transform = {
                        position = {4, -0.5, 6.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[7],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (64)",
                    Transform = {
                        position = {4, -1.49, 6},
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
                    Name = "MenuFLoor_01 (37)",
                    Transform = {
                        position = {4.5, -0.5, 8},
                        rotation = {0, 90, 0},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[7],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (38)",
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
                                    material = materials[7],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (65)",
                    Transform = {
                        position = {5, -1.49, 8},
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
                    Name = "MenuFLoor_01 (53)",
                    Transform = {
                        position = {0, -0.5, 10.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[7],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (54)",
                    Transform = {
                        position = {-1, -0.5, 9.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[7],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (66)",
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
                    Name = "FoundationBlock (67)",
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
                    Name = "FoundationBlock (68)",
                    Transform = {
                        position = {0, -1.49, 11},
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
                    Name = "MenuFLoor_01 (55)",
                    Transform = {
                        position = {0, -0.5, 12.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[7],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (56)",
                    Transform = {
                        position = {0, -0.5, 14.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[7],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (39)",
                    Transform = {
                        position = {1.5, -0.5, 15},
                        rotation = {0, 90, 0},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[7],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (40)",
                    Transform = {
                        position = {3.5, -0.5, 15},
                        rotation = {0, 90, 0},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[7],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (57)",
                    Transform = {
                        position = {4, -0.5, 9.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                    position = {0, 0, 1},
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[7],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (58)",
                    Transform = {
                        position = {4, -0.5, 13.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[7],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (59)",
                    Transform = {
                        position = {4, -0.5, 11.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                    position = {0, 0, 1},
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/MenuFloor/MenuFLoor_01.obj',
                                    material = materials[7],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (69)",
                    Transform = {
                        position = {0, -1.49, 12},
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
                        position = {0, -1.49, 13},
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
                        position = {0, -1.49, 14},
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
                        position = {0, -1.49, 15},
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
                        position = {-1, -1.49, 6},
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
                        position = {3, -1.49, 6},
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
                    Name = "FoundationBlock (76)",
                    Transform = {
                        position = {2, -1.49, 6},
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
                        position = {1, -1.49, 6},
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
                        position = {4, -1.49, 6},
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
                    Name = "vine_02 (3)",
                    Transform = {
                        position = {-0.006, 2.206, 10.631},
                        rotation = {343.5566, 270.0001, 358.2078},
                        scale = {1.4196, 1.2025, 0.9999999},
                        children = {
                            {
                                Name = "vine_02",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/Plants/vine_02.obj',
                                    material = materials[6],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "vine_02",
                    Transform = {
                        position = {0.25032, 2.216, 2.663},
                        rotation = {0, 179.9997, 0},
                        scale = {1.2252, 1.2891, 1},
                        children = {
                            {
                                Name = "vine_02",
                                Transform = {
                                    position = {-2.253, -1.767, -1.788},
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/Plants/vine_02.obj',
                                    material = materials[6],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "grass_02",
                    Transform = {
                        position = {2.767, -0.408, 8.486},
                        children = {
                            {
                                Name = "grass_02",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/Plants/grass_02.obj',
                                    material = materials[6],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "Flower_01",
                    Transform = {
                        position = {-0.196, -0.42, -0.481},
                        children = {
                            {
                                Name = "Flower_01",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/Plants/Flower_01.obj',
                                    material = materials[6],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "vine_02 (1)",
                    Transform = {
                        position = {-4.373587, 1.793, 8.589445},
                        rotation = {0, 269.3684, 0},
                        scale = {1.2252, 1.2891, 1},
                        children = {
                            {
                                Name = "vine_02",
                                Transform = {
                                    position = {-2.253, -1.767, -1.788},
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/Plants/vine_02.obj',
                                    material = materials[6],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "grass_02 (1)",
                    Transform = {
                        position = {-1.596, -0.408, 3.1},
                        rotation = {0, 75.29996, 0},
                        children = {
                            {
                                Name = "grass_02",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/Plants/grass_02.obj',
                                    material = materials[6],
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
                        position = {2, 0, 10},
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
    {
        Name = "Flooring_01_broken_04 (1)",
        Transform = {
            position = {-0.3677717, -0.5, 5.482199},
            rotation = {0, 22.71556, 0},
            children = {
                {
                    Name = "Flooring_01_broken_09",
                    Transform = {
                    },
                    RenderInfo = {
                        mesh = 'objects/decorations/Broeknfloor4/Flooring_01_broken_04.OBJ',
                        material = materials[10],
                        isBatchingStatic = true
                    }
                },
            }
        },
    },
}

return Level:new {
    map = map,
    extras = extras,
    nextLevelPath = 'assets/scripts/scenes/level6.lua',
    ambientLighting = {color = {1.010478, 1.854524, 2.270603, 1}},
    maxNumUndos = {
        threeStars = 0,
        twoStars = 1,
        oneStar = 2
    }
}
