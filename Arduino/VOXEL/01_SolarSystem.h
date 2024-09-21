//----------------------------------SOLAR SYSTEM

#define RADIUS_SUN 3
#define RADIUS_EARTH 2
#define RADIUS_MOON 1
#define ORBIT_EARTH 10
#define ORBIT_MOON 5

void generateFlames(int centerX, int centerY, int centerZ) {
  for (int i = 0; i < MAX_FLAMES; i++) {
    if (flames[i].life == 0) {
      // New flame
      flames[i].x = centerX + random(-3, 4);  // Small range around the Sun
      flames[i].y = centerY + random(-3, 4);
      flames[i].z = centerZ + random(-3, 4);
      flames[i].dx = random(-1, 2);  // Small movement in any direction
      flames[i].dy = random(-1, 2);
      flames[i].dz = random(-1, 2);
      flames[i].life = random(5, 15);  // Short lifespan for each flame
      i = MAX_FLAMES;                  // Generate only one flame per frame
    }
  }
}

void updateAndDrawFlames() {
  for (int i = 0; i < MAX_FLAMES; i++) {
    if (flames[i].life > 0) {
      flames[i].x += flames[i].dx;
      flames[i].y += flames[i].dy;
      flames[i].z += flames[i].dz;

      // Check bounds and draw the flame
      if (flames[i].x >= 0 && flames[i].x < SIZE && flames[i].y >= 0 && flames[i].y < SIZE && flames[i].z >= 0 && flames[i].z < SIZE) {
        SET_BIT(voxelGrid, flames[i].x, flames[i].y, flames[i].z);
      }
      flames[i].life--;  // Decrease life
    }
  }
}

//----------------

void drawSphere(bool Sun, int centerX, int centerY, int centerZ, int radius) {
  for (int x = -radius; x <= radius; x++) {
    for (int y = -radius; y <= radius; y++) {
      for (int z = -radius; z <= radius; z++) {
        int distanceSquared = x * x + y * y + z * z;
        int radiusSquared = radius * radius;
        if (distanceSquared >= radiusSquared - radius && distanceSquared <= radiusSquared + radius) {
          if (Sun) {
            if (random(10) > 3) SET_BIT(voxelGrid, centerX + x, centerY + y, centerZ + z);
          } else SET_BIT(voxelGrid, centerX + x, centerY + y, centerZ + z);
        }
      }
    }
  }
}

//----------------

void drawSolarSystem3D() {

  // Draw the Sun at the center
  int sunX = SIZE / 2;
  int sunY = SIZE / 2;
  int sunZ = SIZE / 2;
  drawSphere(true, sunX, sunY, sunZ, RADIUS_SUN);

  // Generate and update dynamic sparks around the Sun
  generateFlames(sunX, sunY, sunZ);
  updateAndDrawFlames();

  // Apply random flicker effect to the Sun's surface
  //randomSurfaceFlicker(sunX, sunY, sunZ, RADIUS_SUN);

  // Calculate Earth's 3D position
  int earthX = sunX + ORBIT_EARTH * cosTable[(int)thetaEarth % TABLE_SIZE];
  int earthY = sunY + ORBIT_EARTH * sinTable[(int)thetaEarth % TABLE_SIZE] * cosTable[(int)tiltEarth % TABLE_SIZE];
  int earthZ = sunZ + ORBIT_EARTH * sinTable[(int)thetaEarth % TABLE_SIZE] * sinTable[(int)tiltEarth % TABLE_SIZE];

  // Draw Earth
  drawSphere(false, earthX, earthY, earthZ, RADIUS_EARTH);

  // Calculate Moon's 3D position relative to Earth
  int moonX = earthX + ORBIT_MOON * cosTable[(int)thetaMoon % TABLE_SIZE];
  int moonY = earthY + ORBIT_MOON * sinTable[(int)thetaMoon % TABLE_SIZE] * cosTable[(int)tiltMoon % TABLE_SIZE];
  int moonZ = earthZ + ORBIT_MOON * sinTable[(int)thetaMoon % TABLE_SIZE] * sinTable[(int)tiltMoon % TABLE_SIZE];

  // Draw Moon
  drawSphere(false, moonX, moonY, moonZ, RADIUS_MOON);

  // Update angles for next frame
  thetaEarth += speedEarth;
  thetaMoon += speedMoon;

  if (thetaEarth >= 360) thetaEarth -= 360;
  if (thetaMoon >= 360) thetaMoon -= 360;
}
