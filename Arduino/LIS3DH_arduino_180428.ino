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
#define LIS3DH_WHO_AM_I                0x0F//
#define WHOAMI_DEFAULT_VAL              0x33//
#define DEFAULT_CONFIG 0x47//
#define LIS3DH_ACCEL_CONFIG 0x20//
#define LIS3DH_ACCEL_XOUT_H 0x28//
#define LIS3DH_ACCEL_XOUT_L 0x29//
#define LIS3DH_ACCEL_YOUT_H 0x2A//
#define LIS3DH_ACCEL_YOUT_L 0x2B//
#define LIS3DH_ACCEL_ZOUT_H 0x2C//
#define LIS3DH_ACCEL_ZOUT_L 0x2D//
#define LIS3DH_ADDRESS 0x19//
uint8_t _address;
uint8_t _whoami;
uint8_t Config= DEFAULT_CONFIG;
float Axyz[3];
int16_t ax, ay, az;
unsigned long time_stamp = 16;
unsigned long currentTime = 0;
unsigned long previousTime = 0;
unsigned long time_interval = 20000; //(microsecond) = 20millisecond
unsigned long check =0;
void setup() {
  Serial.begin(9600);
  Wire.begin();
   Wire.beginTransmission(LIS3DH_ADDRESS); //open communication with 
    Wire.write(LIS3DH_ACCEL_CONFIG);  
    Wire.write(Config); 
    Wire.endTransmission(); 
  Serial.println(testConnection() ? "LIS3DH connection successful" : "LIS3DH connection failed");

}

void loop() {
  currentTime = micros();
  if(currentTime - previousTime >= time_interval){
    getAccel_Data();
    Serial.print(currentTime/1000, DEC);
    Serial.print(", ");
    Serial.print(Axyz[0],6);
    Serial.print(", ");
    Serial.print(Axyz[1],6);
    Serial.print(", ");
    Serial.print(Axyz[2],6);
    Serial.print(", ");
    Serial.println(currentTime/1000-previousTime/1000, DEC);
    previousTime = currentTime;
  }
}

//
uint8_t getDeviceID(void) {
    _whoami = readRegister(LIS3DH_WHO_AM_I);
    return _whoami;
}
bool testConnection(void) {
    return getDeviceID() == WHOAMI_DEFAULT_VAL;
}
//
bool writeRegister(const uint8_t register_addr, const uint8_t value) {
    //send write call to sensor address
    //send register address to sensor
    //send value to register
    bool write_status = 0;
    Wire.beginTransmission(LIS3DH_ADDRESS); //open communication with 
    Wire.write(register_addr);  
    Wire.write(value); 
    Wire.endTransmission(); 

    return write_status; //returns whether the write succeeded or failed
}

void getAccel_Data(void)
{
  getAcceleration(&ax,&ay,&az);
  Axyz[0] = (float) ax / 16384;
  Axyz[1] = (float) ay / 16384;
  Axyz[2] = (float) az / 16384;
}
uint8_t readRegister(const uint8_t register_addr) {
    uint8_t data = 0;
    Wire.beginTransmission(LIS3DH_ADDRESS); 
    Wire.write(register_addr); 
    Wire.endTransmission(); 
    Wire.requestFrom((int)LIS3DH_ADDRESS, 1);
    while(Wire.available()) {
        data = Wire.read();    // receive a byte as character
    }
    return data; //return the data returned from the register
}

uint16_t readRegisters(const uint8_t msb_register, const uint8_t lsb_register) {
    uint8_t msb = readRegister(msb_register);
    uint8_t lsb = readRegister(lsb_register);
    return (((uint16_t)msb) << 8) | (uint16_t)lsb;

}

int16_t getAccelerationX(void)
{
   //if(accelerometerXIsEnabled()){
        return readRegisters(LIS3DH_ACCEL_XOUT_H, LIS3DH_ACCEL_XOUT_L);
    //} else {
    //    return 0;
    //}
}

int16_t getAccelerationY(void) {
    //if(accelerometerYIsEnabled()){
        return readRegisters(LIS3DH_ACCEL_YOUT_H, LIS3DH_ACCEL_YOUT_L);
    //} else {
     //   return 0;
    //}
}

int16_t getAccelerationZ(void) {
    //if(accelerometerZIsEnabled()){
        return readRegisters(LIS3DH_ACCEL_ZOUT_H, LIS3DH_ACCEL_ZOUT_L);
    //} else {
      //  return 0;
    //}
}

void getAcceleration(int16_t* ax, int16_t* ay, int16_t* az) {
    *ax = getAccelerationX();
    *ay = getAccelerationY();
    *az = getAccelerationZ();
}

