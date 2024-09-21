//----------------------------------Rotating 3axis CUBE

#define SCALE 8.5 // Scale for the rotating cube


// Define the 8 vertices of the cube (centered at the origin)
float vertices[8][3] = {
    {-1, -1, -1}, // 0
    {1, -1, -1},  // 1
    {1, 1, -1},   // 2
    {-1, 1, -1},  // 3
    {-1, -1, 1},  // 4
    {1, -1, 1},   // 5
    {1, 1, 1},    // 6
    {-1, 1, 1}    // 7
};

// Define the 12 edges of the cube, connecting the vertices
int edges[12][2] = {
    {0, 1}, {1, 2}, {2, 3}, {3, 0}, // Bottom face
    {4, 5}, {5, 6}, {6, 7}, {7, 4}, // Top face
    {0, 4}, {1, 5}, {2, 6}, {3, 7}  // Vertical edges
};

// Rotation matrices
void drawRotatedCube() {
  int voxelVertices[8][3];

  // Clear both arrays
  memset(voxelGrid, 0, sizeof(voxelGrid));
  memset(Polar, 0, sizeof(Polar));

  // Apply rotation and convert to voxel coordinates
  for (int i = 0; i < 8; i++) {
    float x = vertices[i][0] * SCALE;
    float y = vertices[i][1] * SCALE;
    float z = vertices[i][2] * SCALE;

    float x1, y1, z1;
//rotateX
    y1 =  y * cos(angleXf) - z * sin(angleXf);
    z1 =  y * sin(angleXf) + z * cos(angleXf);
    y = y1; z = z1;
//rotateY
    x1 =  x * cos(angleYf) + z * sin(angleYf);
    z1 = -x * sin(angleYf) + z * cos(angleYf);
    x = x1; z = z1;
//rotateZ
    x1 =  x * cos(angleZf) - y * sin(angleZf);
    y1 =  x * sin(angleZf) + y * cos(angleZf);
    x = x1; y = y1;

    // Convert to voxel coordinates and move to center
    voxelVertices[i][0] = round(x + center);
    voxelVertices[i][1] = round(y + center);
    voxelVertices[i][2] = round(z + center);
  }

  // Draw edges
  for (int i = 0; i < 12; i++) {
        drawLine3D(voxelGrid, voxelVertices[edges[i][0]][0], voxelVertices[edges[i][0]][1], voxelVertices[edges[i][0]][2],
                              voxelVertices[edges[i][1]][0], voxelVertices[edges[i][1]][1], voxelVertices[edges[i][1]][2]);
  }
}

void updateRotatedCube() {
        // Increment angles for rotation
        angleXf += 0.015;  //0.01
        angleYf += 0.025;  //0.02
        angleZf += 0.035;  //0.03
        drawRotatedCube();
}