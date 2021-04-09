// 참고 : http://www.jkelec.co.kr/img/sensors/manual/mpu9250_gy9250/mpu9250_gy9250_manual.html
#include "MPU9250.h"
#include "Wire.h"
MPU9250 accelgyro;
float Axyz[3];
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;
unsigned long time_stamp;
unsigned long previousTime = 0;
unsigned long time_interval = 16000; //(microsecond) = 16millisecond
void setup() {
Wire.begin();
Serial.begin(9600);
accelgyro.initialize();
//Serial.println(accelgyro.testConnection() ? "MPU9250 connection successful" : "MPU9250 connection failed");
}

void loop() {
  if(micros() - previousTime >= time_interval){
    previousTime = micros();
    getAccel_Data();
    Serial.print(previousTime/1000, DEC);
    Serial.print(", ");
    Serial.print(Axyz[0],6);
    Serial.print(", ");
    Serial.print(Axyz[1],6);
    Serial.print(", ");
    Serial.println(Axyz[2],6);
  }
}
//시간오차 interrupt 적용해보기.
//sleep 도 적용해보기.
//데이터처리
//porting
//stalta 알고리즘?
void getAccel_Data(void)
{
accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
Axyz[0] = (double) ax / 16384;
Axyz[1] = (double) ay / 16384;
Axyz[2] = (double) az / 16384;
}
