// this sample code provided by www.programmingboss.com
#define RXp2 16
#define TXp2 17
#include <iostream>
#include "BluetoothSerial.h" 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "NewPing.h"   

//for ultrasonic 
int trigPin = 19;      // trigger pin
int echoPin = 18;      // echo pin
NewPing sonar(trigPin, echoPin);
// init Class:
BluetoothSerial ESP_BT; 

// Parameters for Bluetooth interface and timing
int incoming;                           // variable to store byte received from phone 
int id = -1;                            // received identification byte
int val_byte1 = -1;                     // most significant byte of data value
int val_byte2 = -1;                     // least significant byte of data value

  char str[4];
  int bag1 = 0;
  int bag2 = 0;
  int bag3 = 0;
  int bag4 = 0;
  int bag5 = 0;
  int bag6 = 0;
  int bag1hole = 0;
  int bag2hole = 0;
  int bag3hole = 0;
  int bag4hole = 0;
  int bag5hole = 0;
  int bag6hole = 0;
  int bag1found = 0;
  int bag2found = 0;
  int bag3found = 0;
  int bag4found = 0;
  int bag5found = 0;
  int bag6found = 0;

  int scoreteam1 = 0;
  int scoreteam2 = 0;
  int IR_val = 0;
  int end_round = 0;
  int scoretotal;

void setup() {
  Serial.begin(9600);
  ESP_BT.begin("ESP32_Control1");        // Name of your Bluetooth interface -> will show up on your phone
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  //pinMode(19, INPUT_PULLUP);
}
void loop() {
    float distance = sonar.ping_median(5);
    //if(distance>600 || distance<2) Serial.println("Out of range");
    //else
    {
      if(distance<600 && distance>2) {
      Serial.print("Distance: ");
      Serial.print(distance, 1); Serial.println(" cm");
      }
    }
    delay(50);
    while (Serial2.available() > 0) {
      String str = Serial2.readString();
      Serial.println(str);
      //IR_val = digitalRead(19);
      Serial.println(IR_val);
      String sub = str.substring(15, 20);
      Serial.println(sub);
      end_round = ESP_BT.read();

      if (sub == ("11 1F")) {
        Serial.println("bag 1 front found");
        bag1 = 1;
        if(distance > 50 && distance < 500 && bag1found == 0){
          Serial.println("bag 1 has fallen in the hole [front]");
          bag1hole = 2;
        }
        bag1found = 1;
      }

      else if (sub == ("11 2F")) {
        Serial.println("bag 2 front found");
        bag2 = 1;
        if(distance > 50 && distance < 500 && bag2found == 0){
          Serial.println("bag 2 has fallen in the hole [front]");
          bag2hole = 2;
        } 
      bag2found = 1;
      }
      else if (sub == ("22 1F") ) {
        Serial.println("bag 4 front found");
        bag4 = 1;
        //IR_val = digitalRead(19);
        Serial.println("IR_val" + IR_val);
        if(distance > 50 && distance < 500 && bag4found == 0){
          Serial.println("bag 4 has fallen in the hole [front]");
          bag4hole = 2;
        } 
      bag4found = 1;
      }
      else if (sub == ("22 2F") ) {
        Serial.println("bag 5 front found");
        bag5 = 1;
        if(distance > 50 && distance < 500 && bag5found == 0){
          Serial.println("bag 5 has fallen in the hole [front]");
          bag5hole = 2;
        }
      bag5found = 1;
      }
    scoreteam1 = bag1 + bag2 + bag3 + bag1hole + bag2hole + bag3hole;
    scoreteam2 = bag4 + bag5 + bag6 + bag4hole + bag5hole + bag6hole;
    Serial.println(scoreteam1);
    Serial.println(scoreteam2);
    scoretotal = scoreteam1 * 10 + scoreteam2;
    Serial.println(scoretotal);
    send_BT(id, scoretotal);
    }

     if (ESP_BT.available()) 
  {
    end_round = ESP_BT.read();           // Read what we receive and store in "incoming"

    if (incoming > 127) {               // ID bytes are 128 or higher, so check if incoming byte is an ID-byte
      reset_rx_BT();                    // reset id and data to -1
      id = incoming - 128;              // write id value
    }
    else if (val_byte1 == -1) {         // if incoming byte is lower than 128 it is a data byte. Check if first data byte is empty (-1)
      val_byte1 = incoming;             // write first data byte (MSB)
    }
    else if (val_byte2 == -1) {         // check if second data byte is empty (-1)
      val_byte2 = incoming;             // write second data byte (LSB)
      int value = 128*val_byte1 + val_byte2;          // recombine the first and second data byte to the actual value that was sent from the phone
      // here is the location that you can implement the code what you want to do with the controller id and value received from the phone
      
      Serial.print("Id: "); Serial.print(id); Serial.print(", val: "); Serial.println(value);   // for debugging write to the serial interface (check with serial monitor)
      send_BT(id, value);               // for test purposes we just send the data back to the phone
      reset_rx_BT();                    // not strictly needed, but just in case erase all bytes (set to -1)
      send_BT(id, scoretotal);
    }
  }
}

void reset_rx_BT() {                    // function to erase all bytes (set to -1)
  id = -1;
  val_byte1 = -1;
  val_byte2 = -1;
}
void send_BT(int id, int value) {       // function to write id and value to the bluetooth interface (and split value in MSB and LSB
  //ESP_BT.write(128 + id);
  //ESP_BT.write(floor(value/128));       // MSB
  //ESP_BT.write(value%128);              // LSB

  ESP_BT.write(scoretotal);
    
}
    //if(ir sensor high && new bag found)
    //if (end round) {
      //bag1 = bag2 = bag3 = bag4 = bag5 = bag6 = 0;
      //score = 0;
    //}


