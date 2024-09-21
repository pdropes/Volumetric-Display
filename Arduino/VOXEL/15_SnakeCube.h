//----------------------------------SNAKE CUBE

// Vertices of a cube (with edges on the boundaries of a 32x32x32 grid)
// Phase 1 edges
int phase1Edges[6][2] = {
    {0, 3}, {0, 4}, {0, 1},
    {6, 7}, {6, 2}, {6, 5}
};

// Phase 2 edges
int phase2Edges[6][2] = {
    {3, 7}, {4, 5}, {1, 2},
    {7, 4}, {2, 3}, {5, 1}
};

// Array to hold multiple cubes
Cube cubes[MAX_CUBES];

// Function to initialize new random cube properties
void initializeCube(Cube &cube, Cube cubes[], bool StartUP = false) {

    if (StartUP) {
      cube.currentPhase = random(1, 3);            //1;
      cube.edgeProgress = random(0, 100) / 100.0;  //0;
      cube.erasing = random(0, 2);                 //false;
    } else {
      cube.currentPhase = 1;
      cube.edgeProgress = 0;
      cube.erasing = false;
    }

    int size = random(5, 12);  // Random size
    int offsetX = random(0, SIZE - size);
    int offsetY = random(0, SIZE - size);
    int offsetZ = random(0, SIZE - size);

    // Define the vertices of the cube
    cube.vertices[0] = { offsetX, offsetY, offsetZ };
    cube.vertices[1] = { offsetX + size, offsetY, offsetZ };
    cube.vertices[2] = { offsetX + size, offsetY + size, offsetZ };
    cube.vertices[3] = { offsetX, offsetY + size, offsetZ };
    cube.vertices[4] = { offsetX, offsetY, offsetZ + size };
    cube.vertices[5] = { offsetX + size, offsetY, offsetZ + size };
    cube.vertices[6] = { offsetX + size, offsetY + size, offsetZ + size };
    cube.vertices[7] = { offsetX, offsetY + size, offsetZ + size };
}

// Function to draw or erase edges for a given cube
void drawPhaseEdges(Vertex vertices[8], int phaseEdges[6][2], float progress, bool erase = false) {
    for (int i = 0; i < 6; i++) {
        // Get the start and end points of the current edge
        int startX = vertices[phaseEdges[i][0]].x;
        int startY = vertices[phaseEdges[i][0]].y;
        int startZ = vertices[phaseEdges[i][0]].z;

        int endX = vertices[phaseEdges[i][1]].x;
        int endY = vertices[phaseEdges[i][1]].y;
        int endZ = vertices[phaseEdges[i][1]].z;

        int currentX, currentY, currentZ;

        if (!erase) {
            // Calculate the current position based on progress (for drawing)
            currentX = startX + (int)((endX - startX) * progress);
            currentY = startY + (int)((endY - startY) * progress);
            currentZ = startZ + (int)((endZ - startZ) * progress);
            // Draw the current portion of the edge
            drawLine3D(voxelGrid, startX, startY, startZ, currentX, currentY, currentZ);
        } else {
            // Calculate the current position based on progress (for erasing)
            currentX = endX - (int)((endX - startX) * progress);
            currentY = endY - (int)((endY - startY) * progress);
            currentZ = endZ - (int)((endZ - startZ) * progress);
            // Erase the current portion of the edge
            drawLine3D(voxelGrid, currentX, currentY, currentZ, endX, endY, endZ);
        }
    }
}

// Function to handle the drawing or erasing of a single cube
void drawCubeEdges(Cube &cube) {
    if (!cube.erasing) {
        // Drawing phase
        if (cube.currentPhase == 1) {
            drawPhaseEdges(cube.vertices, phase1Edges, cube.edgeProgress);
        } else if (cube.currentPhase == 2) {
            drawPhaseEdges(cube.vertices, phase1Edges, 1.0);  // Phase 1 is fully drawn
            drawPhaseEdges(cube.vertices, phase2Edges, cube.edgeProgress);
        }

        // Update edge progress
        cube.edgeProgress += 0.07; // Speed for drawing

        // Move to the next phase when progress completes
        if (cube.edgeProgress >= 1.0) {
            if (cube.currentPhase == 1) {
                cube.edgeProgress = 0.0;
                cube.currentPhase = 2;
            } else {
                cube.edgeProgress = 1.0;
                cube.currentPhase = 1;
                cube.erasing = true;  // Start erasing
            }
        }
    } else {
        // Erasing phase
        if (cube.currentPhase == 1) {
            drawPhaseEdges(cube.vertices, phase2Edges, 1.0); // Ensure Phase 2 is fully drawn
            drawPhaseEdges(cube.vertices, phase1Edges, cube.edgeProgress, true);
        } else {
            drawPhaseEdges(cube.vertices, phase2Edges, cube.edgeProgress, true);
        }

        // Update progress for erasing
        cube.edgeProgress -= random(2, 5) * 0.01;

        if (cube.edgeProgress <= 0.0) {
            if (cube.currentPhase == 1) {
                cube.edgeProgress = 1.0;
                cube.currentPhase = 2;
            } else {
                initializeCube(cube, cubes, false);  // Reinitialize this cube
            }
        }
    }
}

void initSnakeCube() {
  for (int i = 1; i < MAX_CUBES; i++) {
    initializeCube(cubes[i], cubes, true);
  }
}

void updateCubeEdges() {
  for (int i = 0; i < MAX_CUBES; i++) {
    drawCubeEdges(cubes[i]);
  }
}