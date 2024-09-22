# Volumetric-Display, with ESP32 and LED matrix

VOXEL by #dropes


<img src="https://github.com/pdropes/Volumetric-Display/blob/main/Photos/POV_Cover.jpg" width=1024>
Video: https://youtu.be/vmjfssugH9E


## VOXEL - HARDWARE

- The motor I used the head of a VCR, after opening it, I placed the power transmitter coils inside, the ferrites had to be filed (very carefully) to fit well and then glued them.
- The distance between the coils is very important and must be calibrated in advance, with the help of a power supply with adjustable maximum current. In the receiver, you must connect a low value resistance 2ohms, or a 20W 12V lamp, with a multimeter to monitor the current passing through it.
- Then the emitter must be supplied with 12V, without progressive increases, the supply must be immediate or it will burn out, maximum current of 2A to start.
- The objective is to have the maximum current measured in the receiver, but without saturating the coils, to do this the distance between the coils is slowly reduced, the current rises to a point, then starts to fall, the distance from the coil must be recorded highest measured current.
- It is likely that the source used in these tests will present a current peak when calibrating the distances. You must be very careful not to overheat both the transmitter and the receiver, and you must immediately move the coils away.
- Both must have a heat sink, both on the two boards and on the emitter power transistors.
- The power supply must be at least 50W, with a voltage of 18V to 24V, with a lower voltage the wireless power transmitter will overheat, requiring more current to compensate.
- The IR transmitter and receiver are inside the head through two holes, this head must rotate without touching anything.
- Assembling all the components requires some skills, if you are not able to solder with an oven or heating plate and hot air station, I recommend ordering the PCBs with the components already soldered, joining the two plates will have to be done later.
- The most difficult are the 1024 “0603 LEDs” and the 64 “controller ICs”.
- The PCBs must have the minimum thickness possible, with 0.6mm being a good value, with the LED controller ICs touching them. Their color doesn't matter much, in rotation it becomes unnoticeable.

More calibrations:
- The 1st balance is static, with the boards horizontal and without the tube, weights must be placed, so that no differences are noticed when rotating manually, always horizontally.
- The dynamic balance was achieved by placing a piezo on one of the legs of the box, visualizing the signal amplitude on the oscilloscope, then placing washers at certain points, where the displayed amplitude reduces.
- The tube alignment must also be done, looking for eccentricity and having as little oscillation as possible.
  These processes can be tedious and time-consuming, be patient.

## VOXEL - SOFTWARE

Project carried out in Arduino IDE v2.2.1
The 1st programming must be via USB, the rest can be via Wi-Fi for the convenience of keeping the tube, this is the best option, the Arduino-ota routine must be configured in advance, with the name and password of the network to be connected, this part must always remain in the code.
Although you chose the ESP32S2, another type of ESP32 should work, it uses the internal RAM for animations, and the optional PSRAM, for pre-calculation of heavier animations.
The USB connection is direct, without the need for a separate IC for this.
Programming the ESP32 must be done with the engine stopped, in all cases.

Programming steps:
- Connect the VOXEL board directly via USB to the PC.
- Open Arduino, in BOARDS MANAGER, find and install “esp32 by Espressif”
- In LIBRARIES MANAGER, find and install “Arduino_ESP32_OTA by Arduino”
- In TOOLS-BOARDS-ESP32-ESP32S2 Dev Module, choose the COM where the VOXEL is connected
- Check the board settings with the attached file “Arduino V2.2.1 config.png”.
- In VOXEL.INO change the name and password of the local network 
- Compile and upload to ESP32.
- After that, all programming does not require a USB connection, on the Arduino IDE, the Wi-Fi port must be visible.
- If you always intend to program via Wi-Fi, the Arduino-ota settings in VOXEL.ino must remain.

New Animation:
- When programming more effects, you must create a blank file in the same folder, extension .h
- Open VOXEL.ino
- Include the new file name in “FX”
- Next, the “FXlast” declaration, must be changed to the last effect number.
- void ISR_IR, which is the int. that detects the complete turn via IR, the procedure to be carried out in this new effect must be included (check the examples already written).
- void ARDUINO_ISR_ATTR, this is a timer that is run 100 times per turn, the reference of the new effect must also be included, or copy 3 lines from another, so that it can be clearly displayed.
- Shared.h file shares variables between VOXEL.ino and effect.h, or use static variables to avoid conflicts.

## WARNING !
The use of acrylic tube is NECESSARY in this assembly!
It keeps the boards in place at the top, prevents air friction, and ensures GREATER SAFETY to the touch.
Its absence can cause PHYSICAL OR VISION DAMAGE, the rotation speed corresponds to between 25 and 42 km/h!

