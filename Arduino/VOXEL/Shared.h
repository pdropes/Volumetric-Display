//----------------------------------SHARED

#define ANGLES 100
#define SIZE 32
#define SIZE_2 16

// Center point (between LED 15 and LED 16)
const float center = 15.5;
const float center2 = center * center;

uint32_t voxelGrid[1024] = {0}; // 32x32x32 grid, each bit is a voxel
uint32_t (*animationFrames)[1024];  // Pointer to store the animation frames in PSRAM

// Define the matrix as an array of uint32_t, with each bit representing an LED
uint32_t Polar[SIZE][ANGLES];

uint8_t buf_brd_1[SIZE * 2];
uint8_t buf_brd_2[SIZE * 2];
uint8_t buff1[SIZE * 2];
uint8_t buff2[SIZE * 2];

//----------------------------------RAIN

uint8_t persistenceGrid[32][32] = {0}; // Counters for last level persistence 
uint32_t layerBuffers[32][32]; // Buffer for entire layers

//---------------------------------BOUNCING BALLS

#define FIELD_SIZE 30 //32 must be pair
#define BALL_RADIUS 5
#define BALL_DIAMETER (2 * BALL_RADIUS)
#define FIELD_RADIUS (FIELD_SIZE / 2)
#define NUM_BALLS 3

struct Ball {
    int8_t x, y, z;     // Ball position
    int8_t dx, dy, dz;  // Ball direction
};

Ball balls[NUM_BALLS];  // Array of balls

//----------------------------------ARROWS

// Structure to manage line states
struct Line {
  int fixed1, fixed2;  // Fixed coordinates (depends on the axis of movement)
  int moving;          // Current position (moving coordinate)
  int direction;       // Direction (-1 or 1)
  int length;          // Length of the line
  bool active;         // Is the line currently active
  int axis;            // 0 = move along X, 1 = move along Y, 2 = move along Z
};

const int maxLines = 20;    // Maximum number of simultaneous lines
Line lines[maxLines];       // Array to hold the state of each line
unsigned long lastUpdate = 0;
const int updateInterval = 8;  // Speed of movement (in milliseconds)

//----------------------------------TORNADO

#define TABLE_SIZE 360

#define NUM_PARTICLES 100  // Number of particles in the tornado
#define TORNADO_SIZE 32
#define TORNADO_SIZE_2 16
#define TORNADO_RADIUS 10  // Maximum radius of the tornado
#define TORNADO_HEIGHT TORNADO_SIZE  // Height of the tornado

struct Particle {
    float x, y, z;       // Position
    int angleIndex;      // Angle index for lookup table
    float height;        // Height of the particle
};

Particle particles[NUM_PARTICLES];  // Array of particles

//---

#define NUM_DEBRIS 30  // Number of debris particles
#define MAX_DEBRIS_RADIUS (TORNADO_SIZE / 2 - 2)  // Maximum radius for debris, leaving a margin at the boundary
#define MIN_SPEED 100   // Minimum speed for particles far from the tornado
#define MAX_SPEED 300   // Maximum speed for particles close to the tornado


struct Debris {
    float x, y, z;       // Position
    float initialRadius; // Initial radius from the tornado center
    int angleIndex;      // Angle index for lookup table
    float height;        // Fixed height
    float speed;
};

Debris debris[NUM_DEBRIS];  // Array of debris particles

//----------------------------------VOXEL / SPHERE / TORUS

float angleXf = 0.0;
float angleYf = 0.0;
float angleZf = 0.0;

int angleXi = 0;
int angleYi = 0;
int angleZi = 0;

//----------------------------------SOLAR SYSTEM

// Tilt angles for 3D orbits (in degrees)
float tiltEarth = 23.5;  // Earth's orbital tilt
float tiltMoon = 5.1;    // Moon's orbital tilt

float thetaEarth = 0;
float thetaMoon = 0;
float speedEarth = 1;  // Degrees per cycle
float speedMoon = 5;   // Faster orbit around Earth


#define MAX_FLAMES 20  // Maximum number of sparks at any time

struct Flame {
    int x, y, z;
    int life;  // Number of frames the spark will last
    int dx, dy, dz;  // Direction of movement
};

