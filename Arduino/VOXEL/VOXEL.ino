///////////////////////////////
// V O X E L by #dropes 2024 //
///////////////////////////////

//wifi
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
//spi
#include <SPI.h>
//cube
#include <Arduino.h>
//#include <math.h>

//FX
#include "Shared.h"
#include "00_CubeStatic.h" 
#include "01_SolarSystem.h"
#include "02_Sphere.h"     
#include "03_Cube3axis.h"  
#include "04_Torus.h"      
#include "05_SineX2.h"     
#include "06_Orbital.h"    
#include "07_Rain.h"       
#include "08_Star.h"       
#include "09_Bouncing.h"   
#include "10_Arrows.h"     
#include "11_Tornado.h"  
#include "12_PlanetaryGearbox.h"  
#include "13_DNA.h"
#include "14_Spirals.h"
#include "15_SnakeCube.h"
#include "16_Snakes.h"
#include "17_Bubbles.h"
#include "18_Tetrahedrons.h"
#include "19_Firework.h"  

#define FXlast 19

#define MATRIX_WIDTH 32
#define MATRIX_HEIGHT 32
#define ANGLES 100

#define HSPI_MISO 37
#define HSPI_MOSI 35
#define HSPI_SCLK 36
#define HSPI_CS 34
#define LE1 41
#define LE2 40
#define IR_PIN 1


//15000000;  //TLC59283: Data transfer rate: 35MHz "Ghosts :(" MAX=17MHz
static const int spiClk = 15000000;  
SPIClass* hspi = NULL;

// Timer0 Configuration Pointer (Handle)
hw_timer_t* timer = NULL;

//Enter your sensitive data to connect to Wi-Fi
const char* ssid = "..."; // your network SSID (name) */
const char* password = "..."; // your network password (use for WPA, or use as key for WEP) */

uint8_t currentAngle, newAngle;
int hys = 3000;

uint8_t blank[MATRIX_HEIGHT * 2];
uint32_t buf_matrix[MATRIX_HEIGHT];


unsigned long lastIRTrigger = 0;
unsigned long rotationTime = 0;  // Time for one rotation in milliseconds
unsigned long currentState;
unsigned long timePerSegment;

float rotate = 0.0;
int deg = 0;
int deg_old = 0;
float deg_inc = 0.0;
uint8_t FX = 0;
int FX_count = 0;
uint8_t oldFX = 0;
int frame = 0; //for animations

uint8_t seq_yx[16][4] = {
  { 48, 47, 16, 15 },
  { 49, 46, 17, 14 },
  { 50, 45, 18, 13 },
  { 51, 44, 19, 12 },
  { 52, 43, 20, 11 },
  { 53, 42, 21, 10 },
  { 54, 41, 22, 9 },
  { 55, 40, 23, 8 },
  { 56, 39, 24, 7 },
  { 57, 38, 25, 6 },
  { 58, 37, 26, 5 },
  { 59, 36, 27, 4 },
  { 60, 35, 28, 3 },
  { 61, 34, 29, 2 },
  { 62, 33, 30, 1 },
  { 63, 32, 31, 0 }
};


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//----------------------------------Rectangular to polar conversion
void rec2pol(uint32_t *arr, int angle) {

 // 100x faster when assigning just the angle to be displayed
 // for (int angle = 0; angle < angles; angle++) { 

    // negative angle for the 2nd board
    int Iangle;
    if (angle < 49) //*50
      Iangle = angle + 47; //*47 esq/dto
    else
      Iangle = angle + 46; //*46 frt/back
    if (Iangle > 99) 
      Iangle -= 100; 
    //float Itheta = (2 * PI * Iangle) / ANGLES;

    int IangleAdd = Iangle + 3;  //*3 
    if (IangleAdd > 99)
      IangleAdd -= 100;

    for (int r = 0; r < 16; r++) {
      int x1 = round(center + r * cosT[angle]);
      int y1 = round(center + r * sinT[angle]);
      int x2 = round(center + r * cosT[Iangle]);
      int y2 = round(center + r * sinT[Iangle]);

      for (int z = 0; z < MATRIX_HEIGHT; z++) {
        //1st board
        if (GET_BIT(arr, x1, y1, z)) {
          Polar[z][angle] |= (1UL << (r + 16));
        }
        if (GET_BIT(arr, x2, y2, z)) {
          Polar[z][angle] |= (1UL << (15 - r));
        }
        z++;
        //2nd board
        if (GET_BIT(arr, x1, y1, z)) {
          Polar[z][IangleAdd] |= (1UL << (r + 16));
        }
        if (GET_BIT(arr, x2, y2, z)) {
          Polar[z][IangleAdd] |= (1UL << (15 - r));
        }
      }
    }
 // }
}

