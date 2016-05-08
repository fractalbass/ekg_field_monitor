///////////////////////////////////////////////////////////////
//  EKGFieldMonitorRevB
//  A simplified program to send out the
//  EKG readings from the Olimex EKG shield
//  Each reading includes a time delta since the last reading
//  (measured in milliseconds) and a value (0-23).
//
//  Miles Porter, Painted Harmony Group, Inc
//
//  April 18, 2016
///////////////////////////////////////////////////////////////

#include <SPI.h>
#include <SD.h>

const int EKG_PIN=0;
const int PULSE_PIN=6;
const int WRITE_PIN=3;
const int PAUSE=20;
long lastTimeStamp=0;
byte samples[100];
byte deltaT[100];
int cur_counter = 0;
long log_num = 0;

const int chipSelect = 4;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
}

void loop() {
  // Get the EKG value
  int ekg = analogRead(EKG_PIN);

  //  Calculate the DeltaT
  long timeStamp = millis();
  int dt = int(timeStamp - lastTimeStamp);
  lastTimeStamp = timeStamp;

  //  If this is a pulse signal, turn on the PULSE_PIN
  if (ekg > 750) {
    analogWrite(PULSE_PIN,ekg);
  } else {
    analogWrite(PULSE_PIN, 0);
  }
  samples[cur_counter] =(byte) map(ekg,0,1024,0,255);
  deltaT[cur_counter] = (byte) dt;
  cur_counter++;

  if (cur_counter>99) {
    cur_counter=0;
    writeSamples();
  }
  delay(PAUSE);  
}

void writeSamples() { //Simulate Writing to the SD Card.
  digitalWrite(WRITE_PIN, HIGH);
  log_num++;
  String log_str = (String) log_num;
  String filename = "ekg" + log_str + ".log";
  Serial.println("Writing data to " + filename);
  File dataFile = SD.open(filename, FILE_WRITE);
  
  // if the file is available, write to it:
  if (dataFile) {
    for(int i=0;i<100;i++) {
      dataFile.print(samples[i]);
      dataFile.print(", ");
      dataFile.println(deltaT[i]);
    }
    dataFile.close();
    Serial.println("Write successful");
    // if the file isn't open, pop up an error:    
  } else {
    Serial.println("error opening datalog.txt");
   }
  digitalWrite(WRITE_PIN, LOW);
}
