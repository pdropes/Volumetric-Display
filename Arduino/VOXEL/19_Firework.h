//----------------------------------FIREWORKS

// Function to check if a point is within the circular boundary
bool isInCircle(int x, int y) {
    int dx = x - CENTER;
    int dy = y - CENTER;
    return (dx * dx + dy * dy) <= (RADIUS * RADIUS);
}

// Function to generate a random point within the circle
void getRandomPointInCircle(int &x, int &y) {
    float angle = random(0, 360) * (PI / 180.0);
    float distance = sqrt(random(0, 169));  // sqrt ensures uniform distribution within the circle (smaller radius 13 x 13)

    x = CENTER + distance * cos(angle);
    y = CENTER + distance * sin(angle);
}

// Function to initiate a firework launch
void launchFirework(int index) {
    Firework &fw = fireworks[index];
    fw.state = LAUNCHING;
    fw.currentZ = START_Z;
    fw.lastUpdate = millis();
    getRandomPointInCircle(fw.startX, fw.startY);
    fw.startZ = START_Z;
}

// Function to handle the explosion
void explodeFirework(int index) {
    Firework &fw = fireworks[index];
    fw.state = EXPLODING;
    fw.numSparks = random(20, MAX_SPARKS);  // Random number of sparks per explosion

    for (int i = 0; i < fw.numSparks; i++) {
        fw.sparks[i].x = fw.startX;
        fw.sparks[i].y = fw.startY;
        fw.sparks[i].z = SIZE_2;

        // Generate random angles for a random direction in 3D space
        float theta = random(0, 360) * (PI / 180.0);  // Random angle in XY plane
        float phi = random(0, 180) * (PI / 180.0);    // Random angle in Z direction

        // Convert spherical coordinates (r=1) to Cartesian coordinates
        fw.sparks[i].dx = cos(theta) * sin(phi);
        fw.sparks[i].dy = sin(theta) * sin(phi);
        fw.sparks[i].dz = cos(phi);

        fw.sparks[i].lifetime = SPARK_LIFETIME;
    }
}

// Function to update all fireworks
void updateFireworks() {
    for (int i = 0; i < MAX_FIREWORKS; i++) {
        Firework &fw = fireworks[i];

        if (fw.state == LAUNCHING) {
            if (millis() - fw.lastUpdate >= LAUNCH_INTERVAL) {
                // Set new position
                SET_BIT(voxelGrid, fw.startX, fw.startY, fw.currentZ);
                fw.currentZ--;
                fw.lastUpdate = millis();

                if (fw.currentZ < SIZE / 2) {
                    // Transition to explosion
                    explodeFirework(i);
                }
            }
        }
        else if (fw.state == EXPLODING) {
            int activeSparks = 0;

            for (int j = 0; j < fw.numSparks; j++) {
                Spark &spark = fw.sparks[j];
                if (spark.lifetime > 0) {
                    spark.x += spark.dx;
                    spark.y += spark.dy;
                    spark.z += spark.dz;

                    int ix = round(spark.x);
                    int iy = round(spark.y);
                    int iz = round(spark.z);

                    if (ix >= 0 && ix < SIZE &&
                        iy >= 0 && iy < SIZE &&
                        iz >= 0 && iz < SIZE &&
                        isInCircle(ix, iy)) {
                        SET_BIT(voxelGrid, ix, iy, iz);
                    }

                    spark.lifetime--;
                    if (spark.lifetime > 0) {
                        activeSparks++;
                    }
                }
            }

            if (activeSparks == 0) {
                fw.state = INACTIVE;  // Deactivate firework if no sparks are active
            }
        }
    }
}