Flame flames[MAX_FLAMES];

//----------------------------------PLANETARY GEARBOXES

#define NUM_FRAMES 360

//----------------------------------DNA

static float rotationAngle = 0;
uint8_t traceGrid[SIZE][SIZE][SIZE];  // Stores the lifetime of trace bits

//----------------------------------SNAKE CUBE

// Define constants for the maximum number of cubes
#define MAX_CUBES 5

// Struct to hold cube properties
struct Vertex {
    int x, y, z;
};

struct Cube {
    int currentPhase;
    float edgeProgress;
    bool erasing;
    Vertex vertices[8];   // 8 vertices for each cube
};

//----------------------------------SNAKES

#define MAX_SNAKES 10
#define SNAKE_LENGTH 15  // Length of each snake
#define DIRECTIONS 6      // Number of possible directions (up, down, left, right, forward, backward)

// Struct to define each snake
struct Snake {
    Vertex body[SNAKE_LENGTH];  // Body segments of the snake
    int headDirection;          // Current direction of movement
};

//----------------------------------BUBBLES

struct Bubble {
  int x, y, z;      // Center position of the bubble
  float zPosition;  // Store the Z position as a float for smooth movement
  int radius;       // Radius of the bubble
  float speed;      // Speed at which the bubble rises
  bool active;      // Whether the bubble is currently visible
};

const int MAX_BUBBLES = 50;  // Maximum number of bubbles at a time
Bubble bubbles[MAX_BUBBLES];

//----------------------------------FIREWORKS

#define GRID_LAYER_SIZE (SIZE * SIZE)
#define TOTAL_VOXELS (SIZE * SIZE * SIZE)
#define MAX_SPARKS 50           // Maximum number of sparks per explosion
#define SPARK_LIFETIME 20       // How many frames each spark lasts
#define MAX_FIREWORKS 5         // Maximum number of simultaneous fireworks
#define LAUNCH_INTERVAL 10      // Time between each firework launch step (in ms)
#define RADIUS (SIZE / 2)
#define CENTER (SIZE / 2)
#define START_Z 31


// Spark structure to hold the properties of each spark
struct Spark {
    float x, y, z;               // Current position of the spark
    float dx, dy, dz;            // Direction of movement
    int lifetime;                // Frames remaining before the spark disappears
};

// Firework states
enum FireworkState { INACTIVE, LAUNCHING, EXPLODING };

struct Firework {
    Spark sparks[MAX_SPARKS];    // Array to hold all sparks for this firework
    int numSparks;               // Current number of active sparks
    FireworkState state;         // State of the firework (launching, exploding)
    int startX, startY, startZ;  // Launch position
    int currentZ;                // Current Z position during launch
    unsigned long lastUpdate;    // Last update time for non-blocking delay
};

Firework fireworks[MAX_FIREWORKS];  // Array to hold multiple fireworks

unsigned long lastLaunchTime = 0;  // Track the last time a firework was launched
unsigned long launchInterval;      // Random interval between launches

//----------------------------------
//----------------------------------

#define GET_BIT(arr, x, y, z) ((arr[(z * 32 * 32 + y * 32 + x) / 32] >> (x % 32)) & 1UL)

#define SAFE_INDEX(x, y, z) (((z) * 32 * 32 + (y) * 32 + (x)) / 32)

#define SET_BIT(arr, x, y, z) do { \
    int idxX = (x) >= 0 ? (x) : 0; \
    int idxY = (y) >= 0 ? (y) : 0; \
    int idxZ = (z) >= 0 ? (z) : 0; \
    arr[SAFE_INDEX(idxX, idxY, idxZ)] |= (1UL << (idxX % 32)); \
} while (0)

#define CLEAR_BIT(arr, x, y, z) do { \
    int idxX = (x) >= 0 ? (x) : 0; \
    int idxY = (y) >= 0 ? (y) : 0; \
    int idxZ = (z) >= 0 ? (z) : 0; \
    arr[SAFE_INDEX(idxX, idxY, idxZ)] &= ~(1UL << (idxX % 32)); \
} while (0)

