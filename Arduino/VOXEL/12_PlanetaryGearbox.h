//----------------------------------PLANETARY GEAR

#define VOXEL_SIZE 1024
#define SUN_GEAR_ANGLE_INCREMENT 5       // Speed of the central gear rotation
#define PLANET_GEAR_ANGLE_INCREMENT -4   // Slower speed for planet gears around their own axis
#define PLANET_GEAR_ORBIT_INCREMENT 1    // Speed for planet gears orbiting around the central gear
#define RING_GEAR_ANGLE_INCREMENT -2


void allocateAnimationFrames() {
  // Allocate memory for animationFrames in PSRAM
  animationFrames = (uint32_t(*)[1024])ps_malloc(NUM_FRAMES * 1024 * sizeof(uint32_t));

  for (int i = 0; i < NUM_FRAMES; i++) {
   // animationFrames[i] = (uint32_t*)ps_malloc(VOXEL_SIZE * sizeof(uint32_t));
    memset(animationFrames[i], 0, VOXEL_SIZE * sizeof(uint32_t));  // Initialize with 0s
  }
}

void clearGrid(uint32_t grid[VOXEL_SIZE]) {
    memset(grid, 0, VOXEL_SIZE * sizeof(uint32_t));
}

void drawGear(uint32_t grid[VOXEL_SIZE], int points, int centerX, int centerY, int centerZ, int radius, float angle, int zOffset = 0) {
  int pointsAngle = 360 / points;
    for (int i = 0; i < 360; i += pointsAngle) {  // 8 points around the gear
        int x = centerX + radius * cos((i + angle) * PI / 180.0);
        int y = centerY + radius * sin((i + angle) * PI / 180.0);
        SET_BIT(grid, x, y, centerZ + zOffset );
        SET_BIT(grid, x, y, centerZ + zOffset + 1);
        SET_BIT(grid, x, y, centerZ + zOffset + 2);

    }
}

void precalculatePlanetaryGearboxAnimation() {
    float sunGearAngle = 0;
    float planetGearAngle = 0;
    float ringGearAngle = 0;
    float ringOrbitAngle = 0;

    for (int frame = 0; frame < NUM_FRAMES; frame++) {
        clearGrid(animationFrames[frame]);

        // Draw the sun gear (center gear)
        drawGear(animationFrames[frame], 8, 16, 16, 16, 5, sunGearAngle);

        // Draw the planet gears (around the sun gear) with a slight Z-axis tilt and orbit
        for (int i = 0; i < 3; i++) {
            float orbitAngle = (i * 120 + ringOrbitAngle) * PI / 180.0;  // Orbit angle calculation
            int orbitRadius = 10;  // Distance from the sun gear center

            int planetX = 16 + orbitRadius * cos(orbitAngle);  // X position with orbit
            int planetY = 16 + orbitRadius * sin(orbitAngle);  // Y position with orbit
            
            // Apply Z-axis offset based on the frame to simulate tilt
            int zOffset = 3 * sin((sunGearAngle + i * 120) * PI / 180.0);

            // Draw the planet gear with its own rotation and orbit
            drawGear(animationFrames[frame], 6, planetX, planetY, 16, 3, planetGearAngle, zOffset);
        }

        // Draw the ring gear (outer gear)
        drawGear(animationFrames[frame], 16, 16, 16, 16, 15, ringGearAngle);

        // Update angles for the next frame
        sunGearAngle += SUN_GEAR_ANGLE_INCREMENT;
        planetGearAngle += PLANET_GEAR_ANGLE_INCREMENT;  // Opposite rotation for planet gears
        ringGearAngle += RING_GEAR_ANGLE_INCREMENT;
        ringOrbitAngle += PLANET_GEAR_ORBIT_INCREMENT; // Velocity of 3 gear, center at sun
    }
}





