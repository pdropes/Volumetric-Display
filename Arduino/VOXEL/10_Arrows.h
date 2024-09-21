//-----------------------------------ARROWS

// Initialize a new line
void initLine(int i) {
  lines[i].axis = random(3);  // Randomly choose the axis of movement (0 = X, 1 = Y, 2 = Z)
  
  if (lines[i].axis == 0) {  // Move along X
    lines[i].fixed1 = random(32);  // Random Y coordinate
    lines[i].fixed2 = random(32);  // Random Z coordinate
    lines[i].moving = (random(2) == 0) ? 0 : 31;  // Start at 0 (left) or 31 (right) along X
    lines[i].direction = (lines[i].moving == 0) ? 1 : -1;  // Direction
  } else if (lines[i].axis == 1) {  // Move along Y
    lines[i].fixed1 = random(32);  // Random X coordinate
    lines[i].fixed2 = random(32);  // Random Z coordinate
    lines[i].moving = (random(2) == 0) ? 0 : 31;  // Start at 0 (bottom) or 31 (top) along Y
    lines[i].direction = (lines[i].moving == 0) ? 1 : -1;  // Direction
  } else {  // Move along Z
    lines[i].fixed1 = random(32);  // Random X coordinate
    lines[i].fixed2 = random(32);  // Random Y coordinate
    lines[i].moving = (random(2) == 0) ? 0 : 31;  // Start at 0 (near) or 31 (far) along Z
    lines[i].direction = (lines[i].moving == 0) ? 1 : -1;  // Direction
  }

  // Avoid 15 and 16 in all coordinates
  if (lines[i].fixed1 == 15) lines[i].fixed1 = 14; 
  if (lines[i].fixed1 == 16) lines[i].fixed1 = 17;
  if (lines[i].fixed2 == 15) lines[i].fixed2 = 14; 
  if (lines[i].fixed2 == 16) lines[i].fixed2 = 17;

  lines[i].length = random(5, 17);  // Random line length between 5 and 16
  lines[i].active = true;  // Activate the line
}

// Move a line, updating its position in the grid
void moveLine(int i) {
  int index;

  if (lines[i].axis == 0) {  // Moving along X
    index = lines[i].fixed1 * 32 + lines[i].fixed2;  // Calculate the index in the 1D array

    // Clear the previous points of the line
    for (int j = 0; j < lines[i].length; j++) {
      int posX = lines[i].moving - j * lines[i].direction;
      if (posX >= 0 && posX < 32) {
        voxelGrid[posX * 32 + lines[i].fixed2] &= ~(1UL << lines[i].fixed1);
      }
    }

    // Update the position
    lines[i].moving += lines[i].direction;

    // Draw the new points of the line if within bounds
    bool outOfBounds = true;
    for (int j = 0; j < lines[i].length; j++) {
      int posX = lines[i].moving - j * lines[i].direction;
      if (posX >= 0 && posX < 32) {
        voxelGrid[posX * 32 + lines[i].fixed2] |= (1UL << lines[i].fixed1);
        outOfBounds = false;
      }
    }

    // Deactivate the line if the entire length has exited the grid
    if (outOfBounds) {
      lines[i].active = false;
    }

  } else if (lines[i].axis == 1) {  // Moving along Y
    index = lines[i].fixed1 * 32 + lines[i].fixed2;  // Calculate the index in the 1D array

    // Clear the previous points of the line
    for (int j = 0; j < lines[i].length; j++) {
      int posY = lines[i].moving - j * lines[i].direction;
      if (posY >= 0 && posY < 32) {
        voxelGrid[index] &= ~(1UL << posY);
      }
    }

    // Update the position
    lines[i].moving += lines[i].direction;

    // Draw the new points of the line if within bounds
    bool outOfBounds = true;
    for (int j = 0; j < lines[i].length; j++) {
      int posY = lines[i].moving - j * lines[i].direction;
      if (posY >= 0 && posY < 32) {
        voxelGrid[index] |= (1UL << posY);
        outOfBounds = false;
      }
    }

    // Deactivate the line if the entire length has exited the grid
    if (outOfBounds) {
      lines[i].active = false;
    }

  } else {  // Moving along Z
    index = lines[i].moving * 32 + lines[i].fixed2;  // Calculate the index in the 1D array

    // Clear the previous points of the line
    for (int j = 0; j < lines[i].length; j++) {
      int posZ = lines[i].moving - j * lines[i].direction;
      if (posZ >= 0 && posZ < 32) {
        voxelGrid[lines[i].fixed1 * 32 + posZ] &= ~(1UL << lines[i].fixed2);
      }
    }

    // Update the position
    lines[i].moving += lines[i].direction;

    // Draw the new points of the line if within bounds
    bool outOfBounds = true;
    for (int j = 0; j < lines[i].length; j++) {
      int posZ = lines[i].moving - j * lines[i].direction;
      if (posZ >= 0 && posZ < 32) {
        voxelGrid[lines[i].fixed1 * 32 + posZ] |= (1UL << lines[i].fixed2);
        outOfBounds = false;
      }
    }

    // Deactivate the line if the entire length has exited the grid
    if (outOfBounds) {
      lines[i].active = false;
    }
  }
}

void updateArrows() {
  // Update each line
  for (int i = 0; i < maxLines; i++) {
    if (lines[i].active) {
      moveLine(i);
    } else if (random(100) < 20) {  // 20% chance to start a new line each interval
      initLine(i);
    }
  }
}