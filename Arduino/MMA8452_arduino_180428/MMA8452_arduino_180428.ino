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
#define MMA8452_WHO_AM_I                0x0D//
#define WHOAMI_DEFAULT_VAL              0x2A//
#define MMA8452_ACCEL_CONFIG 0x2A//
#define MMA8452_ACCEL_XOUT_H 0x01//
#define MMA8452_ACCEL_XOUT_L 0x02//
#define MMA8452_ACCEL_YOUT_H 0x03//
#define MMA8452_ACCEL_YOUT_L 0x04//
#define MMA8452_ACCEL_ZOUT_H 0x05//
#define MMA8452_ACCEL_ZOUT_L 0x06//
#define MMA8452_ADDRESS 0x1D//
uint8_t _address;
uint8_t _whoami;
uint8_t Config= 1;
float Axyz[3];
int16_t ax, ay, az;
unsigned long currentTime = 0;
unsigned long previousTime = 0;
unsigned long time_interval = 20000; //(microsecond) = 20millisecond
unsigned long check =0;
void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MMA8452_ADDRESS); //open communication with 
  Wire.write(MMA8452_ACCEL_CONFIG);  
  Wire.write(Config); 
  Wire.endTransmission(); 
  Serial.println(testConnection() ? "MMA8452 connection successful" : "MMA8452 connection failed");

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
    _whoami = readRegister(MMA8452_WHO_AM_I);
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
    Wire.beginTransmission(MMA8452_ADDRESS); //open communication with 
    Wire.write(register_addr);  
    Wire.write(value); 
    Wire.endTransmission(); 

    return write_status; //returns whether the write succeeded or failed
}

void getAccel_Data(void)
{
  getAcceleration(&ax,&ay,&az);
  Axyz[0] = (float) ax / 2048;
  Axyz[1] = (float) ay / 2048;
  Axyz[2] = (float) az / 2048;
}
uint8_t readRegister(const uint8_t register_addr) {
    uint8_t data = 0;
    Wire.beginTransmission(MMA8452_ADDRESS); 
    Wire.write(register_addr); 
    Wire.endTransmission(); 
    Wire.requestFrom((int)MMA8452_ADDRESS, 1);
    while(Wire.available()) {
      data = Wire.read();    // receive a byte as character
    }
    return data; //return the data returned from the register
}

uint16_t readRegisters(const uint8_t msb_register, const uint8_t lsb_register) {
    uint8_t msb = readRegister(msb_register);
    uint8_t lsb = readRegister(lsb_register);
    lsb >>=4;
    return (((uint16_t)msb) << 4) | (uint16_t)lsb;

}

int16_t getAccelerationX(void)
{
   int16_t val;
   val = readRegisters(MMA8452_ACCEL_XOUT_H, MMA8452_ACCEL_XOUT_L);
   val = val<<4;
   val = val>>4;
        return val;
}

int16_t getAccelerationY(void) {
  int16_t val;
  val = readRegisters(MMA8452_ACCEL_YOUT_H, MMA8452_ACCEL_YOUT_L);
  val = val<<4;
  val = val>>4;
        return val;
}

int16_t getAccelerationZ(void) {
  int16_t val;
  val = readRegisters(MMA8452_ACCEL_ZOUT_H, MMA8452_ACCEL_ZOUT_L);
  val = val<<4;
  val = val>>4;
        return val;
}

void getAcceleration(int16_t* ax, int16_t* ay, int16_t* az) {
    *ax = getAccelerationX();
    *ay = getAccelerationY();
    *az = getAccelerationZ();
}

