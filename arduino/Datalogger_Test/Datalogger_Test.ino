/*
  SD card datalogger

 This example shows how to log data from three analog sensors
 to an SD card using the SD library.

 The circuit:
 * analog sensors on analog ins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4

 created  24 Nov 2010
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;
const int CS_PIN=10;
const int POW_PIN=8;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  pinMode(6, OUTPUT);
  Serial.print("Initializing SD card...");
   
  pinMode(CS_PIN, OUTPUT);
  pinMode(POW_PIN, OUTPUT);
  digitalWrite(POW_PIN, HIGH);

  if (!SD.begin(CS_PIN)) {
    Serial.println("Card failed.");
  }
  else {
    Serial.println("card initialized.");
  }
}

void loop() {
  // make a string for assembling the data to log:
  String dataString = "";
  int sensor=0;
  String comma = "";
  // read three sensors and append to the string:
  for (int analogPin = 0; analogPin < 6; analogPin++) {
    sensor = analogRead(analogPin);
    dataString += comma + String(sensor);
    comma = ",";
  }
  
  analogWrite(6, sensor);
  

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}









