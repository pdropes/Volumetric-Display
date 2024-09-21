//---------------------------------RAIN v1.1 (remove multiplications, optimize clear layer 0)

#define PERSISTENCE_FRAMES 20  // Number of frames a drop stays at the last level

void initializeLayers() {
    for (int i = 0; i < SIZE; i++) {
        memset(layerBuffers[i], 0, sizeof(layerBuffers[i]));  // Clear each layer
    }
}

void shiftRainDown() {
    // Instead of shifting bit by bit, just shift whole layers in the buffer
    for (int z = SIZE - 1; z > 0; z--) {
        memcpy(layerBuffers[z], layerBuffers[z - 1], sizeof(layerBuffers[z]));
    }
    memset(layerBuffers[0], 0, sizeof(layerBuffers[0])); // Clear the top layer for new rain
}

void addNewRain() {
    for (int i = 0; i < 3; i++) {  // Add some random raindrops
        int x = random(0, SIZE);
        int y = random(0, SIZE);
        if (x != 15 && x != 16 && y != 15 && y != 16) { // Avoid center axis
          int bitIndex = y * SIZE + x;
          layerBuffers[0][bitIndex / 32] |= (1UL << (bitIndex % 32));
        }
    }
}

void handlePersistence() {
    // Handle persistence on the last level (layer 31)
    for (int y = 0; y < SIZE; y++) {
        int ySIZE = y * SIZE;
        for (int x = 0; x < SIZE; x++) {
            int bitIndex = ySIZE + x;
            
            if (layerBuffers[SIZE - 2][bitIndex / 32] & (1UL << (bitIndex % 32))) {
                // If there's a raindrop in layer 30, set persistence
                persistenceGrid[x][y] = PERSISTENCE_FRAMES;
            }

            if (persistenceGrid[x][y] > 0) {
                // Keep the raindrop persistent in layer 31
                layerBuffers[SIZE - 1][bitIndex / 32] |= (1UL << (bitIndex % 32));
                persistenceGrid[x][y]--;
            } else {
                // Clear the raindrop from layer 31
                layerBuffers[SIZE - 1][bitIndex / 32] &= ~(1UL << (bitIndex % 32));
            }
        }
    }
}

void applyLayersToVoxelGrid() {
    // Copy layer buffers into the voxel grid
    for (int z = 0; z < SIZE; z++) {
        memcpy(&voxelGrid[z * SIZE], layerBuffers[z], sizeof(layerBuffers[z]));
    }
}

void updateRain() {
    shiftRainDown();        // Shift all layers down
    addNewRain();           // Add new raindrops at the top
    handlePersistence();    // Handle persistence on the bottom layer (layer 31)
    applyLayersToVoxelGrid();  // Apply the updated layers to the main voxel grid
}

