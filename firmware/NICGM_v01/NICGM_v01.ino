/////Open Source Non-Invasive Glucose Monitor v0.1//////////////


#include <Wire.h>
#include "MCP3421.h" 
MCP3421 MCP = MCP3421();

int LED[] = {5,6,11,10}; //LEDs Red, IR, Green, NIR, respectively
int buzzer = 9;
//int button 
long rawReadings[] = {0,0,0,0}; //Raw ADC readings from MCP3421, same order as LEDs.
//long darkReadings[] = {0,0,0,0}; //Dark Values
//long lightReadings[] = {0,0,0,0};  //Full Light Values
long OXYReadings[] = {0,0};

///PRECALIBRATED VALUES/////
long darkReadings[] = {32301,32135,32169,31690}; //Raw ADC readings from MCP3421, same order as LEDs.
long lightReadings[] = {8300,-9240,28100,-8471}; //Dark Values

void setup(){
  for (int x=0; x<4; x++){
    pinMode(LED[x], OUTPUT);
    digitalWrite(LED[x], LOW);
  }
  
  pinMode(buzzer, OUTPUT);
  analogWrite(buzzer, 20);
  delay(200);
  analogWrite(buzzer, 50);
  delay(200);
  digitalWrite(buzzer,LOW);
  Serial.begin(9600);
  //Serial.println("NICGM v0.1");
  //Serial.println("Initiallizing ADC");
  delay(20);
  
  //bloodOxygen();
  
  MCP.init(0x68,3,0);
  //Calibration();
}

void loop(){
  testReading();
  delay(50);
}

void testReading(){
  
  int sampleNum = 3;
  for (int led = 0; led<4;led++){
    long avg = 0;
    digitalWrite(LED[led], HIGH);
    delay(100);
    for (int sample=0; sample<sampleNum; sample++){
      while (MCP.ready() == 0);
      rawReadings[led] = MCP.getLong();
      avg += rawReadings[led];
    }
    rawReadings[led] = avg / sampleNum;
    digitalWrite(LED[led], LOW);
    //rawReadings[led] = map(rawReadings[led], lightReadings[led],darkReadings[led],0, 255);
  }
  
  for (int y = 0; y<3; y++){
    
    Serial.print(rawReadings[y]);
    Serial.print(",");
  }
  
  Serial.println(rawReadings[3]);
}

void getDark(){
   int sampleNum = 100;
  for (int led = 0; led<4;led++){
    long avg = 0;
    digitalWrite(LED[led], HIGH);
    delay(100);
    for (int sample=0; sample<sampleNum; sample++){
      while (MCP.ready() == 0);
      darkReadings[led] = MCP.getLong();
      avg += darkReadings[led];
    }
    darkReadings[led] = avg / sampleNum;
    digitalWrite(LED[led], LOW);
  }
  Serial.println("The Dark Values Are:");
  for (int y = 0; y<3; y++){
    Serial.print(darkReadings[y]);
    Serial.print(",");
  }
  Serial.println(darkReadings[3]);
  delay(1000);
}

void getFulllight(){
   int sampleNum = 10;
  for (int led = 0; led<4;led++){
    long avg = 0;
    digitalWrite(LED[led], HIGH);
    delay(100);
    for (int sample=0; sample<sampleNum; sample++){
      while (MCP.ready() == 0);
      lightReadings[led] = MCP.getLong();
      avg += lightReadings[led];
    }
    lightReadings[led] = avg / sampleNum;
    digitalWrite(LED[led], LOW);
  }
  Serial.println("The Full Light Values Are:");
  for (int y = 0; y<3; y++){
    Serial.print(lightReadings[y]);
    Serial.print(",");
  }
  Serial.println(lightReadings[3]);
  delay(1000);
}

void bloodOxygen(){
  Serial.print('\r');
  MCP.init(0x68,3,1);
  while(1){
    int sampleNum = 3;
    for (int led = 0; led<2;led++){
    long avg = 0;
    digitalWrite(LED[led], HIGH);
    delay(20);
    for (int sample=0; sample<sampleNum; sample++){
      while (MCP.ready() == 0);
      OXYReadings[led] = MCP.getLong();
      avg += OXYReadings[led];
    }
    OXYReadings[led] = avg / sampleNum;
    digitalWrite(LED[led], LOW);
  }

  for (int y = 0; y<2; y++){
    Serial.print(OXYReadings[y]);
    Serial.print(",");
  }
  Serial.println(OXYReadings[1]);
  delay(1);
}
}

void Calibration(){
  //MCP.init(0x68,1,1);
  
  Serial.println("Block Light Fully");
  Serial.println("Send any Char to Begin");
  while (Serial.available() && Serial.read()); // empty buffer
    while (!Serial.available());                 // wait for data
    while (Serial.available() && Serial.read()); 
  delay(100);
  Serial.println("Calibrating Dark Voltage");
  getDark();
  Serial.println("Expose Light");
  Serial.println("Send any Char to Begin");
  while (Serial.available() && Serial.read()); // empty buffer
    while (!Serial.available());                 // wait for data
    while (Serial.available() && Serial.read()); 
  delay(100);
  Serial.println("Calibrating Light Voltage");
  getFulllight();
  Serial.println("Calibration Complete");
  Serial.print('\r\n');
}
