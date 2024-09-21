//----------------------------------Two Sines

//----------------------------------Table for direct output
uint8_t seq[64][2] = {
  { 63, 0 }, { 59, 1 }, { 55, 0 }, { 51, 1 }, { 47, 0 }, { 43, 1 }, { 39, 0 }, { 35, 1 }, 
  { 31, 0 }, { 27, 1 }, { 23, 0 }, { 19, 1 }, { 15, 0 }, { 11, 1 }, {  7, 0 }, {  3, 1 }, 
  {  2, 1 }, {  6, 0 }, { 10, 1 }, { 14, 0 }, { 18, 1 }, { 22, 0 }, { 26, 1 }, { 30, 0 }, 
  { 34, 1 }, { 38, 0 }, { 42, 1 }, { 46, 0 }, { 50, 1 }, { 54, 0 }, { 58, 1 }, { 62, 0 }, 
  { 61, 0 }, { 57, 1 }, { 53, 0 }, { 49, 1 }, { 45, 0 }, { 41, 1 }, { 37, 0 }, { 33, 1 }, 
  { 29, 0 }, { 25, 1 }, { 21, 0 }, { 17, 1 }, { 13, 0 }, {  9, 1 }, {  5, 0 }, {  1, 1 }, 
  {  0, 1 }, {  4, 0 }, {  8, 1 }, { 12, 0 }, { 16, 1 }, { 20, 0 }, { 24, 1 }, { 28, 0 }, 
  { 32, 1 }, { 36, 0 }, { 40, 1 }, { 44, 0 }, { 48, 1 }, { 52, 0 }, { 56, 1 }, { 60, 0 }
};


//sine wave
const float PI_180 = PI / 180;
uint8_t wave;
uint8_t column;
uint8_t sine_wave;
uint8_t value1, value2;
uint8_t buff3[SIZE * 2];
uint8_t buff4[SIZE * 2];


void showSine(int deg) {

  memset(buff3, 0, sizeof(buff3));  //apaga toda a memória do buffer 3
  memset(buff4, 0, sizeof(buff4));  //apaga toda a memória do buffer 4

  for (uint8_t column = 0; column < 8; column++) {
    uint8_t sine_wave = round(15 + (15 * sin((deg + (column * 15)) * PI_180 )));


    if ((sine_wave % 2) == 0) {

      sine_wave = sine_wave / 2;

      bitSet(buff3[sine_wave], column);
      bitSet(buff3[63 - sine_wave], 7 - column);

      bitSet(buff3[31 - sine_wave], column);
      bitSet(buff3[32 + sine_wave], 7 - column);

      sine_wave = round(15 + (15 * sin((120 + deg + (column * 15)) * PI_180 )));
      sine_wave = sine_wave / 2;
      bitSet(buff3[31 - sine_wave], column);
      bitSet(buff3[32 + sine_wave], 7 - column);

      bitSet(buff3[sine_wave], column);
      bitSet(buff3[63 - sine_wave], 7 - column);

    } else {
      sine_wave = (sine_wave + 1) / 2;
      bitSet(buff4[sine_wave], column);
      bitSet(buff4[63 - sine_wave], 7 - column);

      bitSet(buff4[31 - sine_wave], column);
      bitSet(buff4[32 + sine_wave], 7 - column);

      sine_wave = round(15 + (15 * sin((120 + deg + (column * 15)) * PI_180 )));
      sine_wave = (sine_wave + 1) / 2;
      bitSet(buff4[31 - sine_wave], column);
      bitSet(buff4[32 + sine_wave], 7 - column);

      bitSet(buff4[sine_wave], column);
      bitSet(buff4[63 - sine_wave], 7 - column);
    }
  }

  for (int t = 0; t < 64; t++) {
    value1 = buff3[t];
    value2 = buff4[t];

    if (seq[t][1]) {
      buff1[t] = reverse(value1);
      buff2[t] = reverse(value2);
    } else {
      buff1[t] = value1;
      buff2[t] = value2;
    }
  }
}

