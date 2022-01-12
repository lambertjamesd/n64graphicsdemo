Vtx ground_Plane_normal[] = {
    {{{500, 0, -500}, 0, {2048, 2048}, {0, 127, 0, 255}}},
    {{{-500, 0, -500}, 0, {0, 2048}, {0, 127, 0, 255}}},
    {{{-500, 0, 500}, 0, {0, 0}, {0, 127, 0, 255}}},
    {{{500, 0, 500}, 0, {2048, 0}, {0, 127, 0, 255}}},
};


Gfx ground_model_gfx[] = {
    // Material DefaultMaterial
    // End Material DefaultMaterial
    gsSPVertex(&ground_Plane_normal[0], 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSPEndDisplayList(),
};