//----------------------------------Function to draw a line using Bresenham's 3D algorithm
void drawLine3D(uint32_t *arr, int x0, int y0, int z0, int x1, int y1, int z1) {
    int dx = abs(x1 - x0), dy = abs(y1 - y0), dz = abs(z1 - z0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int sz = (z0 < z1) ? 1 : -1;
    
    int err1, err2;
    
    if (dx >= dy && dx >= dz) { // X dominant
        err1 = 2 * dy - dx;
        err2 = 2 * dz - dx;
        for (; x0 != x1; x0 += sx) {
            SET_BIT(arr, x0, y0, z0);
            if (err1 > 0) { y0 += sy; err1 -= 2 * dx; }
            if (err2 > 0) { z0 += sz; err2 -= 2 * dx; }
            err1 += 2 * dy;
            err2 += 2 * dz;
        }
    } else if (dy >= dz) { // Y dominant
        err1 = 2 * dx - dy;
        err2 = 2 * dz - dy;
        for (; y0 != y1; y0 += sy) {
            SET_BIT(arr, x0, y0, z0);
            if (err1 > 0) { x0 += sx; err1 -= 2 * dy; }
            if (err2 > 0) { z0 += sz; err2 -= 2 * dy; }
            err1 += 2 * dx;
            err2 += 2 * dz;
        }
    } else { // Z dominant
        err1 = 2 * dy - dz;
        err2 = 2 * dx - dz;
        for (; z0 != z1; z0 += sz) {
            SET_BIT(arr, x0, y0, z0);
            if (err1 > 0) { y0 += sy; err1 -= 2 * dz; }
            if (err2 > 0) { x0 += sx; err2 -= 2 * dz; }
            err1 += 2 * dy;
            err2 += 2 * dx;
        }
    }
    SET_BIT(arr, x1, y1, z1); // Ensure the endpoint is set
}

//----------------------------------Function to reverse a byte order
unsigned char reverse(unsigned char a)
{
  return ((a & 0x1)  << 7) | ((a & 0x2)  << 5) |
         ((a & 0x4)  << 3) | ((a & 0x8)  << 1) |
         ((a & 0x10) >> 1) | ((a & 0x20) >> 3) |
         ((a & 0x40) >> 5) | ((a & 0x80) >> 7);
}

//----------------------------------SIN & COS TABLES for animations
const float sinTable[360] PROGMEM = {
 0.0000,  0.0175,  0.0349,  0.0523,  0.0698,  0.0872,  0.1045,  0.1219,  0.1392,  0.1564, 
 0.1736,  0.1908,  0.2079,  0.2250,  0.2419,  0.2588,  0.2756,  0.2924,  0.3090,  0.3256, 
 0.3420,  0.3584,  0.3746,  0.3907,  0.4067,  0.4226,  0.4384,  0.4540,  0.4695,  0.4848, 
 0.5000,  0.5150,  0.5299,  0.5446,  0.5592,  0.5736,  0.5878,  0.6018,  0.6157,  0.6293, 
 0.6428,  0.6561,  0.6691,  0.6820,  0.6947,  0.7071,  0.7193,  0.7314,  0.7431,  0.7547, 
 0.7660,  0.7771,  0.7880,  0.7986,  0.8090,  0.8192,  0.8290,  0.8387,  0.8480,  0.8572, 
 0.8660,  0.8746,  0.8829,  0.8910,  0.8988,  0.9063,  0.9135,  0.9205,  0.9272,  0.9336, 
 0.9397,  0.9455,  0.9511,  0.9563,  0.9613,  0.9659,  0.9703,  0.9744,  0.9781,  0.9816, 
 0.9848,  0.9877,  0.9903,  0.9925,  0.9945,  0.9962,  0.9976,  0.9986,  0.9994,  0.9998, 
 1.0000,  0.9998,  0.9994,  0.9986,  0.9976,  0.9962,  0.9945,  0.9925,  0.9903,  0.9877, 
 0.9848,  0.9816,  0.9781,  0.9744,  0.9703,  0.9659,  0.9613,  0.9563,  0.9511,  0.9455, 
 0.9397,  0.9336,  0.9272,  0.9205,  0.9135,  0.9063,  0.8988,  0.8910,  0.8829,  0.8746, 
 0.8660,  0.8572,  0.8480,  0.8387,  0.8290,  0.8192,  0.8090,  0.7986,  0.7880,  0.7771, 
 0.7660,  0.7547,  0.7431,  0.7314,  0.7193,  0.7071,  0.6947,  0.6820,  0.6691,  0.6561, 
 0.6428,  0.6293,  0.6157,  0.6018,  0.5878,  0.5736,  0.5592,  0.5446,  0.5299,  0.5150, 
 0.5000,  0.4848,  0.4695,  0.4540,  0.4384,  0.4226,  0.4067,  0.3907,  0.3746,  0.3584, 
 0.3420,  0.3256,  0.3090,  0.2924,  0.2756,  0.2588,  0.2419,  0.2250,  0.2079,  0.1908, 
 0.1736,  0.1564,  0.1392,  0.1219,  0.1045,  0.0872,  0.0698,  0.0523,  0.0349,  0.0175, 
-0.0000, -0.0175, -0.0349, -0.0523, -0.0698, -0.0872, -0.1045, -0.1219, -0.1392, -0.1564, 
-0.1736, -0.1908, -0.2079, -0.2250, -0.2419, -0.2588, -0.2756, -0.2924, -0.3090, -0.3256, 
-0.3420, -0.3584, -0.3746, -0.3907, -0.4067, -0.4226, -0.4384, -0.4540, -0.4695, -0.4848, 
-0.5000, -0.5150, -0.5299, -0.5446, -0.5592, -0.5736, -0.5878, -0.6018, -0.6157, -0.6293, 
-0.6428, -0.6561, -0.6691, -0.6820, -0.6947, -0.7071, -0.7193, -0.7314, -0.7431, -0.7547, 
-0.7660, -0.7771, -0.7880, -0.7986, -0.8090, -0.8192, -0.8290, -0.8387, -0.8480, -0.8572, 
-0.8660, -0.8746, -0.8829, -0.8910, -0.8988, -0.9063, -0.9135, -0.9205, -0.9272, -0.9336, 
-0.9397, -0.9455, -0.9511, -0.9563, -0.9613, -0.9659, -0.9703, -0.9744, -0.9781, -0.9816, 
-0.9848, -0.9877, -0.9903, -0.9925, -0.9945, -0.9962, -0.9976, -0.9986, -0.9994, -0.9998, 
-1.0000, -0.9998, -0.9994, -0.9986, -0.9976, -0.9962, -0.9945, -0.9925, -0.9903, -0.9877, 
-0.9848, -0.9816, -0.9781, -0.9744, -0.9703, -0.9659, -0.9613, -0.9563, -0.9511, -0.9455, 
-0.9397, -0.9336, -0.9272, -0.9205, -0.9135, -0.9063, -0.8988, -0.8910, -0.8829, -0.8746, 
-0.8660, -0.8572, -0.8480, -0.8387, -0.8290, -0.8192, -0.8090, -0.7986, -0.7880, -0.7771, 
-0.7660, -0.7547, -0.7431, -0.7314, -0.7193, -0.7071, -0.6947, -0.6820, -0.6691, -0.6561, 
-0.6428, -0.6293, -0.6157, -0.6018, -0.5878, -0.5736, -0.5592, -0.5446, -0.5299, -0.5150, 
-0.5000, -0.4848, -0.4695, -0.4540, -0.4384, -0.4226, -0.4067, -0.3907, -0.3746, -0.3584, 
-0.3420, -0.3256, -0.3090, -0.2924, -0.2756, -0.2588, -0.2419, -0.2250, -0.2079, -0.1908, 
-0.1736, -0.1564, -0.1392, -0.1219, -0.1045, -0.0872, -0.0698, -0.0523, -0.0349, -0.0175
};

const float cosTable[360] PROGMEM = {
 1.0000,  0.9998,  0.9994,  0.9986,  0.9976,  0.9962,  0.9945,  0.9925,  0.9903,  0.9877, 
 0.9848,  0.9816,  0.9781,  0.9744,  0.9703,  0.9659,  0.9613,  0.9563,  0.9511,  0.9455, 
 0.9397,  0.9336,  0.9272,  0.9205,  0.9135,  0.9063,  0.8988,  0.8910,  0.8829,  0.8746, 
 0.8660,  0.8572,  0.8480,  0.8387,  0.8290,  0.8192,  0.8090,  0.7986,  0.7880,  0.7771, 
 0.7660,  0.7547,  0.7431,  0.7314,  0.7193,  0.7071,  0.6947,  0.6820,  0.6691,  0.6561, 
 0.6428,  0.6293,  0.6157,  0.6018,  0.5878,  0.5736,  0.5592,  0.5446,  0.5299,  0.5150, 
 0.5000,  0.4848,  0.4695,  0.4540,  0.4384,  0.4226,  0.4067,  0.3907,  0.3746,  0.3584, 
 0.3420,  0.3256,  0.3090,  0.2924,  0.2756,  0.2588,  0.2419,  0.2250,  0.2079,  0.1908, 
 0.1736,  0.1564,  0.1392,  0.1219,  0.1045,  0.0872,  0.0698,  0.0523,  0.0349,  0.0175, 
-0.0000, -0.0175, -0.0349, -0.0523, -0.0698, -0.0872, -0.1045, -0.1219, -0.1392, -0.1564, 
-0.1736, -0.1908, -0.2079, -0.2250, -0.2419, -0.2588, -0.2756, -0.2924, -0.3090, -0.3256, 
-0.3420, -0.3584, -0.3746, -0.3907, -0.4067, -0.4226, -0.4384, -0.4540, -0.4695, -0.4848, 
-0.5000, -0.5150, -0.5299, -0.5446, -0.5592, -0.5736, -0.5878, -0.6018, -0.6157, -0.6293, 
-0.6428, -0.6561, -0.6691, -0.6820, -0.6947, -0.7071, -0.7193, -0.7314, -0.7431, -0.7547, 
-0.7660, -0.7771, -0.7880, -0.7986, -0.8090, -0.8192, -0.8290, -0.8387, -0.8480, -0.8572, 
-0.8660, -0.8746, -0.8829, -0.8910, -0.8988, -0.9063, -0.9135, -0.9205, -0.9272, -0.9336, 
-0.9397, -0.9455, -0.9511, -0.9563, -0.9613, -0.9659, -0.9703, -0.9744, -0.9781, -0.9816, 
-0.9848, -0.9877, -0.9903, -0.9925, -0.9945, -0.9962, -0.9976, -0.9986, -0.9994, -0.9998, 
-1.0000, -0.9998, -0.9994, -0.9986, -0.9976, -0.9962, -0.9945, -0.9925, -0.9903, -0.9877, 
-0.9848, -0.9816, -0.9781, -0.9744, -0.9703, -0.9659, -0.9613, -0.9563, -0.9511, -0.9455, 
-0.9397, -0.9336, -0.9272, -0.9205, -0.9135, -0.9063, -0.8988, -0.8910, -0.8829, -0.8746, 
-0.8660, -0.8572, -0.8480, -0.8387, -0.8290, -0.8192, -0.8090, -0.7986, -0.7880, -0.7771, 
-0.7660, -0.7547, -0.7431, -0.7314, -0.7193, -0.7071, -0.6947, -0.6820, -0.6691, -0.6561, 
-0.6428, -0.6293, -0.6157, -0.6018, -0.5878, -0.5736, -0.5592, -0.5446, -0.5299, -0.5150, 
-0.5000, -0.4848, -0.4695, -0.4540, -0.4384, -0.4226, -0.4067, -0.3907, -0.3746, -0.3584, 
-0.3420, -0.3256, -0.3090, -0.2924, -0.2756, -0.2588, -0.2419, -0.2250, -0.2079, -0.1908, 
-0.1736, -0.1564, -0.1392, -0.1219, -0.1045, -0.0872, -0.0698, -0.0523, -0.0349, -0.0175, 
 0.0000,  0.0175,  0.0349,  0.0523,  0.0698,  0.0872,  0.1045,  0.1219,  0.1392,  0.1564, 
 0.1736,  0.1908,  0.2079,  0.2250,  0.2419,  0.2588,  0.2756,  0.2924,  0.3090,  0.3256, 
 0.3420,  0.3584,  0.3746,  0.3907,  0.4067,  0.4226,  0.4384,  0.4540,  0.4695,  0.4848, 
 0.5000,  0.5150,  0.5299,  0.5446,  0.5592,  0.5736,  0.5878,  0.6018,  0.6157,  0.6293, 
 0.6428,  0.6561,  0.6691,  0.6820,  0.6947,  0.7071,  0.7193,  0.7314,  0.7431,  0.7547, 
 0.7660,  0.7771,  0.7880,  0.7986,  0.8090,  0.8192,  0.8290,  0.8387,  0.8480,  0.8572, 
 0.8660,  0.8746,  0.8829,  0.8910,  0.8988,  0.9063,  0.9135,  0.9205,  0.9272,  0.9336, 
 0.9397,  0.9455,  0.9511,  0.9563,  0.9613,  0.9659,  0.9703,  0.9744,  0.9781,  0.9816, 
 0.9848,  0.9877,  0.9903,  0.9925,  0.9945,  0.9962,  0.9976,  0.9986,  0.9994,  0.9998
};

//----------------------------------SIN & COS TABLES for hardware
const float sinT[100] PROGMEM = {
 0.0000,  0.0628,  0.1253,  0.1874,  0.2487,  0.3090,  0.3681,  0.4258,  0.4818,  0.5358, 
 0.5878,  0.6374,  0.6845,  0.7290,  0.7705,  0.8090,  0.8443,  0.8763,  0.9048,  0.9298, 
 0.9511,  0.9686,  0.9823,  0.9921,  0.9980,  1.0000,  0.9980,  0.9921,  0.9823,  0.9686, 
 0.9511,  0.9298,  0.9048,  0.8763,  0.8443,  0.8090,  0.7705,  0.7290,  0.6845,  0.6374, 
 0.5878,  0.5358,  0.4818,  0.4258,  0.3681,  0.3090,  0.2487,  0.1874,  0.1253,  0.0628, 
-0.0000, -0.0628, -0.1253, -0.1874, -0.2487, -0.3090, -0.3681, -0.4258, -0.4818, -0.5358, 
-0.5878, -0.6374, -0.6845, -0.7290, -0.7705, -0.8090, -0.8443, -0.8763, -0.9048, -0.9298, 
-0.9511, -0.9686, -0.9823, -0.9921, -0.9980, -1.0000, -0.9980, -0.9921, -0.9823, -0.9686, 
-0.9511, -0.9298, -0.9048, -0.8763, -0.8443, -0.8090, -0.7705, -0.7290, -0.6845, -0.6374, 
-0.5878, -0.5358, -0.4818, -0.4258, -0.3681, -0.3090, -0.2487, -0.1874, -0.1253, -0.0628, 
};

const float cosT[100] PROGMEM = {
 1.0000,  0.9980,  0.9921,  0.9823,  0.9686,  0.9511,  0.9298,  0.9048,  0.8763,  0.8443, 
 0.8090,  0.7705,  0.7290,  0.6845,  0.6374,  0.5878,  0.5358,  0.4818,  0.4258,  0.3681, 
 0.3090,  0.2487,  0.1874,  0.1253,  0.0628, -0.0000, -0.0628, -0.1253, -0.1874, -0.2487, 
-0.3090, -0.3681, -0.4258, -0.4818, -0.5358, -0.5878, -0.6374, -0.6845, -0.7290, -0.7705, 
-0.8090, -0.8443, -0.8763, -0.9048, -0.9298, -0.9511, -0.9686, -0.9823, -0.9921, -0.9980, 
-1.0000, -0.9980, -0.9921, -0.9823, -0.9686, -0.9511, -0.9298, -0.9048, -0.8763, -0.8443, 
-0.8090, -0.7705, -0.7290, -0.6845, -0.6374, -0.5878, -0.5358, -0.4818, -0.4258, -0.3681, 
-0.3090, -0.2487, -0.1874, -0.1253, -0.0628,  0.0000,  0.0628,  0.1253,  0.1874,  0.2487, 
 0.3090,  0.3681,  0.4258,  0.4818,  0.5358,  0.5878,  0.6374,  0.6845,  0.7290,  0.7705, 
 0.8090,  0.8443,  0.8763,  0.9048,  0.9298,  0.9511,  0.9686,  0.9823,  0.9921,  0.9980, 
};




