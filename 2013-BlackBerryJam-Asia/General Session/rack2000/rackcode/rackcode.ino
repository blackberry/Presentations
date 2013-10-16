/* Copyright (c) 2013 BlackBerry.
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
 
int relay1 = 3;
int relay2 = 4;
int relay3 = 5;
int relay4 = 6;





int incomingByte = 0; // for incoming serial data
int incomingByteCommand = 0; // for incoming serial data
int i=0;
char commandbuffer[100];


void setup()
{
  Serial.begin(9600);
  Serial.println('Hello from serial');


  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT); 
  pinMode(relay3, OUTPUT); 
  pinMode(relay4, OUTPUT); 

  //set all to low, just in calse
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
  digitalWrite(relay4, LOW);

  //for testing purposes
  digitalWrite(relay2, HIGH);
  delay(1000);
  digitalWrite(relay2, LOW);


}
void loop()
{
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    //heartbeat code just to keep the BT connection running.
    if(incomingByte == 65)
    {
      Serial.print("H");
    }

    else if(incomingByte == 49) //1
    {
      while(!Serial.available())
      {
      }
      incomingByteCommand  = Serial.read();
      if(incomingByteCommand == 49)
      {
        digitalWrite(relay1, HIGH);
      }
      else{
        digitalWrite(relay1, LOW);
      }
    }
    else if(incomingByte == 50) //2
    {
      while(!Serial.available())
      {
      }
      incomingByteCommand  = Serial.read();
      if(incomingByteCommand == 49)
      {
        digitalWrite(relay2, HIGH);
      }
      else{
        digitalWrite(relay2, LOW);
      }
    }
    else if(incomingByte == 51) //3
    {
      while(!Serial.available())
      {
      }
      incomingByteCommand  = Serial.read();
      if(incomingByteCommand == 49)
      {
        digitalWrite(relay3, HIGH);
      }
      else{
        digitalWrite(relay3, LOW);
      }

    }
    else if(incomingByte == 52) //4
    {
      while(!Serial.available())
      {
      }
      incomingByteCommand  = Serial.read();
      if(incomingByteCommand == 49)
      {
        digitalWrite(relay4, HIGH);
      }
      else{
        digitalWrite(relay4, LOW);
      }

    }
    else if(incomingByte == 48) //0 //Lets kill all the lights
    {
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, LOW);
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, LOW);
    }
    else
    {
      // say what you got:
      Serial.print("Did not understand: ");
      Serial.write(incomingByte);
      Serial.print(incomingByte,DEC);    
      Serial.println();   
    }


  }
}











