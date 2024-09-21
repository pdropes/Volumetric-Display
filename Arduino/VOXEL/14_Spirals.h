//----------------------------------SPIRALS

#define numSpirals 10
const float TABLE_Spirals = TABLE_SIZE / numSpirals;


void drawSpirals(int frame) {
  float spiralRadius = 8.0;        // Adjust the radius for spacing between spirals
  float selfRotationRadius = 5.0;  // Radius for the self-rotation of each spiral

  for (int i = 0; i < numSpirals; i++) {
    // Calculate dynamic starting positions for each spiral around the center
    float startAngle = radians((float)(i * TABLE_Spirals + frame * 2));  // º separation, rotates with frame
    float startX = center + spiralRadius * cos(startAngle);              // X position rotates around the center
    float startY = center + spiralRadius * sin(startAngle);              // Y position rotates around the center

    for (int z = 0; z < 32; z++) {  // Traverse the height (Z-axis)

      // Calculate the self-rotation angle for each spiral
      int angleIndex = (int)(frame + (z * TABLE_SIZE / 32) + (i * TABLE_Spirals)) % TABLE_SIZE;  // º between spirals

      // Self-rotation for each spiral
      int x = (int)(startX + (selfRotationRadius * cosTable[angleIndex]));  // Spiral radius for self-rotation
      int y = (int)(startY + (selfRotationRadius * sinTable[angleIndex]));

      // Ensure we are within bounds before setting the voxel
      if (x >= 0 && x < 32 && y >= 0 && y < 32) {
        SET_BIT(voxelGrid, x, y, z);  // Set the voxel at the calculated position
      }

      //-------------- Offset spirals

      // Overlapping spirals, 180º out of phase
      angleIndex = (int)(frame + 180 + (z * TABLE_SIZE / 32) + (i * TABLE_Spirals)) % TABLE_SIZE;  // º between spirals

      // Self-rotation for each spiral
      x = (int)(startX + (selfRotationRadius * cosTable[angleIndex]));  // Spiral radius for self-rotation
      y = (int)(startY + (selfRotationRadius * sinTable[angleIndex]));

      // Ensure we are within bounds before setting the voxel
      if (x >= 0 && x < 32 && y >= 0 && y < 32) {
        SET_BIT(voxelGrid, x, y, z);  // Set the voxel at the calculated position
      }
    }
  }
}
