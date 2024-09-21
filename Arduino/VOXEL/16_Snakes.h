//----------------------------------SNAKES

#define GRID_SIZE 32

// Array to store multiple snakes
Snake snakes[MAX_SNAKES];

// Directions: +X, -X, +Y, -Y, +Z, -Z
const int directionVectors[DIRECTIONS][3] = {
  {  1,  0,  0 },   // +X
  { -1,  0,  0 },  // -X
  {  0,  1,  0 },   // +Y
  {  0, -1,  0 },  // -Y
  {  0,  0,  1 },   // +Z
  {  0,  0, -1 }   // -Z
};

bool isWithinCircularBoundary(int x, int y) {
    // Check if the point (x, y) is within the circular boundary
    float dx = x - center;
    float dy = y - center;
    return (dx * dx + dy * dy <= center2);
}

void initializeSnake(Snake &snake) {
    // Random initial position within valid bounds
    int startX, startY, startZ;
    do {
        startX = random(0, GRID_SIZE);
        startY = random(0, GRID_SIZE);
        startZ = random(0, GRID_SIZE);
    } while (!isWithinCircularBoundary(startX, startY));

    // Initialize the snake body in a straight line along -X axis
    for (int i = 0; i < SNAKE_LENGTH; i++) {
        snake.body[i] = {startX - i, startY, startZ};  // Snake initially moves along -X
    }

    // Set a random initial direction
    snake.headDirection = random(0, DIRECTIONS);
}

void moveSnake(Snake &snake) {
  // Calculate the new head position based on the current direction
  Vertex newHead = {
    snake.body[0].x + directionVectors[snake.headDirection][0],
    snake.body[0].y + directionVectors[snake.headDirection][1],
    snake.body[0].z + directionVectors[snake.headDirection][2]
  };
    // Check if the new head is out of bounds
    if (!isWithinCircularBoundary(newHead.x, newHead.y) || 
        newHead.z < 0 || newHead.z >= GRID_SIZE) {
    // Pick a new random direction until it's valid
    int newDirection;
    do {
      newDirection = random(0, DIRECTIONS);
      newHead.x = snake.body[0].x + directionVectors[newDirection][0];
      newHead.y = snake.body[0].y + directionVectors[newDirection][1];
      newHead.z = snake.body[0].z + directionVectors[newDirection][2];
    } while (!isWithinCircularBoundary(newHead.x, newHead.y) || 
            newHead.z < 0 || newHead.z >= GRID_SIZE);

    // Update the snake's direction to the new valid one
    snake.headDirection = newDirection;
  } else {
    // Randomly change direction with some probability
    if (random(0, 10) > 7) {
      snake.headDirection = random(0, DIRECTIONS);
    }
  }
  // Shift the body: Move each segment to the position of the one in front of it
  for (int i = SNAKE_LENGTH - 1; i > 0; i--) {
    snake.body[i] = snake.body[i - 1];
  }
  // Update the head with the new position
  snake.body[0] = newHead;
}

// Function to draw all snakes on the voxel grid
void drawSnakes() {
  // Iterate through each snake
  for (int i = 0; i < MAX_SNAKES; i++) {
    // Draw lines connecting each consecutive body segment
    for (int j = 0; j < SNAKE_LENGTH - 1; j++) {
      drawLine3D(voxelGrid,
                 snakes[i].body[j].x, snakes[i].body[j].y, snakes[i].body[j].z,
                 snakes[i].body[j + 1].x, snakes[i].body[j + 1].y, snakes[i].body[j + 1].z);
    }
  }
}

// Function to update all snakes' movement and redraw the grid
void updateSnakes() {
  // Move and update each snake
  for (int i = 0; i < MAX_SNAKES; i++) {
    moveSnake(snakes[i]);
  }
  // After updating positions, draw all the snakes
  drawSnakes();
}