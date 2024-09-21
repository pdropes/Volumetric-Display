//----------------------------------SHPERE

#define SPACING 2

// Lookup and rotate a point around the X axis
void rotateX(float &y, float &z, int angleIndex) {
  float temp = y;
  y = y * cosTable[angleIndex] - z * sinTable[angleIndex];
  z = temp * sinTable[angleIndex] + z * cosTable[angleIndex];
}

// Lookup and rotate a point around the Y axis
void rotateY(float &x, float &z, int angleIndex) {
  float temp = x;
  x = x * cosTable[angleIndex] + z * sinTable[angleIndex];
  z = -temp * sinTable[angleIndex] + z * cosTable[angleIndex];
}

// Lookup and rotate a point around the Z axis
void rotateZ(float &x, float &y, int angleIndex) {
  float temp = x;
  x = x * cosTable[angleIndex] - y * sinTable[angleIndex];
  y = temp * sinTable[angleIndex] + y * cosTable[angleIndex];
}

// Convert angle in radians to table index
int angleToIndex(float angle) {
  int index = (int)(degrees(angle)) % TABLE_SIZE;
  return (index < 0) ? index + TABLE_SIZE : index;
}

// Draw a sphere with rotation using lookup tables
void drawSphere(int angleX, int angleY, int angleZ) {

  float radius = (SIZE / 2.0) - SPACING;
  float cx = SIZE / 2.0, cy = SIZE / 2.0, cz = SIZE / 2.0;

  for (int thetaIndex = 0; thetaIndex < TABLE_SIZE; thetaIndex += 22) {  // ~16 points
    for (int phiIndex = 0; phiIndex < TABLE_SIZE / 2; phiIndex += 11) {  // ~16 points
      float x = radius * sinTable[phiIndex] * cosTable[thetaIndex];
      float y = radius * sinTable[phiIndex] * sinTable[thetaIndex];
      float z = radius * cosTable[phiIndex];

      // Apply rotations using lookup tables
      rotateX(y, z, angleX);
      rotateY(x, z, angleY);
      rotateZ(x, y, angleZ);

      // Translate back to grid
      int ix = (int)(cx + x);
      int iy = (int)(cy + y);
      int iz = (int)(cz + z);

      SET_BIT(voxelGrid, ix, iy, iz);
    }
  }
}

void initSphere() {
  drawSphere(0, 0, 0);
  angleXi = 0;
  angleYi = 0;
  angleZi = 0;
}

void updateSphere() {
  // Increment angles for smooth rotation
  angleXi = (angleXi + 1) % TABLE_SIZE;
  angleYi = (angleYi + 2) % TABLE_SIZE;
  angleZi = (angleZi + 3) % TABLE_SIZE;
  drawSphere(angleXi, angleYi, angleZi);
}
