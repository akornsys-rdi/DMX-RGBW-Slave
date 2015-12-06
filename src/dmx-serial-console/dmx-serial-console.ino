// - - - - -
// DmxSerial - A hardware supported interface to DMX.
// DmxSerialSend.ino: Sample DMX application for sending 3 DMX values:
// 
// Copyright (c) 2011 by Matthias Hertel, http://www.mathertel.de
// This work is licensed under a BSD style license. See http://www.mathertel.de/License.aspx
// 
// Documentation and samples are available at http://www.mathertel.de/Arduino
// 25.07.2011 creation of the DmxSerial library.
// 10.09.2011 fully control the serial hardware register
//            without using the Arduino Serial (HardwareSerial) class to avoid ISR implementation conflicts.
// 01.12.2011 include file and extension changed to work with the Arduino 1.0 environment
// - - - - -

#include <DMXSerial.h>

void setup() {
    static unsigned char cfg = 1;
    
    Serial.begin(9600);
    do {
        Serial.println("DMX Controller");
        Serial.println("---");
        Serial.println("Current configuration:");
        Serial.print("Mode: ");
        if (modeChannel == 1) Serial.println("ON");
        else if (modeChannel == 2) Serial.println("ON");
        else if (modeChannel == 10) Serial.println("ON");
        else Serial.println("NONE");
        Serial.println("Channels: 5");
        Serial.print("Channel mode: ");
        Serial.println(stChannel);
        Serial.print("Channel red: ");
        Serial.println(stChannel+1);
        Serial.print("Channel green: ");
        Serial.println(stChannel+2);
        Serial.print("Channel blue: ");
        Serial.println(stChannel+3);
        Serial.print("Channel white: ");
        Serial.println(stChannel+4);
        Serial.println();
        
    } while(cfg);
    Serial.end();
    DMXSerial.init(DMXController);
}

void parse() {
  char c;

  while(!Serial.available());
  c = Serial.read();
  if ((c >= '0') && (c <= '9')) {
    value = 10 * value + c - '0';
  }
  else {
    if (c == 'c') channel = value;
    else if (c == 'w') {
        // actualizar valor
    }
    value = 0;
  }    
}

void loop() {
    static unsigned char channel[6];
    static unsigned char value[6];

    DMXSerial.write(3, 127);
}