//----------------------------------Convert the matrix for both boards
void matrix2brd() {  // This procedure must be quick
  uint8_t value_1;
  uint8_t value_2;
  uint8_t xy_loc;

  uint32_t value_up;
  uint32_t value_down;

  // captures the image from the buffer matrix
  for (uint8_t y = 0; y < 32; y += 2) {
    value_up = buf_matrix[y];
    value_down = buf_matrix[y + 1];
    for (int x = 3; x > -1; x--) {
      value_1 = value_up & 0XFF;
      value_2 = value_down & 0XFF;

      // places the byte in the final position and orientation to be displayed.
      xy_loc = seq_yx[y >> 1][x];

      if (y % 4 == 0) {  //y= 0 4 8 12
        buf_brd_1[xy_loc] = reverse(value_1);
        buf_brd_2[xy_loc] = reverse(value_2);
      } else {
        buf_brd_1[xy_loc] = value_1;
        buf_brd_2[xy_loc] = value_2;
      }
      value_up >>= 8;
      value_down >>= 8;
    }
  }
}

//----------------------------------Clear LEDs
void clearLEDs() {
  digitalWrite(LE1, LOW);
  digitalWrite(LE2, LOW);
  hspi->writeBytes(blank, 64);
  digitalWrite(LE1, HIGH);
  digitalWrite(LE2, HIGH);
  digitalWrite(LE1, LOW);
  digitalWrite(LE2, LOW);
}

//----------------------------------Rectangular 32x32 - Polar 32x32/ang - Matrix 32x32- Board 2x32x16
void rec2brd(uint8_t angleSet) {
  rec2pol(voxelGrid, angleSet);
  for (uint8_t z = 0; z < MATRIX_HEIGHT; z++) {
    buf_matrix[z] = Polar[z][angleSet];
  }
  matrix2brd();
}

//----------------------------------Interrupt routine for NEW IMAGE
void ARDUINO_ISR_ATTR Tm() {

  switch (FX) {
    case 0:  //CubeStatic rotates on Z
      if (rotate < 100.0) rotate += 0.01;
      else rotate = 0;
      newAngle = currentAngle + round(rotate);
      if (newAngle > 99) newAngle -= 100;
      rec2brd(newAngle);
      break;

    case 1:  //Solar System
      rec2brd(currentAngle);
      break;

    case 2:  //Sphere
      rec2brd(currentAngle);
      if (currentAngle == 99) clearLEDs();
      break;

    case 3:  //Cube3axi
      rec2brd(currentAngle);
      break;

    case 4:  //Torus
      rec2brd(currentAngle);
      if (currentAngle == 99) clearLEDs();
      break;

    case 5:  //SineX2
      if (deg_inc < 359) deg_inc += 0.03;
      else deg_inc = 0;
      deg = (int)deg_inc;
      if (deg != deg_old) showSine(deg);
      if (currentAngle % 4 == 0) {
        memcpy(buf_brd_1, buff1, sizeof(buff1));
        memcpy(buf_brd_2, buff2, sizeof(buff2));
      } else {
        memset(buf_brd_1, 0, sizeof(buf_brd_1));
        memset(buf_brd_2, 0, sizeof(buf_brd_2));
      }
      deg_old = deg;
      break;

    case 6:  //Orbital
      rec2brd(currentAngle);
      if (currentAngle == 99) clearLEDs();
      break;

    case 7:  //Rain
      rec2brd(currentAngle);
      break;

    case 8:  //Star
      //Rotate the Star
      if (rotate < 100.0) rotate += 0.01;
      else rotate = 0;
      newAngle = currentAngle + round(rotate);
      if (newAngle > 99) newAngle -= 100;
      rec2pol(voxelGrid, newAngle);
      for (uint8_t z = 0; z < MATRIX_HEIGHT; z++) {
        buf_matrix[z] = Polar[z][newAngle];
        if (random(50) == 1) {
          buf_matrix[z] |= (1 << (micros() & 0b11111));  //sparks :)
        }
      }
      matrix2brd();
      break;

    case 9 ... 19:  //Bouncing Ball, Arrows, Tornado, Planetary Gearbox, DNA, Spirals, SnakeCube, Snakes, Bubbles, Tetrahedrons, Firework
        rec2brd(currentAngle);
        break;
  }

  digitalWrite(LE1, LOW);
  hspi->writeBytes(buf_brd_1, 64);
  digitalWrite(LE1, HIGH);
  digitalWrite(LE1, LOW);

  digitalWrite(LE2, LOW);
  hspi->writeBytes(buf_brd_2, 64);
  digitalWrite(LE2, HIGH);
  digitalWrite(LE2, LOW);

  if (currentAngle == 99) {
    timerStop(timer);
  } else currentAngle++;
}


