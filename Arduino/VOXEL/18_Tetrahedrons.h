//---------------------------------- TETRAHEDRONS

int tRadius = 8;
int angleOffset = 120;
int baseHeight = 26; //to be centered
int topHeight = round(tRadius * 1.43); //10

// Rotate each tetrahedron by increasing the angle
float tetraRotationAngle = 0.0;
float globalRotationAngle = 0.0;


void rotateAroundZ(int& x, int& y, float angle, int centerX, int centerY) {
    // Use pre-calculated sin and cos tables to rotate around Z-axis relative to the local center
    int relX = x - centerX;
    int relY = y - centerY;

    int newX = relX * cosTable[(int)angle] - relY * sinTable[(int)angle];
    int newY = relX * sinTable[(int)angle] + relY * cosTable[(int)angle];

    x = newX + centerX;  // Translate back to global position
    y = newY + centerY;  // Translate back to global position
}

void drawTetrahedronWithRotation(int centerX, int centerY, int centerZ, int tRadius) {
    int vertices[4][3];  // Store the 4 vertices of the tetrahedron

    // Calculate base vertices
    for (int i = 0; i < 3; i++) {
        int baseAngle = i * angleOffset;
        vertices[i][0] = centerX + tRadius * cosTable[baseAngle];  // X
        vertices[i][1] = centerY + tRadius * sinTable[baseAngle];  // Y
        vertices[i][2] = centerZ;  // Z height of the base
    }

    // Calculate the top vertex
    vertices[3][0] = centerX;
    vertices[3][1] = centerY;
    vertices[3][2] = centerZ - topHeight;  // Top height

    // Rotate the base vertices around their local center (centerX, centerY)
    for (int i = 0; i < 3; i++) {
        rotateAroundZ(vertices[i][0], vertices[i][1], tetraRotationAngle, centerX, centerY);
        rotateAroundZ(vertices[i][0], vertices[i][1], globalRotationAngle, center, center);
    }

    // Rotate the top vertex as well (it should follow the same rotation as the base)
    rotateAroundZ(vertices[3][0], vertices[3][1], tetraRotationAngle, centerX, centerY);
    rotateAroundZ(vertices[3][0], vertices[3][1], globalRotationAngle, center, center);

    // Draw the tetrahedron edges manually, vertex-by-vertex
    for (int i = 0; i < 3; i++) {
        drawLine3D(voxelGrid, vertices[i][0], vertices[i][1], vertices[i][2], vertices[(i + 1) % 3][0], vertices[(i + 1) % 3][1], vertices[(i + 1) % 3][2]);  // Base edges
        drawLine3D(voxelGrid, vertices[i][0], vertices[i][1], vertices[i][2], vertices[3][0], vertices[3][1], vertices[3][2]);  // Top vertex to base
    }
}

//---------------------------------- MAIN

void drawTetra() {
   
    if ((int)tetraRotationAngle % 120 < 10) {
      //Small pause
      tetraRotationAngle -= 0.6; 
      globalRotationAngle += 0.2;
    } else {
      // Increase rotation angle 
      tetraRotationAngle -= 6.0; 
      globalRotationAngle += 2;
    }

    if (tetraRotationAngle < 0) tetraRotationAngle = 359;
    if (globalRotationAngle > 359) globalRotationAngle = 0;

    // Draw the 3 base tetrahedrons with rotation
    for (int i = 0; i < 3; i++) {
        int baseAngle = i * angleOffset;
        int centerX = center + tRadius * cosTable[baseAngle];  // Base positions remain fixed
        int centerY = center + tRadius * sinTable[baseAngle];

        // Rotate each tetrahedron individually around its own axis
        drawTetrahedronWithRotation(centerX, centerY, baseHeight, tRadius);
    }

    // Draw the top tetrahedron with rotation (around its own axis)
    drawTetrahedronWithRotation(center, center, baseHeight - topHeight, tRadius);
}


