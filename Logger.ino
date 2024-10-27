/*
 * 
  SD card datalogger with Teensyduino 4.1
 
  SD card attached to SPI bus as follows:
    MOSI - pin 11, pin 7 on Teensy with audio board
    MISO - pin 12
    CLK - pin 13, pin 14 on Teensy with audio board
    CS - pin 4,  pin 10 on Teensy with audio board
   
 */

#include <SD.h>
#include <SPI.h>
#include <TimeLib.h>
#include <EEPROM.h>

// User define two of the Serial inputs as SR1 and SR2

#define SR1 Serial1
#define SR2 Serial3

// Two LEDs should be connected, which indicates any file_write action to SD card. Pin could be changed. 

#define LED_SR1 5
#define LED_SR2 6

// One pushbutton should be connected, which triggers ISR when pressed (rising edge of signal), 
// to print all files to serial monitor.  Pin could be changed. 

#define BUTTON 7

// IMPORTANT
// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.

// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
// Teensy audio board: pin 10
// Teensy 3.5 & 3.6 & 4.1 on-board: BUILTIN_SDCARD
// Wiz820+SD board: pin 4
// Teensy 2.0: pin 0
// Teensy++ 2.0: pin 20

const int chipSelect = BUILTIN_SDCARD;

// Global var

int address_EEPROM = 0;
char dirName[24];

void setup() {
  //UNCOMMENT THESE TWO LINES FOR TEENSY AUDIO BOARD:
  //SPI.setMOSI(7);  // Audio shield has MOSI on pin 7
  //SPI.setSCK(14);  // Audio shield has SCK on pin 14

  Serial.begin(9600);
  SR1.begin(38400);
  SR2.begin(38400);
  while(!Serial && !SR1 && !SR2){
    ; // wait for initialization
  }

  Serial.println("Initializing SD card...");

  // Check if initialized successfully
  if (!SD.begin(chipSelect)) {
    Serial.println("Card initialization failed or not present");
    return;
  }
  Serial.println("Card initialized");

  // Make new directory for each reset/power restart on logger. 
  // Test number saved in EEPROM, which resets to 0 on 256th tests (0 - 255). 
  int testNum = EEPROM.read(address_EEPROM);
  Serial.println(testNum);
  sprintf(dirName, "Test_%d", testNum);
  SD.mkdir((const char *)dirName);
  if (testNum == 255) {
    testNum = -1;
  }
  EEPROM.update(0, testNum+1);

  // Initialize LED for indicating file_write action
  pinMode(LED_SR1, OUTPUT);
  pinMode(LED_SR2, OUTPUT);
  
  // Attach interrupt for reading files to Serial monitor when BUTTON is pressed
  pinMode(BUTTON, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON), readSDfile, RISING);
  
}

void loop() {
  // Stores data from Serial #1
  String dataSR1 = "";

  if (SR1.available() > 0) {
    dataSR1 = SR1.read();
    char FileSR1[30];
    sprintf(FileSR1, "/%s/FileSR1.txt", dirName); 
    File dataFileSR1 = SD.open(FileSR1, FILE_WRITE);

    // Write into file if it's available
    if (dataFileSR1) {
      dataFileSR1.print(dataSR1);
      dataFileSR1.close();
      digitalWrite(LED_SR1, HIGH);
      digitalWrite(LED_SR1, LOW);
      // Serial.print(dataSR1);   Debug output
    } else {
      Serial.println("Error opening/creating file1");
    }
  }

  // Stores data from Serial #2
  String dataSR2 = "";
  
  if (SR2.available() > 0) {
    dataSR2 = Serial.read();
    char FileSR2[30];
    sprintf(FileSR2, "/%s/FileSR2.txt", dirName); 
    File dataFileSR2 = SD.open(FileSR2, FILE_WRITE);

    // Write into file if it's available
    if (dataFileSR2) {
      dataFileSR2.print(dataSR2);
      dataFileSR2.close();
      digitalWrite(LED_SR2, HIGH);
      digitalWrite(LED_SR2, LOW);
      // Serial.print(dataSR2);    Debug output
    } else {
      Serial.println("Error opening/creating file2");
    }
  }
  
}


// ISR
void readSDfile() {
  int readLimit = EEPROM.read(address_EEPROM);

  for (int i = 0; i <= readLimit; i++) {
    for (int j = 1; j < 3; j++) {
      char readFileName[30];
      sprintf(readFileName, "Test_%d/FileSR%d.txt", i, j);
      File dataFile = SD.open(readFileName);
      if (dataFile) {
        Serial.write("Start printing file:");
        Serial.write(readFileName);
        Serial.write("\n");
        while (dataFile.available()) {
          Serial.write(dataFile.read());
        }
        dataFile.close();
      }
    }
  }
  
}
