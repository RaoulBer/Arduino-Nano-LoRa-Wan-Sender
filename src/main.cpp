#include <SPI.h>
#include <LoRa.h>

//Ultraschallsensor
#define PIN_TRIGGER 7
#define PIN_ECHO    6

const int SENSOR_MAX_RANGE = 300; // in cm
unsigned long waveSpeed = 58;
unsigned long duration;
unsigned long distance;
int prevDistance = 0;
int threshold = 2;

// LoRa WAN module
#define ss 13
#define rst 3
#define dio0 5
//SCK  16
//MISO 15
//MOSI 14



int counter = 0;

int measureDistance(){
  int returnValue;
  digitalWrite(PIN_TRIGGER, LOW);
  delayMicroseconds(2);

  digitalWrite(PIN_TRIGGER, HIGH);
  delayMicroseconds(10);

  duration = pulseIn(PIN_ECHO, HIGH);
  distance = duration/waveSpeed;
  Serial.println(String(distance));

  if (distance > SENSOR_MAX_RANGE || distance <= 0){
    returnValue = -1;
  } else {
    returnValue = distance;
  }
  return returnValue;
}

void setup() {  
  Serial.begin(9600);
  pinMode(PIN_TRIGGER, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  while (!Serial);
  Serial.println("LoRa Sender");
  LoRa.setPins(ss, rst, dio0);

  if (!LoRa.begin(866E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.print("Distance is: ");
  int distance = measureDistance();
  Serial.println(distance);

  // send packet
  LoRa.beginPacket();
  if (abs(distance - prevDistance) > threshold){
    LoRa.print("Post");
  }
  LoRa.endPacket();
  delay(2000);
  prevDistance = distance;
}