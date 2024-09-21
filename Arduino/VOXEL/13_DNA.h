//----------------------------------DNA

// Get sine value from the precomputed table
float sinLUTFunc(float angle) {
  int index = (int)angle % TABLE_SIZE;
  return sinTable[index];
}

// Get cosine value from the precomputed table
float cosLUTFunc(float angle) {
  int index = (int)angle % TABLE_SIZE;
  return cosTable[index];
}

// Rotate a point around the Z axis (for animation)
void rotateZ(float &x, float &y, float angle) {
  float temp = x;
  x = x * cosLUTFunc(angle) - y * sinLUTFunc(angle);
  y = temp * sinLUTFunc(angle) + y * cosLUTFunc(angle);
}

// Function to update the trace
void updateTrace() {
  for (int z = 0; z < SIZE; z++) {
    for (int y = 0; y < SIZE; y++) {
      for (int x = 0; x < SIZE; x++) {
        if (traceGrid[x][y][z] > 0) {
          traceGrid[x][y][z]--;  // Decrease lifetime
          if (traceGrid[x][y][z] > 0) {
            SET_BIT(voxelGrid, x, y, z);  // Set bit if trace still alive
          }
        }
      }
    }
  }
}

// Function to add a trace at a specific position
void addTrace(int x, int y, int z) {
  if (x < 0 || x >= SIZE || y < 0 || y >= SIZE || z < 0 || z >= SIZE) return;
  traceGrid[x][y][z] = 15;//random(10, 20);  // Random lifetime between 10 and 20
  if (z > 0) traceGrid[x][y][z - 1] = 5;
  if (z < 31) traceGrid[x][y][z + 1] = 5; 
}

// Reduce the number of rungs by controlling their spacing
void drawDNA(float rotationAngle) {

  float cx = SIZE / 2.0, cy = SIZE / 2.0;//, cz = SIZE / 2.0;
  float radius = 10.0;  // Distance between the two strands
  int helixHeight = SIZE - 1;

  for (int z = 0; z <= helixHeight; z++) {
    // Calculate positions of the two strands
    float angle = (float)z / helixHeight * 360.0;  // Full helix turn

    float x1 = radius * cosLUTFunc(angle);
    float y1 = radius * sinLUTFunc(angle);
    float x2 = radius * cosLUTFunc(angle + 180);  // Offset by 180 degrees
    float y2 = radius * sinLUTFunc(angle + 180);

    // Apply rotation to strands
    rotateZ(x1, y1, rotationAngle);
    rotateZ(x2, y2, rotationAngle);

    // Translate the strands to the center of the grid
    int ix1 = (int)(cx + x1);
    int iy1 = (int)(cy + y1);
    int iz = z;

    int ix2 = (int)(cx + x2);
    int iy2 = (int)(cy + y2);

    // Set the voxels for the two strands
    SET_BIT(voxelGrid, ix1, iy1, iz);
    SET_BIT(voxelGrid, ix1 + 1, iy1, iz);
    SET_BIT(voxelGrid, ix1 - 1, iy1, iz);
    SET_BIT(voxelGrid, ix1 , iy1 + 1, iz);
    SET_BIT(voxelGrid, ix1 , iy1 - 1, iz); 
    if (iz < 31)SET_BIT(voxelGrid, ix1 , iy1, iz + 1);
    if (iz > 0) SET_BIT(voxelGrid, ix1 , iy1, iz - 1); 

    SET_BIT(voxelGrid, ix2, iy2, iz);
    SET_BIT(voxelGrid, ix2 + 1, iy2, iz);
    SET_BIT(voxelGrid, ix2 - 1, iy2, iz);
    SET_BIT(voxelGrid, ix2 , iy2 + 1, iz);
    SET_BIT(voxelGrid, ix2 , iy2 - 1, iz); 
    if (iz < 31)SET_BIT(voxelGrid, ix2 , iy2, iz + 1);
    if (iz > 0) SET_BIT(voxelGrid, ix2 , iy2, iz - 1); 

    // Draw horizontal "rungs" every 5 units
    if (z % 5 == 0) {
      // Draw a line 3D
      drawLine3D(voxelGrid, ix1, iy1, iz, ix2, iy2, iz);  // Nucleotide rung
      // Add a trace at the ends of the rungs
      addTrace(ix1, iy1, iz);
      addTrace(ix2, iy2, iz);

    }
  }
}

void updateDNA() {
  drawDNA(rotationAngle);  // Draw and animate the DNA
  updateTrace();           // Update the trace over time
  rotationAngle += 2;      // Rotate over time
}

