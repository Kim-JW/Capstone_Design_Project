/*
  WongyuLee
  2015118466
  hanhb0@naver.com
  update : get acceleration data from arduino due
  pin연결
  arduino     sensor
  3.3v        vdd
  gnd         gnd
  sda20       sda
  scl21       scl
*/
#include "Wire.h"
enum MMA8451Q_Register {
 MMA8451_WHO_AM_I = 0x0D,//
 WHOAMI_DEFAULT_VAL = 0x2A,//
 MMA8451_ACCEL_CONFIG = 0x2A,//
 MMA8451_ACCEL_XOUT_H = 0x01,//
 MMA8451_ACCEL_XOUT_L = 0x02,//
 MMA8451_ACCEL_YOUT_H = 0x03,//
 MMA8451_ACCEL_YOUT_L = 0x04,//
 MMA8451_ACCEL_ZOUT_H = 0x05,//
 MMA8451_ACCEL_ZOUT_L = 0x06,//
 MMA8451_ADDRESS = 0x1C//
};
#define SCALE_2G  2 
#define SCALE_4G  4 
#define SCALE_8G  8
byte _address = MMA8451_ADDRESS;
byte _whoami;
uint8_t Config = 1;
float Axyz[3];
short x,y,z;
float ax, ay, az;
byte len = 1;
unsigned long currentTime = 0;
unsigned long previousTime = 0;
unsigned long time_interval = 20000; //(microsecond) = 20millisecond
unsigned long check = 0;
void setup() {
  Serial.begin(9600);
  Wire.begin();
  standby();
  active();
  Wire.endTransmission();
  //Serial.println("whoami");
 // Serial.println(_whoami);
  //Serial.println(testConnection()? "MMA8451 connection successful" : "MMA8451 connection failed");

}

void loop() {
  currentTime = micros();
  if (currentTime - previousTime >= time_interval) {
    getAccel_Data();
    Serial.print(currentTime / 1000, DEC);
    Serial.print(", ");
    Serial.print(Axyz[0], 6);
    Serial.print(", ");
    Serial.print(Axyz[1], 6);
    Serial.print(", ");
    Serial.println(Axyz[2], 6);
   // Serial.print(", ");
    //Serial.println(currentTime / 1000 - previousTime / 1000, DEC);
    previousTime = currentTime;
  }
}

//
uint8_t getDeviceID(void) {
  _whoami = readRegister(MMA8451_WHO_AM_I);
  return _whoami;
}
bool testConnection(void) {
  getDeviceID();
  if(_whoami != 0x2A)
  return 0;
  
  return 1;
}
//.
void standby()
{
  byte c = readRegister(MMA8451_ACCEL_CONFIG);
  writeRegister(MMA8451_ACCEL_CONFIG, c & ~(0x01)); //Clear the active bit to go into standby
}

void active()
{
  byte c = readRegister(MMA8451_ACCEL_CONFIG);
  writeRegister(MMA8451_ACCEL_CONFIG, c | 0x01); //Set the active bit to begin detection
}

void writeRegister(MMA8451Q_Register reg, byte data)
{
  writeRegisters(reg, &data, 1);
}

void writeRegisters(MMA8451Q_Register reg, byte *buffer, byte len)
{
  Wire.beginTransmission(MMA8451_ADDRESS);
  Wire.write(reg);
  for (int x = 0; x < len; x++)
    Wire.write(buffer[x]);
  Wire.endTransmission(); //Stop transmitting
}
byte readRegister(MMA8451Q_Register register_addr) {
  Wire.beginTransmission(_address);
  Wire.write(register_addr);
  Wire.endTransmission(false);
  Wire.requestFrom(_address, 1);
  if(Wire.available()){ //Wait for the data to come back
    return Wire.read(); //Return this one byte
  }
  else{
    return 0;
  }
}

void readRegisters(MMA8451Q_Register reg, byte *buffer, byte len) {
      #ifdef _VARIANT_ARDUINO_DUE_X_
       Wire.requestFrom((uint8_t) _address, (uint8_t) len,(uint32_t) reg, (uint8_t) 1,true);
      #else
      Wire.beginTransmission(_address);
      Wire.write(reg);
      Wire.endTransmission(false);
      Wire.requestFrom(_address, leg);
      #endif
      if(Wire.available() == len){
        for(int x = 0 ; x < len ; x++)
          buffer[x] = Wire.read();
       }
 /*
  uint8_t msb = readRegister(msb_register);
  uint8_t lsb = readRegister(lsb_register);
  lsb >>= 4;
  return (((uint16_t)msb) << 4) | (uint16_t)lsb;
*/
}

void getAccel_Data()
{
  byte rawData[6];  // x/y/z accel register data stored here

  readRegisters(MMA8451_ACCEL_XOUT_H, rawData, 6);  // Read the six raw data registers into data array

  x = ((short)(rawData[0]<<8 | rawData[1])) >> 2;
  y = ((short)(rawData[2]<<8 | rawData[3])) >> 2;
  z = ((short)(rawData[4]<<8 | rawData[5])) >> 2;
  Axyz[0] = (float) x / (float)(1<<13) * (float)(SCALE_2G);
  Axyz[1] = (float) y / (float)(1<<13) * (float)(SCALE_2G);
  Axyz[2] = (float) z / (float)(1<<13) * (float)(SCALE_2G);
}