//----------------------------------IR interrupt routine for Segment Time
void ISR_IR() {
  unsigned long currentTime = micros();
  // Check if HYS ms have passed since the last trigger
  if (currentTime - lastIRTrigger >= hys) {
    timePerSegment = round((currentTime - lastIRTrigger) / 100.5);
    lastIRTrigger = currentTime;

    currentAngle = 0;
    timerStop(timer);  //it should also stop at angle 99 (time counter reset)
    timerAlarm(timer, timePerSegment, true, 0);
    timerStart(timer);

    switch (FX) {
      case 0:  // CubeStatic rotates on Z
        drawCube();
        oldFX = FX;
        break;

      case 1:  //Solar System
        memset(voxelGrid, 0, sizeof(voxelGrid));
        memset(Polar, 0, sizeof(Polar));
        drawSolarSystem3D();
        oldFX = FX;
        break;

      case 2:  // Sphere
        if (oldFX != FX) {
          initSphere();
        }
        memset(voxelGrid, 0, sizeof(voxelGrid));
        memset(Polar, 0, sizeof(Polar));
        updateSphere();
        oldFX = FX;
        break;

      case 3:  //Cube3axi
        if (oldFX != FX) {
          angleXf = 0.0;
          angleYf = 0.0;
          angleZf = 0.0;
        }
        updateRotatedCube();
        oldFX = FX;
        break;

      case 4:  //Torus rotates on 3 axes
        if (oldFX != FX) {
          angleXi = 0;
          angleYi = 0;
          angleZi = 0;
        }
        memset(voxelGrid, 0, sizeof(voxelGrid));
        memset(Polar, 0, sizeof(Polar));
        clearLEDs();
        updateTorus();
        oldFX = FX;
        break;

      case 5:  //SineX2
        oldFX = FX;
        break;

      case 6:  //Orbital
        if (oldFX != FX) {
          clearLEDs();  //Removes traces
        }
        // Clear previous drawings
        memset(voxelGrid, 0, sizeof(voxelGrid));
        memset(Polar, 0, sizeof(Polar));
        updateOrbital();
        oldFX = FX;
        break;

      case 7:  //Rain
        if (oldFX != FX) {
          memset(voxelGrid, 0, sizeof(voxelGrid));
        }
        memset(Polar, 0, sizeof(Polar));
        updateRain();  // Update the rain pattern
        oldFX = FX;
        break;

      case 8:  //Star
        if (oldFX != FX) {
          // Clear previous drawings
          memset(voxelGrid, 0, sizeof(voxelGrid));
          memset(Polar, 0, sizeof(Polar));
          drawStarOfDavid(voxelGrid);
        }
        oldFX = FX;
        break;

      case 9:  //Bouncing Ball
        if (oldFX != FX) {
          initBalls();
          frame = 0;
        }
        if (frame == 0) {
          memset(voxelGrid, 0, sizeof(voxelGrid));
          memset(Polar, 0, sizeof(Polar));
          updateBalls();
          frame = 1;
        } else frame = 0;
        oldFX = FX;
        break;

      case 10:  //Arrows
        if (oldFX != FX) {
          memset(voxelGrid, 0, sizeof(voxelGrid));
          // Initialize all lines as inactive at start
          for (int i = 0; i < maxLines; i++) {
            lines[i].active = false;
          }
        }
        memset(Polar, 0, sizeof(Polar));
        if (millis() - lastUpdate > updateInterval) {
          lastUpdate = millis();
          updateArrows();
        }
        oldFX = FX;
        break;

      case 11:  //Tornado
        if (oldFX != FX) {
          initializeParticles();
          initializeDebris();  // Initialize debris
        }
        memset(voxelGrid, 0, sizeof(voxelGrid));
        memset(Polar, 0, sizeof(Polar));
        updateTornado();
        oldFX = FX;
        break;

      case 12:  //Planetary Gearbox
        if (oldFX != FX) frame = 0;
        memset(Polar, 0, sizeof(Polar));
        frame = (frame + 1) % NUM_FRAMES;
        memcpy(voxelGrid, animationFrames[frame], sizeof(voxelGrid));
        oldFX = FX;
        break;

      case 13:  //DNA
        memset(voxelGrid, 0, sizeof(voxelGrid));
        memset(Polar, 0, sizeof(Polar));
        updateDNA();
        oldFX = FX;
        break;

      case 14:  //Spirals
        if (oldFX != FX) deg = 0;
        memset(voxelGrid, 0, sizeof(voxelGrid));
        memset(Polar, 0, sizeof(Polar));    
        drawSpirals(deg);
        deg = (deg + 3) % 360;  // Rotate over time
        oldFX = FX;
        break;

      case 15:  //SnakeCube
        if (oldFX != FX) {
          initSnakeCube();
        }
        memset(voxelGrid, 0, sizeof(voxelGrid));
        memset(Polar, 0, sizeof(Polar));
        updateCubeEdges();
        oldFX = FX;
        break;

      case 16:  //Snakes
        if (oldFX != FX) {  // Initialize all snakes
          for (int i = 0; i < MAX_SNAKES; i++) {
            initializeSnake(snakes[i]);
          }
        }
        memset(voxelGrid, 0, sizeof(voxelGrid));
        memset(Polar, 0, sizeof(Polar));
        updateSnakes();
        oldFX = FX;
        break;
      
      case 17: //Bubbles
        if (FX != oldFX) {
          initializeBubbles();
        }  
        memset(voxelGrid, 0, sizeof(voxelGrid));
        memset(Polar, 0, sizeof(Polar));
        updateBubbles();
        oldFX = FX;
        break;

      case 18: //Tetrahedrons
        memset(voxelGrid, 0, sizeof(voxelGrid));
        memset(Polar, 0, sizeof(Polar));
        drawTetra();
        oldFX = FX;
        break;
        
      case 19:  //Firework (This must be the LAST effect)
        if (oldFX != FX) {
          launchInterval = random(300, 1000);  // Set initial random launch interval original=500 / 2000
        }
        memset(voxelGrid, 0, sizeof(voxelGrid));
        memset(Polar, 0, sizeof(Polar));
        unsigned long currTime = millis();  //"currentTime" variable is already used (micros);
        // Check if it's time to launch the next firework
        if (currTime - lastLaunchTime >= launchInterval) {
          for (int i = 0; i < MAX_FIREWORKS; i++) {
            if (fireworks[i].state == INACTIVE) {
              launchFirework(i);                  // Launch a new firework
              lastLaunchTime = currTime;          // Reset the launch time
              launchInterval = random(200, 600);  // Set the next random interval
              i = MAX_FIREWORKS;                  // Launch one firework at a time
            }
          }
        }
        updateFireworks();  // Update all fireworks
        oldFX = FX;
        break;
    }
  }

  //----------------

  if (FX_count == 500) { // Time per animation
    if (FX < FXlast) FX++;  //FX % FXtotal
    else FX = 0;
    FX_count = 0;
  } else FX_count++;
}

