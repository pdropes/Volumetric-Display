//----------------------------------STAR with sparkles

void drawStarOfDavid(uint32_t *voxelGrid) {
    // Define the vertices of the first tetrahedron (Pyramid 1)
    int v1[4][3] = {{24, 20, 12}, {7, 20, 12}, {16, 6, 12}, {16, 16, 26}};
    
    // Define the vertices of the second tetrahedron (Pyramid 2)
    int v2[4][3] = {{24, 11, 19}, {7, 11, 19}, {16, 25, 19}, {16, 16, 6}};
    
    // Draw the edges of the first tetrahedron
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            drawLine3D(voxelGrid, v1[i][0], v1[i][1], v1[i][2], v1[j][0], v1[j][1], v1[j][2]);
        }
    }

    // Draw the edges of the second tetrahedron
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            drawLine3D(voxelGrid, v2[i][0], v2[i][1], v2[i][2], v2[j][0], v2[j][1], v2[j][2]);
        }
    }
}