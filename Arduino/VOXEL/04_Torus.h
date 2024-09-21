//----------------------------------Rotating TORUS

#define R1 10  // Major radius of the torus def:10
#define R2 5   // Minor radius of the torus def:5
#define TORUS_DETAIL 20  // Number of divisions for the torus (higher = smoother) (def:20)

int start;
int end;

void drawTorus(int X, int Y, int Z) {  // Rotation angles)

  // Precompute rotation values
  float cosTX = cosTable[X];
  float sinTX = sinTable[X];
  float cosTY = cosTable[Y];
  float sinTY = sinTable[Y];
  float cosTZ = cosTable[Z];
  float sinTZ = sinTable[Z];

  // Draw the torus
  for (int i = 0; i < TORUS_DETAIL; i++) {
    for (int j = 0; j < TORUS_DETAIL; j++) {
      // Parametric angles
      int u = (i * 360 / TORUS_DETAIL) % TABLE_SIZE;
      int v = (j * 360 / TORUS_DETAIL) % TABLE_SIZE;

      // Torus parametric equations using lookup tables
      float x = (R1 + R2 * cosTable[v]) * cosTable[u];
      float y = (R1 + R2 * cosTable[v]) * sinTable[u];
      float z = R2 * sinTable[v];

      // Rotate around X-axis using precomputed values
      float y1 = y * cosTX - z * sinTX;
      float z1 = y * sinTX + z * cosTX;

      // Rotate around Y-axis using precomputed values
      float x2 = x * cosTY + z1 * sinTY;
      float z2 = -x * sinTY + z1 * cosTY;

      // Rotate around Z-axis using precomputed values
      float x3 = x2 * cosTZ - y1 * sinTZ;
      float y3 = x2 * sinTZ + y1 * cosTZ;

      // Translate to the center of the voxel grid
      int8_t voxelX = (int8_t)(x3 + center);
      int8_t voxelY = (int8_t)(y3 + center);
      int8_t voxelZ = (int8_t)(z2 + center);

      SET_BIT(voxelGrid, voxelX, voxelY, voxelZ);
    }
  }
}

void updateTorus() {
  // Slowly rotate on all three axes
  angleXi = (angleXi + 1) % TABLE_SIZE;
  angleYi = (angleYi + 2) % TABLE_SIZE;
  angleZi = (angleZi + 3) % TABLE_SIZE;
  drawTorus(angleXi, angleYi, angleZi);
}







    