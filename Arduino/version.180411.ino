/*
WongyuLee
2015118466
hanhb0@naver.com
update : get timestamp using Timer interrupt
참고:https://arsviator.blogspot.kr/2009/06/%EC%95%84%EB%92%A4%EB%85%B8%EC%97%90%EC%84%9C-%ED%83%80%EC%9D%B4%EB%A8%B8-%EC%9D%B8%ED%84%B0%EB%9F%BD%ED%8A%B8-timer-interrupt-in.html
pin연결 
arduino     sensor
3.3v        vdd
gnd         gnd
A4          sda
A5          scl
*/

#include "MPU9250.h"
#include "MsTimer2.h"
MPU9250 sensor;
const int pinX = 0;
const int pinY = 1;
const int pinZ = 2;
int minVal = 265;
int maxVal = 402;
volatile double ax, ay, az;
volatile int rx, ry, rz; // analog read value
volatile int xAng,yAng,zAng;
volatile unsigned long time_stamp;
void setup() {
//Wire.begin();
Serial.begin(9600);
MsTimer2::set(16,getAccel_Data); //timeinterval = 16ms
MsTimer2::start();
//Nonnection Check
//Serial.println(sensor.testConnection() ? "MPU9250 connection successful" : "MPU9250 connection failed");
}

void loop() {
}

void getAccel_Data(void)
{
  rx = analogRead(pinX);
  ry = analogRead(pinY);
  rz = analogRead(pinZ);
  xAng = map(rx, minVal, maxVal, -90, 90);
  yAng = map(ry, minVal, maxVal, -90, 90);
  zAng = map(rz, minVal, maxVal, -90, 90);
  ax = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  ay = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  az = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);
  time_stamp = millis();
  Serial.print(time_stamp, DEC);
  Serial.print(", ");
  Serial.print(ax,6);
  Serial.print(", ");
  Serial.print(ay,6);
  Serial.print(", ");
  Serial.println(az,6);
}


