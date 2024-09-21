//----------------------------------TORNADO

//---Particles

void initializeParticles() {
  for (int i = 0; i < NUM_PARTICLES; i++) {
    int angleIndex = random(0, TABLE_SIZE);                   // Use index to access precomputed values

    particles[i].angleIndex = angleIndex;                     // Store the index of the angle
    particles[i].height = random(0, TORNADO_HEIGHT);          // Random starting height
    particles[i].x = TORNADO_SIZE_2 + TORNADO_RADIUS * cosTable[angleIndex];  // Initial X position
    particles[i].y = TORNADO_SIZE_2 + TORNADO_RADIUS * sinTable[angleIndex];  // Initial Y position
    particles[i].z = particles[i].height;                                     // Initial Z position
  }
}


void updateParticles() {
  for (int i = 0; i < NUM_PARTICLES; i++) {
    // Update the angle index to rotate the particle around the tornado center
    particles[i].angleIndex = (particles[i].angleIndex + 5) % TABLE_SIZE;

    // Simulate the vortex effect by reducing the radius as the height increases
    float radius = (1 - (particles[i].height / TORNADO_SIZE)) * TORNADO_RADIUS;

    // Update the X, Y position based on the current angle index and radius using the lookup table
    particles[i].x = TORNADO_SIZE_2 + radius * cosTable[particles[i].angleIndex];
    particles[i].y = TORNADO_SIZE_2 + radius * sinTable[particles[i].angleIndex];

    // Update the Z position to simulate rising
    particles[i].height += 0.5;
    if (particles[i].height >= TORNADO_HEIGHT) {
      particles[i].height = 0;  // Reset height to simulate continuous motion
    }
    particles[i].z = particles[i].height;
  }
}

void drawParticles() {
  // Clear the voxel grid
  memset(voxelGrid, 0, sizeof(voxelGrid));

  // Draw each particle in its current position
  for (int i = 0; i < NUM_PARTICLES; i++) {
    if ((int8_t)particles[i].z > 0) {  //exludes the top layer, garbage at center
      SET_BIT(voxelGrid, (int8_t)particles[i].x, (int8_t)particles[i].y, (int8_t)particles[i].z);
    }
  }
}

//--- Debris

void initializeDebris() {
  for (int i = 0; i < NUM_DEBRIS; i++) {
    int angleIndex = random(0, TABLE_SIZE);                    // Use index to access precomputed values
    float radius = random(TORNADO_RADIUS, MAX_DEBRIS_RADIUS);  // Random radius within bounds
    debris[i].angleIndex = angleIndex;                         // Store the index of the angle

    debris[i].initialRadius = radius;              // Store initial radius
    debris[i].height = random(0, TORNADO_HEIGHT);  // Random height

    // Initialize x and y based on precomputed values
    debris[i].x = TORNADO_SIZE_2 + radius * cosTable[angleIndex];
    debris[i].y = TORNADO_SIZE_2 + radius * sinTable[angleIndex];
    debris[i].z = debris[i].height;  // Fixed height

    // Calculate and store initial speed
    debris[i].speed = (float)random(MIN_SPEED, MAX_SPEED) / 1000; //calculateSpeed(radius);
  }
}


void updateDebris() {
  for (int i = 0; i < NUM_DEBRIS; i++) {
    // Get the fixed radius of the debris
    float radius = debris[i].initialRadius;

    // Update the angle index to rotate the debris around the center
    debris[i].angleIndex = (debris[i].angleIndex + (int)(debris[i].speed * 10)) % TABLE_SIZE;  // Increment angle index and wrap around

    // Update the X, Y position based on the current angle index and radius using the lookup table
    debris[i].x = TORNADO_SIZE_2 + radius * cosTable[debris[i].angleIndex];
    debris[i].y = TORNADO_SIZE_2 + radius * sinTable[debris[i].angleIndex];

    debris[i].z = debris[i].height; //dont move
  }
}


void drawDebris() {
  for (int i = 0; i < NUM_DEBRIS; i++) {
    int8_t x = (int8_t)debris[i].x;
    int8_t y = (int8_t)debris[i].y;
    int8_t z = (int8_t)debris[i].z;

    if (x >= 0 && x < TORNADO_SIZE && y >= 0 && y < TORNADO_SIZE && z >= 0 && z < TORNADO_SIZE) {
      SET_BIT(voxelGrid, x, y, z);
    }
  }
}

void updateTornado() {
  updateParticles();  // Update particles
  updateDebris();     // Update debris
  drawParticles();    // Draw particules
  drawDebris();       // Draw debris
}
