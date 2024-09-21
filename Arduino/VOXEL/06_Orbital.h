//----------------------------------3 Orbital Circles

// Function to apply a rotation to a point (x, y, z) around the X, Y, and Z axes
void rotatePoint(float &x, float &y, float &z, float sinX, float cosX, float sinY, float cosY, float sinZ, float cosZ) {
    // Rotate around X-axis
    float y1 = y * cosX - z * sinX;
    float z1 = y * sinX + z * cosX;
    
    // Rotate around Y-axis
    float x1 = x * cosY + z1 * sinY;
    float z2 = -x * sinY + z1 * cosY;
    
    // Rotate around Z-axis
    float x2 = x1 * cosZ - y1 * sinZ;
    float y2 = x1 * sinZ + y1 * cosZ;
    
    x = x2;
    y = y2;
    z = z2;
}

void drawCircle(int CircleRadius, int angleX, int angleY, int angleZ) {
    // Convert angles to radians
    float sinX = sinTable[angleX], cosX = cosTable[angleX];
    float sinY = sinTable[angleY], cosY = cosTable[angleY];
    float sinZ = sinTable[angleZ], cosZ = cosTable[angleZ];
    
    // Draw the circle in the XY plane
    for (int i = 0; i < 360; i += 6) {    //resolution 6=OK 4=TRACES
        float x = CircleRadius * cosTable[i];
        float y = CircleRadius * sinTable[i];
        float z = 0;  // Initially in the XY plane

        // Rotate the point around the three axes
        rotatePoint(x, y, z, sinX, cosX, sinY, cosY, sinZ, cosZ);

        // Convert to voxel space
        int voxelX = round(x) + center;
        int voxelY = round(y) + center;
        int voxelZ = round(z) + center;

        SET_BIT(voxelGrid, voxelX    , voxelY    , voxelZ    );
        SET_BIT(voxelGrid, voxelX + 1, voxelY + 1, voxelZ    ); //Some thickness
        SET_BIT(voxelGrid, voxelX - 1, voxelY - 1, voxelZ    );
        SET_BIT(voxelGrid, voxelX    , voxelY    , voxelZ + 1);
        SET_BIT(voxelGrid, voxelX    , voxelY    , voxelZ - 1);
    }
}

void updateOrbital() {
        angleXi = (angleXi + 1) % TABLE_SIZE;
        angleYi = (angleYi + 2) % TABLE_SIZE;
        angleZi = (angleZi + 3) % TABLE_SIZE;

        drawCircle(14, angleXi, angleYi, angleZi);  //PI / 4 = 45º
        drawCircle(12, angleYi, angleZi, angleXi);
        drawCircle(10, angleZi, angleYi, angleXi);
}












