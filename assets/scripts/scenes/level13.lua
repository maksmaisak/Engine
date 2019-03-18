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
        albedo = 'objects/tile/ButtonTile/Texture/ButtonTile_AlbedoTransparency.png',
        metallicSmoothness = 'objects/tile/ButtonTile/Texture/ButtonTile_MetallicSmoothness.png',
        ao = 'objects/tile/ButtonTile/Texture/ButtonTile_AO.png',
        normal = 'objects/tile/ButtonTile/Texture/ButtonTile_Normal.png',
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
        albedo = 'objects/decorations/MenuFloor/Texture/UvMappedFlooring_initialShadingGroup_AlbedoTransparency.png',
        metallicSmoothness = 'objects/decorations/MenuFloor/Texture/UvMappedFlooring_initialShadingGroup_MetallicSmoothness.png',
        normal = 'objects/decorations/MenuFloor/Texture/UvMappedFlooring_initialShadingGroup_Normal.png',
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
        albedo = 'objects/tile/Blocks/TerrainBlockVar2/Texture/TerrainBlockV2_AlbedoTransparency.png',
        metallicSmoothness = 'objects/tile/Blocks/TerrainBlockVar2/Texture/TerrainBlockV2_MetallicSmoothness.png',
        ao = 'objects/tile/Blocks/TerrainBlockVar2/Texture/TerrainBlockV2_AO.png',
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
        albedo = 'objects/tile/Blocks/TerrainBlockVar3/Texture/TerrainBlockV3_AlbedoTransparency.png',
        metallicSmoothness = 'objects/tile/Blocks/TerrainBlockVar3/Texture/TerrainBlockV3_MetallicSmoothness.png',
        ao = 'objects/tile/Blocks/TerrainBlockVar3/Texture/TerrainBlockV3_AO.png',
        normal = 'objects/tile/Blocks/TerrainBlockVar3/Texture/TerraainBlockV3.png',
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
    gridSize = {x = 13, y = 13},
    grid = {
        {{},{},{},{},{},{},{},{},{},{},{},{},{}},
        {{},{},{},{},{},{},{},{},{},{},{},{},{}},
        {{},{},{},{},{},{},{},{},{},{},{},{},{}},
        {{},{},{},{},{},{},{},{},{},{},{},{},{}},
        {{},{},{},{},{},{},{},{},{},{},{},{},{}},
        {{},{},{},{},{},{},{},{},{},{},{},{},{}},
        {{},{},{},{},{},{},{},{},{},{},{},{},{}},
        {{},{},{},{},{},{},{},{},{},{},{},{},{}},
        {{},{},{},{},{},{},{},{},{},{},{},{},{}},
        {{},{},{},{},{},{},{},{},{},{},{},{},{}},
        {{},{},{},{},{},{},{},{},{},{},{},{},{}},
        {{},{},{},{},{},{},{},{},{},{},{},{},{}},
        {{},{},{},{},{},{},{},{},{},{},{},{},{}}
    }
}

