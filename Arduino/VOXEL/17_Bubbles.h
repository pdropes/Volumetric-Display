//----------------------------------BUBBLES

unsigned long lastBubbleTime = 0;
const unsigned long bubbleCreationInterval = 500; // Create a bubble every 1/5second

void initializeBubbles() {
  for (int i = 0; i < MAX_BUBBLES; i++) {
    bubbles[i].active = false;  // Initially, all bubbles are inactive
  }
}

void createBubble() {
  // Find an inactive bubble slot
  for (int i = 0; i < MAX_BUBBLES; i++) {
    if (!bubbles[i].active) {
      // Set random position and radius
      int bubbleRadius = random(2, 5);         // Radius between 1 and 6
      bubbles[i].x = random(bubbleRadius + 3, 28 - bubbleRadius);//random(0, 32);
      bubbles[i].y = random(bubbleRadius + 3, 28 - bubbleRadius);//random(0, 32);
      bubbles[i].z = 31 + (bubbleRadius * 2);                    // Start at the bottom
      bubbles[i].zPosition = 31.0 + (bubbleRadius * 2);
      bubbles[i].radius = bubbleRadius;
      bubbles[i].speed = 0.05 * bubbleRadius;  // Larger bubbles rise faster
      bubbles[i].active = true;                // Mark this bubble as active
      i = MAX_BUBBLES;                         // Create one bubble at a time
    }
  }
}

void checkBubbleCreation() {
  unsigned long currentTime = millis();  
  // If it's time to create a new bubble
  if (currentTime - lastBubbleTime >= bubbleCreationInterval) {
    createBubble();
    lastBubbleTime = currentTime;  // Update the last bubble creation time
  }
}

void moveBubbles() {
  for (int i = 0; i < MAX_BUBBLES; i++) {
    if (bubbles[i].active) {
      // Increase the z position based on the speed
      bubbles[i].zPosition -= bubbles[i].speed;  // Update the float Z position
      bubbles[i].z = (int)bubbles[i].zPosition;  // Convert to integer for drawing
      // Tilt bubbles
      int rand = random(0, 50);
      if (rand == 0) bubbles[i].x += 1; 
      if (rand == 1) bubbles[i].x -= 1; 
      if (rand == 2) bubbles[i].y += 1;
      if (rand == 3) bubbles[i].y -= 1;
      // If the bubble reaches the top, deactivate it
      if (bubbles[i].z <= 0) {
        bubbles[i].active = false;  // Deactivate the bubble
      }
    }
  }
}


void drawBubble(Bubble &bubble) {
  int radiusSquared = bubble.radius * bubble.radius;
  int innerRadiusSquared = (bubble.radius - 1) * (bubble.radius - 1);
  for (int x = bubble.x - bubble.radius; x <= bubble.x + bubble.radius; x++) {
    for (int y = bubble.y - bubble.radius; y <= bubble.y + bubble.radius; y++) {
      for (int z = bubble.z - bubble.radius; z <= bubble.z + bubble.radius; z++) {
        // Ensure we're within grid bounds
        if (x >= 0 && x < 32 && y >= 0 && y < 32 && z >= 0 && z < 32) {
          // Calculate the distance from the current voxel to the bubble's center
          int dx = x - bubble.x;
          int dy = y - bubble.y;
          int dz = z - bubble.z;

          // Squared distance from the center of the bubble
          int distanceSquared = dx * dx + dy * dy + dz * dz;

          // Check if the voxel is on the outer shell of the bubble
          if (distanceSquared <= radiusSquared && distanceSquared > innerRadiusSquared) {
            SET_BIT(voxelGrid, x, y, z);  // Draw the voxel
          }
        }
      }
    }
  }
}

void renderBubbles() {
  for (int i = 0; i < MAX_BUBBLES; i++) {
    if (bubbles[i].active) {
      drawBubble(bubbles[i]);
    }
  }
}

void updateBubbles() {
  checkBubbleCreation();  // Create new bubbles over time
  moveBubbles();          // Update the positions of active bubbles
  renderBubbles();        // Render the active bubbles
}

