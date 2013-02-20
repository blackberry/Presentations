/*
 * Copyright 2013 Research In Motion
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// This code depends on the following library;
// http://labs.arduino.cc/ADK/AccessoryMode
// http://labs.arduino.cc/uploads/ADK/GettingStarted/ArduinoADK-beta-001.zip
//  - i used the original version - not v2.

#include <AndroidAccessory.h>
#include <Servo.h>

// Just copied these values from the sample..,
char accessoryName[] = "Mega_ADK";
char companyName[] = "Arduino SA";

// Tracking the time to not send too much data
long timer = millis();

// Track if the last time we looped we were connected
int8_t lastConnected = false;

// initialize the accessory:
AndroidAccessory usb(companyName, accessoryName);

// Servo for connections.
Servo servo;

int16_t angle = 0; // could have fit the angle in unsigned 8bit...
int8_t direction = 2; // direction is added to angle when the timer goes off
int8_t timerTick = 0; // Used to track every 3rd timer interrupt.

// Move the servo via timer interrupt - the USB code can block.
ISR(TIMER2_OVF_vect) {
  timerTick++;
  if (timerTick%3==0) {
    angle += direction;
    timerTick = 0;
    if (angle < 0) {
      direction = -direction;
      angle = direction;
    } else if (angle > 180) {
      direction = -direction;
      angle = 180 + direction;
    }
    
    servo.write(angle);
  }
};

// Setup timer 2 for interrupts.
void setupTimer() {
  cli();  
  TCCR2A = (0 << COM2A1) | (0 << COM2A0) | (0 << COM2B1) | (0 << COM2B0) | (0 << WGM21) | (0 << WGM20);
  TCCR2B = (0 << FOC2A) | (0 << FOC2B) | (0 << WGM22) | (1 << CS22) | (1 << CS21) | (0 << CS20);
  TCNT2 = 0;
  OCR2A = 0;
  OCR2B = 0;
  ASSR = 0;
  TIMSK2 = (1 << TOIE2);
  sei();
}

void setup() {
  usb.begin();

  servo.attach(12);
  
  setupTimer();

  Serial.begin(115200);
  Serial.println("Started");
}

void loop() {
  if(millis()-timer>20) { // sending 50 times per second
    if (usb.isConnected()) { // isConnected makes sure the USB connection is open
      if (!lastConnected) {
        Serial.println("Connected");
        lastConnected = true;
      }
      int16_t distance = analogRead(0);
      
      byte toWrite[6];
      toWrite[0] = 0xff;
      toWrite[1] = 0xff;
      toWrite[2] = angle;
      toWrite[3] = distance >> 8;
      toWrite[4] = distance;
      toWrite[5] = 0;
      
      usb.write(toWrite,6);
    } else {
       if (lastConnected) {
         Serial.println("Disconnected");
         lastConnected = false;
      }
    }
    timer = millis();
  }
}