local grid = map.grid

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
                                material = materials[12],
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
                                material = materials[12],
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
    Name = "Tile 3",
    Transform = {
        position = {1, 0, 4},
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

grid[2][6].tile = {
    Name = "Tile",
    Transform = {
        position = {1, 0, 5},
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
                                material = materials[6],
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
    Name = "Tile 2",
    Transform = {
        position = {1, 0, 6},
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
                                material = materials[12],
                                isBatchingStatic = true
                            }
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
                                material = materials[6],
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
    Name = "Tile 2",
    Transform = {
        position = {1, 0, 8},
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
                                material = materials[12],
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
    Name = "Tile 2",
    Transform = {
        position = {1, 0, 9},
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
                                material = materials[12],
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
                                material = materials[12],
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
    Name = "Tile 3",
    Transform = {
        position = {1, 0, 11},
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

grid[3][3].tile = {
    Name = "Tile 1",
    Transform = {
        position = {2, 0, 2},
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
    Name = "Tile",
    Transform = {
        position = {2, 0, 3},
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
                                material = materials[6],
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
    Name = "Tile 1",
    Transform = {
        position = {2, 0, 4},
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
                                material = materials[6],
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
    Name = "Tile 1",
    Transform = {
        position = {2, 0, 6},
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

grid[3][8].tile = {
    Name = "Tile",
    Transform = {
        position = {2, 0, 7},
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
                                material = materials[6],
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

grid[3][10].tile = {
    Name = "Tile 2",
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
                                material = materials[12],
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
    Name = "Tile 2",
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
                                material = materials[12],
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
    Name = "Tile 3",
    Transform = {
        position = {2, 0, 11},
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

grid[3][13].tile = {
    Name = "Tile",
    Transform = {
        position = {2, 0, 12},
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
                                material = materials[6],
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
    Name = "Tile 3",
    Transform = {
        position = {3, 0, 2},
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

grid[4][4].tile = {
    Name = "Tile 3",
    Transform = {
        position = {3, 0, 3},
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

grid[4][9].tile = {
    Name = "Tile 3",
    Transform = {
        position = {3, 0, 8},
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

grid[4][11].tile = {
    Name = "Tile 1",
    Transform = {
        position = {3, 0, 10},
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

grid[4][12].tile = {
    Name = "Tile 3",
    Transform = {
        position = {3, 0, 11},
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
    Name = "Tile 2",
    Transform = {
        position = {3, 0, 12},
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
                                material = materials[12],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[5][1].tile = {
    Name = "Tile 2",
    Transform = {
        position = {4, 0, 0},
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
                                material = materials[12],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[5][2].tile = {
    Name = "Tile",
    Transform = {
        position = {4, 0, 1},
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
                                material = materials[6],
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
    Name = "Tile 2",
    Transform = {
        position = {4, 0, 2},
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
                                material = materials[12],
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
    Name = "Tile",
    Transform = {
        position = {4, 0, 3},
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
                                material = materials[12],
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
    Name = "Tile 1",
    Transform = {
        position = {4, 0, 5},
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

grid[5][7].tile = {
    Name = "Tile",
    Transform = {
        position = {3.997, -2.980232E-08, 5.99319},
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

grid[5][8].tile = {
    Name = "Tile 3",
    Transform = {
        position = {4, 0, 7},
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

grid[6][3].tile = {
    Name = "Tile 3",
    Transform = {
        position = {5, 0, 2},
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

grid[6][4].tile = {
    Name = "Tile",
    Transform = {
        position = {5, 0, 3},
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
                                material = materials[6],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[6][5].tile = {
    Name = "Tile (1)",
    Transform = {
        position = {5, 0, 4},
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
                                material = materials[6],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[6][6].tile = {
    Name = "Tile 1 (1)",
    Transform = {
        position = {5, 0, 5},
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

grid[6][7].tile = {
    Name = "Tile (2)",
    Transform = {
        position = {5, 0, 6},
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
                                material = materials[6],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[6][8].tile = {
    Name = "Tile 2",
    Transform = {
        position = {5, 0, 7},
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
                                material = materials[12],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[7][3].tile = {
    Name = "Tile 3",
    Transform = {
        position = {6, 0, 2},
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

grid[7][4].tile = {
    Name = "Tile 1",
    Transform = {
        position = {6, 0, 3},
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

grid[7][6].tile = {
    Name = "Tile 1 (2)",
    Transform = {
        position = {6, 0, 5},
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

grid[7][8].tile = {
    Name = "Tile",
    Transform = {
        position = {6, 0, 7},
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
                                material = materials[6],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[8][3].tile = {
    Name = "Tile 1",
    Transform = {
        position = {7, 0, 2},
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

grid[8][4].tile = {
    Name = "Tile 2",
    Transform = {
        position = {7, 0, 3},
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
                                material = materials[12],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[8][6].tile = {
    Name = "Tile 1 (12)",
    Transform = {
        position = {7, 0, 5},
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

grid[8][8].tile = {
    Name = "Tile",
    Transform = {
        position = {7, 0, 7},
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
                                material = materials[6],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[9][3].tile = {
    Name = "Tile",
    Transform = {
        position = {8.01, -2.980232E-08, 2.015955},
        rotation = {0, 270.0002, 0},
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

grid[9][4].tile = {
    Name = "Tile 2",
    Transform = {
        position = {8, 0, 3},
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
                                material = materials[12],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[9][6].tile = {
    Name = "Tile",
    Transform = {
        position = {8, 0, 5},
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

grid[9][8].tile = {
    Name = "Tile",
    Transform = {
        position = {8, 0, 7},
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
                                material = materials[6],
                                isBatchingStatic = true
                            }
                        },
                    }
                },
            },
        }
    },
}

grid[10][3].tile = {
    Name = "Tile 1 (3)",
    Transform = {
        position = {9, 0, 2},
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

grid[10][4].tile = {
    Name = "Tile 1 (4)",
    Transform = {
        position = {9, 0, 3},
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

grid[10][8].tile = {
    Name = "Tile 1 (8)",
    Transform = {
        position = {9, 0, 7},
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

grid[11][3].tile = {
    Name = "Tile 1 (5)",
    Transform = {
        position = {10, 0, 2},
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

grid[11][4].tile = {
    Name = "Tile 1 (6)",
    Transform = {
        position = {10, 0, 3},
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

grid[11][5].tile = {
    Name = "Tile 1 (7)",
    Transform = {
        position = {10, 0, 4},
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

grid[11][6].tile = {
    Name = "Tile 1 (9)",
    Transform = {
        position = {10, 0, 5},
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

grid[11][7].tile = {
    Name = "Tile 1 (10)",
    Transform = {
        position = {10, 0, 6},
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

grid[11][8].tile = {
    Name = "Tile 1 (11)",
    Transform = {
        position = {10, 0, 7},
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

grid[2][9].obstacle = {
    Name = "PillarRight",
    Transform = {
        position = {0.9989563, 0, 7.995044},
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

grid[2][11].obstacle = {
    Name = "Obstacle",
    Transform = {
        position = {1.000006, 0, 9.999994},
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

grid[3][12].obstacle = {
    Name = "Obstacle (1)",
    Transform = {
        position = {2, 0, 11},
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

grid[4][9].obstacle = {
    Name = "PillarLeft",
    Transform = {
        position = {2.998956, 0, 7.995044},
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

grid[4][11].obstacle = {
    Name = "Obstacle (2)",
    Transform = {
        position = {3.000006, 0, 10.00001},
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

grid[6][5].obstacle = {
    Name = "PillarLeft",
    Transform = {
        position = {4.971047, 0, 4.004956},
        rotation = {0, 89.99983, 0},
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

grid[6][7].obstacle = {
    Name = "PillarRight",
    Transform = {
        position = {4.971041, 0, 6.004956},
        rotation = {0, 89.99983, 0},
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

grid[3][11].goal = {
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
            position = {2, 0, 10},
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

grid[5][4].button = {
    targetPositions = {
        {x = 6, y = 6},
        {x = 3, y = 11},
    },
    actor = {
        Name = "Button (1)",
        Transform = {
            position = {4, 0.2, 3},
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
                                    material = materials[5],
                                }
                            },
                        }
                    },
                },
            }
        },
    }
}

grid[9][6].button = {
    targetPositions = {
        {x = 3, y = 9},
    },
    actor = {
        Name = "Button (2)",
        Transform = {
            position = {8, 0.2, 5},
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
                                    material = materials[5],
                                }
                            },
                        }
                    },
                },
            }
        },
    }
}

grid[5][7].portal = {
    teleportPosition = {x = 9, y = 3},
    actor = {
        Name = "Portal (1)",
        Transform = {
            position = {4.03, 0.5, 5.99319},
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

grid[9][3].portal = {
    teleportPosition = {x = 5, y = 7},
    actor = {
        Name = "Portal",
        Transform = {
            position = {8.01, 0.5, 1.982955},
            rotation = {0, 270.0002, 0},
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

grid[3][9].door = {
    actor = {
        Name = "Door",
        Transform = {
            position = {1.998956, 0, 7.995044},
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

grid[6][6].door = {
    actor = {
        Name = "Door",
        Transform = {
            position = {4.971044, 0, 5.004956},
            rotation = {0, 89.99983, 0},
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

grid[5][1].player = {
    Name = "Player",
    Transform = {
        position = {4, 0, 0},
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
            position = {-0.01, 5.01, 1.66},
            rotation = {315, 225, -2.414836E-06},
        },
        Camera = {
            isOrthographic = true,
            orthographicHalfSize = 5.29,
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
                    Name = "vine_02",
                    Transform = {
                        position = {-4.43, 4.65, 13.68},
                        rotation = {347.8719, 269.9996, 8.514329E-05},
                        scale = {1.225201, 1.2891, 1},
                        children = {
                            {
                                Name = "vine_02",
                                Transform = {
                                    position = {-3.89, -1.28, -4.72},
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/Plants/vine_02.obj',
                                    material = materials[8],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "vine_03",
                    Transform = {
                        position = {2.44, -0.184, 6.28},
                        rotation = {0, 270.0002, 0},
                        children = {
                            {
                                Name = "vine_03",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/Plants/vine_03.obj',
                                    material = materials[8],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (96)",
                    Transform = {
                        position = {7, -0.5, 5.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                    position = {2, 0, 0},
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
                    Name = "FoundationBlock (113)",
                    Transform = {
                        position = {4.041, -1.49, 4},
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
                    Name = "Flower_01",
                    Transform = {
                        position = {-0.56, -0.42, 7.49},
                        children = {
                            {
                                Name = "Flower_01",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/Plants/Flower_01.obj',
                                    material = materials[8],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "grass_02 (6)",
                    Transform = {
                        position = {-2.14, -0.408, 10.69},
                        rotation = {0, 6.011236, 0},
                        children = {
                            {
                                Name = "grass_02",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/Plants/grass_02.obj',
                                    material = materials[8],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "MenuFLoor_01 (65)",
                    Transform = {
                        position = {6.5, -0.5, 4},
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
                    Name = "grass_01",
                    Transform = {
                        position = {6.33, -0.44, 8.47},
                        children = {
                            {
                                Name = "grass_01",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/Plants/grass_01.obj',
                                    material = materials[8],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "vine_01 (1)",
                    Transform = {
                        position = {8.7, -0.49, -0.54},
                        rotation = {0, 179.9996, 0},
                        scale = {0.86536, 0.86536, 0.86536},
                        children = {
                            {
                                Name = "vine_01",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/Plants/vine_01.obj',
                                    material = materials[8],
                                    isBatchingStatic = true
                                }
                            },
                        }
                    },
                },
                {
                    Name = "FoundationBlock (114)",
                    Transform = {
                        position = {5, -1.49, -1},
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
                    Name = "FoundationBlock (115)",
                    Transform = {
                        position = {4.041, -1.49, 5},
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
                    Name = "FoundationBlock (116)",
                    Transform = {
                        position = {4.075, -1.49, 6},
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
                    Name = "FoundationBlock (117)",
                    Transform = {
                        position = {4.075, -1.49, 7},
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
                    Name = "FoundationBlock (118)",
                    Transform = {
                        position = {4.075, -1.49, 8},
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
                    Name = "FoundationBlock (119)",
                    Transform = {
                        position = {3.07, -1.49, 8.011},
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
                    Name = "MenuFLoor_01 (66)",
                    Transform = {
                        position = {8.5, -0.5, 4},
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
                    Name = "MenuFLoor_01 (97)",
                    Transform = {
                        position = {3, -0.5, 0.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                    position = {2, 0, 0},
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
                    Name = "MenuFLoor_01 (67)",
                    Transform = {
                        position = {7.5, -0.5, 6},
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
                    Name = "MenuFLoor_01 (68)",
                    Transform = {
                        position = {5.5, -0.5, 6},
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
                    Name = "MenuFLoor_01 (69)",
                    Transform = {
                        position = {6.5, -0.5, 1},
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
                    Name = "MenuFLoor_01 (70)",
                    Transform = {
                        position = {4.5, -0.5, -1},
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
                    Name = "MenuFLoor_01 (98)",
                    Transform = {
                        position = {1, -0.5, -0.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                    position = {2, 0, 0},
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
                    Name = "MenuFLoor_01 (71)",
                    Transform = {
                        position = {2.5, -0.5, 1},
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
                    Name = "MenuFLoor_01 (72)",
                    Transform = {
                        position = {0.5, -0.5, 1},
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
                    Name = "MenuFLoor_01 (99)",
                    Transform = {
                        position = {-2, -0.5, 2.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                    position = {2, 0, 0},
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
                    Name = "MenuFLoor_01 (100)",
                    Transform = {
                        position = {-2, -0.5, 4.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                    position = {2, 0, 0},
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
                    Name = "MenuFLoor_01 (101)",
                    Transform = {
                        position = {-2, -0.5, 6.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                    position = {2, 0, 0},
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
                    Name = "MenuFLoor_01 (102)",
                    Transform = {
                        position = {-2, -0.5, 8.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                    position = {2, 0, 0},
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
                    Name = "MenuFLoor_01 (103)",
                    Transform = {
                        position = {-2, -0.5, 10.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                    position = {2, 0, 0},
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
                    Name = "MenuFLoor_01 (104)",
                    Transform = {
                        position = {-2, -0.5, 12.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                    position = {2, 0, 0},
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
                    Name = "MenuFLoor_01 (105)",
                    Transform = {
                        position = {-3, -0.5, 5.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                    position = {2, 0, 0},
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
                    Name = "MenuFLoor_01 (106)",
                    Transform = {
                        position = {-3, -0.5, 7.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                    position = {2, 0, 0},
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
                    Name = "MenuFLoor_01 (107)",
                    Transform = {
                        position = {-3, -0.5, 9.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                    position = {2, 0, 0},
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
                    Name = "MenuFLoor_01 (108)",
                    Transform = {
                        position = {-3, -0.5, 11.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                    position = {2, 0, 0},
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
                    Name = "MenuFLoor_01 (73)",
                    Transform = {
                        position = {1.5, -0.5, 13},
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
                    Name = "MenuFLoor_01 (74)",
                    Transform = {
                        position = {3.5, -0.5, 13},
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
                    Name = "MenuFLoor_01 (109)",
                    Transform = {
                        position = {2, -0.5, 11.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                    position = {2, 0, 0},
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
                    Name = "MenuFLoor_01 (110)",
                    Transform = {
                        position = {2, -0.5, 9.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                    position = {2, 0, 0},
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
                    Name = "MenuFLoor_01 (75)",
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
                    Name = "MenuFLoor_01 (76)",
                    Transform = {
                        position = {6.5, -0.5, 8},
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
                    Name = "MenuFLoor_01 (77)",
                    Transform = {
                        position = {8.5, -0.5, 8},
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
                    Name = "MenuFLoor_01 (77)",
                    Transform = {
                        position = {10.5, -0.5, 8},
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
                    Name = "MenuFLoor_01 (78)",
                    Transform = {
                        position = {6.5, -0.5, 9},
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
                    Name = "MenuFLoor_01 (111)",
                    Transform = {
                        position = {3, -0.5, 9.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                    position = {2, 0, 0},
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
                    Name = "MenuFLoor_01 (112)",
                    Transform = {
                        position = {9, -0.5, 6.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                    position = {2, 0, 0},
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
                    Name = "MenuFLoor_01 (113)",
                    Transform = {
                        position = {9, -0.5, 4.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                    position = {2, 0, 0},
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
                    Name = "MenuFLoor_01 (114)",
                    Transform = {
                        position = {9, -0.5, 2.5},
                        children = {
                            {
                                Name = "Group5936",
                                Transform = {
                                    position = {2, 0, 0},
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
                    Name = "MenuFLoor_01 (79)",
                    Transform = {
                        position = {10.5, -0.5, 1},
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
                    Name = "MenuFLoor_01 (80)",
                    Transform = {
                        position = {8.5, -0.5, 1},
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
                    Name = "FoundationBlock (120)",
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
                    Name = "FoundationBlock (121)",
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
                    Name = "FoundationBlock (122)",
                    Transform = {
                        position = {3, -1.49, 0},
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
                    Name = "FoundationBlock (123)",
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
                    Name = "FoundationBlock (124)",
                    Transform = {
                        position = {2, -1.49, 1},
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
                    Name = "FoundationBlock (125)",
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
                    Name = "FoundationBlock (126)",
                    Transform = {
                        position = {0, -1.49, 1},
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
                    Name = "FoundationBlock (127)",
                    Transform = {
                        position = {0, -1.49, 2},
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
                    Name = "FoundationBlock (128)",
                    Transform = {
                        position = {0, -1.49, 3},
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
                    Name = "FoundationBlock (129)",
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
                    Name = "FoundationBlock (130)",
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
                    Name = "FoundationBlock (131)",
                    Transform = {
                        position = {-1, -1.49, 5},
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
                    Name = "FoundationBlock (132)",
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
                    Name = "FoundationBlock (133)",
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
                    Name = "FoundationBlock (134)",
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
                    Name = "FoundationBlock (135)",
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
                    Name = "FoundationBlock (136)",
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
                    Name = "FoundationBlock (137)",
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
                    Name = "FoundationBlock (138)",
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
                    Name = "FoundationBlock (139)",
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
                    Name = "FoundationBlock (140)",
                    Transform = {
                        position = {6, -1.49, 1},
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
                    Name = "FoundationBlock (141)",
                    Transform = {
                        position = {7, -1.49, 1},
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
                    Name = "FoundationBlock (142)",
                    Transform = {
                        position = {8, -1.49, 1},
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
                    Name = "FoundationBlock (143)",
                    Transform = {
                        position = {9, -1.49, 1},
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
                    Name = "FoundationBlock (144)",
                    Transform = {
                        position = {10, -1.49, 1},
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
                    Name = "FoundationBlock (145)",
                    Transform = {
                        position = {11, -1.49, 1},
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
                    Name = "vine_01 (2)",
                    Transform = {
                        position = {-2.63, -0.49, 8.13},
                        rotation = {0, 269.9995, 0},
                        scale = {0.8653601, 0.86536, 0.8653601},
                        children = {
                            {
                                Name = "vine_01",
                                Transform = {
                                },
                                RenderInfo = {
                                    mesh = 'objects/decorations/Plants/vine_01.obj',
                                    material = materials[8],
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
                        position = {4.971044, 0, 5.004956},
                        rotation = {0, 89.99983, 0},
                    },
                },
                {
                    Name = "Door (1)",
                    Transform = {
                        position = {1.998956, 0, 7.995044},
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
    nextLevelPath = 'assets/scripts/scenes/level14.lua',
    ambientLighting = {color = {1.022366, 1.866412, 2.270603, 1}},
    maxNumUndos = {
        threeStars = 1,
        twoStars = 3,
        oneStar = 5
    }
}