//----------------------------------SETUP
void setup() {

  hspi = new SPIClass(HSPI);
  hspi->begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_CS);  //SCLK, MISO, MOSI, SS

  pinMode(IR_PIN, INPUT);
  pinMode(LE1, OUTPUT);
  pinMode(LE2, OUTPUT);

  //Clear all LEDs
  hspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  clearLEDs();
  hspi->endTransaction();

  randomSeed(analogRead(0));  // Initialize random number generator
  memset(persistenceGrid, 0, sizeof(persistenceGrid));

  allocateAnimationFrames();
  precalculatePlanetaryGearboxAnimation();

//FX=16;
//oldFX=15;

  //Serial.begin(115200); just for debugging
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else  // U_SPIFFS
        type = "filesystem";
      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    })
    .onEnd([]() {
    })
    .onProgress([](unsigned int progress, unsigned int total) {
    })
    .onError([](ota_error_t error) {
    });
  ArduinoOTA.begin();
  hspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));

  // Configure Timer0 Interrupt . the remaining configuration is inside the int IR
  timer = timerBegin(1000000);       //timer 1Mhz resolution
  timerAttachInterrupt(timer, &Tm);  //attach callback

  delay(500);
  attachInterrupt(digitalPinToInterrupt(IR_PIN), ISR_IR, FALLING);  // IR int = Falling, in Rising it shakes a little
  
}

//----------------------------------LOOP
void loop() {
  ArduinoOTA.handle();
}