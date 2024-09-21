//----------------------------------BOUNCING 3 balls

void initBalls() {
  // Initialize balls with starting positions and directions
  for (int i = 0; i < NUM_BALLS; i++) {
    balls[i].x = random(BALL_RADIUS, FIELD_SIZE - BALL_RADIUS);
    balls[i].y = random(BALL_RADIUS, FIELD_SIZE - BALL_RADIUS);
    balls[i].z = random(BALL_RADIUS, FIELD_SIZE - BALL_RADIUS);
    balls[i].dx = random(1, 3) * (random(0, 2) == 0 ? 1 : -1);
    balls[i].dy = random(1, 3) * (random(0, 2) == 0 ? 1 : -1);
    balls[i].dz = random(1, 3) * (random(0, 2) == 0 ? 1 : -1);
  }
}


bool isWithinCircle(int8_t x, int8_t y) {
  int8_t centerX = FIELD_RADIUS;
  int8_t centerY = FIELD_RADIUS;
  int8_t dx = x - centerX;
  int8_t dy = y - centerY;
  return (dx * dx + dy * dy <= FIELD_RADIUS * FIELD_RADIUS);
}

//----------------------------------Exterior Ball
void drawBall(int8_t cx, int8_t cy, int8_t cz, bool state) {
  int radiusSquared = BALL_RADIUS * BALL_RADIUS;
  int innerRadiusSquared = (BALL_RADIUS - 1) * (BALL_RADIUS - 1);

  for (int8_t xOff = -BALL_RADIUS; xOff <= BALL_RADIUS; xOff++) {
    for (int8_t yOff = -BALL_RADIUS; yOff <= BALL_RADIUS; yOff++) {
      for (int8_t zOff = -BALL_RADIUS; zOff <= BALL_RADIUS; zOff++) {
        int8_t x = cx + xOff;
        int8_t y = cy + yOff;
        int8_t z = cz + zOff;

        //Exterior Ball :
        int distanceSquared = xOff * xOff + yOff * yOff + zOff * zOff;

        // Check if the voxel is on the outer shell of the ball
        if (distanceSquared <= radiusSquared && distanceSquared > innerRadiusSquared) {
          if (state) SET_BIT(voxelGrid, x, y, z);
          else CLEAR_BIT(voxelGrid, x, y, z);
        }
      }
    }
  }
}


bool checkCollision(Ball& a, Ball& b) {
  int dx = a.x - b.x;
  int dy = a.y - b.y;
  int dz = a.z - b.z;
  return (dx * dx + dy * dy + dz * dz <= BALL_DIAMETER * BALL_DIAMETER);
}

void handleCollision(Ball& a, Ball& b) {
  // Reflect velocities to simulate a simple elastic collision
  int8_t tempDx = a.dx;
  int8_t tempDy = a.dy;
  int8_t tempDz = a.dz;

  a.dx = b.dx;
  a.dy = b.dy;
  a.dz = b.dz;

  b.dx = tempDx;
  b.dy = tempDy;
  b.dz = tempDz;
}

void updateBalls() {
  for (int i = 0; i < NUM_BALLS; i++) {
    // Clear the current ball position
    drawBall(balls[i].x, balls[i].y, balls[i].z, false);

    // Predict next position
    int8_t nextX = balls[i].x + balls[i].dx;
    int8_t nextY = balls[i].y + balls[i].dy;
    int8_t nextZ = balls[i].z + balls[i].dz;

    // Bounce off the circular walls when the ball touches them
    if (nextX - BALL_RADIUS < 0 || nextX + BALL_RADIUS >= FIELD_SIZE || !isWithinCircle(nextX, balls[i].y)) {
      balls[i].dx = -balls[i].dx;
      nextX = balls[i].x;  // Correct position if it was outside
    }
    if (nextY - BALL_RADIUS < 0 || nextY + BALL_RADIUS >= FIELD_SIZE || !isWithinCircle(balls[i].x, nextY)) {
      balls[i].dy = -balls[i].dy;
      nextY = balls[i].y;  // Correct position if it was outside
    }

    // Bounce off the top and bottom walls in the Z axis
    if (nextZ - BALL_RADIUS < 0 || nextZ + RADIUS >= FIELD_SIZE) {
      balls[i].dz = -balls[i].dz;
      nextZ = balls[i].z;  // Correct position if it was outside
    }

    // Update position
    balls[i].x = nextX;
    balls[i].y = nextY;
    balls[i].z = nextZ;

    // Set the new ball position
    drawBall(balls[i].x, balls[i].y, balls[i].z, true);
  }

  // Handle collisions between balls
  for (int i = 0; i < NUM_BALLS; i++) {
    for (int j = i + 1; j < NUM_BALLS; j++) {
      if (checkCollision(balls[i], balls[j])) {
        // Reflect velocities and adjust positions to avoid sticking
        handleCollision(balls[i], balls[j]);

        // Move balls apart to prevent sticking
        int8_t dx = balls[i].x - balls[j].x;
        int8_t dy = balls[i].y - balls[j].y;
        int8_t dz = balls[i].z - balls[j].z;
        float distance = sqrt(dx * dx + dy * dy + dz * dz);
        float overlap = BALL_DIAMETER - distance;
        if (distance > 0) {
          float moveX = dx / distance * overlap / 2;
          float moveY = dy / distance * overlap / 2;
          float moveZ = dz / distance * overlap / 2;
          balls[i].x += moveX;
          balls[i].y += moveY;
          balls[i].z += moveZ;
          balls[j].x -= moveX;
          balls[j].y -= moveY;
          balls[j].z -= moveZ;
        }
      }
    }
    // Avoid sticking on walls
    if (balls[i].x > FIELD_SIZE - BALL_RADIUS) balls[i].x -= 1;
    if (balls[i].y > FIELD_SIZE - BALL_RADIUS) balls[i].y -= 1;
    if (balls[i].z > FIELD_SIZE - BALL_RADIUS) balls[i].z -= 1;
    if (balls[i].x < BALL_RADIUS) balls[i].x += 1;
    if (balls[i].y < BALL_RADIUS) balls[i].y += 1;
    if (balls[i].z < BALL_RADIUS) balls[i].z += 1;
  }
}