/*
WongyuLee
2015118466
hanhb0@naver.com
update : using interrupt 
참고:http://gammon.com.au/interrupts (interrupt)
http://www.instructables.com/id/Arduino-Timer-Interrupts/ (Timerinterrupt)
pin연결 
arduino     sensor
3.3v        vdd
gnd         gnd
sda20       sda
scl21       scl
arduino due cpu clock = 84MHz
Timer counter  = 1/84,000,000
1ms = 1/1,000s
84000 counter = 1ms
168000 counter = 2ms
*/
#include "Wire.h"
#define MPU9250_WHO_AM_I                0x75
#define WHOAMI_DEFAULT_VAL              0x68
#define MPU9250_GYRO_FULL_SCALE_250DPS  0
#define MPU9250_GYRO_CONFIG             0x1B
#define MPU9250_GYRO_FS_SEL_MASK        0x18
#define MPU9250_FULL_SCALE_8G 2
#define MPU9250_PWR_MGMT_1 0x6B
#define MPU9250_PWR_MGMT_2 0x6C
#define MPU9250_SLEEP_MASK 0x40
#define MPU9250_CLKSEL_MASK 0x07
#define MPU9250_ACCEL_CONFIG 0x1C
#define MPU9250_ACCEL_FS_SEL_MASK 0x18
#define MPU9250_ACCEL_XOUT_H 0x3B
#define MPU9250_ACCEL_XOUT_L 0x3C
#define MPU9250_ACCEL_YOUT_H 0x3D
#define MPU9250_ACCEL_YOUT_L 0x3E
#define MPU9250_ACCEL_ZOUT_H 0x3F
#define MPU9250_ACCEL_ZOUT_L 0x40
#define MPU9250_DISABLE_XA_MASK 0x20
#define MPU9250_DISABLE_YA_MASK 0x10
#define MPU9250_DISABLE_ZA_MASK 0x08

uint8_t _address;
uint8_t _whoami;
volatile float Axyz[3];
volatile int16_t ax, ay, az;

//interrupt variable
unsigned long time_interval = 84000*16; // 1ms = 84,000 (16ms)
volatile unsigned long previousTime = 16;
volatile unsigned long count = 0; // 1/84,000,000 초마다 1씩 증가
volatile char overflowing = 0;
volatile unsigned long time_stamp = 0;
volatile boolean started = false;
volatile unsigned long startTime;
volatile unsigned long endTime;

ISR(TIMER1_COMPA_vect)
{
  overflow();
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  setClockSource(0);
  setFullScaleGyroRange(MPU9250_GYRO_FULL_SCALE_250DPS);
  setFullScaleAccelRange(MPU9250_FULL_SCALE_8G);
  setSleepEnabled(false);
  Serial.println(testConnection() ? "MPU9250 connection successful" : "MPU9250 connection failed");
}

void loop() {
  interrupts();
  noInterrupts();
  Serial.print(time_stamp);
  Serial.print(", ");
  Serial.print(Axyz[0],6);
  Serial.print(", ");
  Serial.print(Axyz[1],6);
  Serial.print(", ");
  Serial.print(Axyz[2],6);
  Serial.print(", ");
  Serial.println("end");
}
//
uint8_t getDeviceID(void) {
    _whoami = readRegister(MPU9250_WHO_AM_I);
    return _whoami;
}
bool testConnection(void) {
    return getDeviceID() == WHOAMI_DEFAULT_VAL;
}
//
bool setFullScaleGyroRange(const uint8_t range) {
    return writeMaskedRegister(MPU9250_GYRO_CONFIG, MPU9250_GYRO_FS_SEL_MASK, range); //MPU9250_GCONFIG_FS_SEL_BIT, MPU9250_GCONFIG_FS_SEL_LENGTH, range);
}
bool writeRegister(const uint8_t register_addr, const uint8_t value) {
    //send write call to sensor address
    //send register address to sensor
    //send value to register
    bool write_status = 0;
    Wire.beginTransmission(_address); //open communication with 
    Wire.write(register_addr);  
    Wire.write(value); 
    Wire.endTransmission(); 

    return write_status; //returns whether the write succeeded or failed
}
bool setSleepEnabled(const bool enabled) {
    // Set X-axis accelerometer standby enabled status.
    return writeMaskedRegister(MPU9250_PWR_MGMT_1, MPU9250_SLEEP_MASK, enabled);
}
bool setClockSource(const uint8_t source) {
    if(source > 7) {
        return 0;
    }
    return writeMaskedRegister(MPU9250_PWR_MGMT_1, MPU9250_CLKSEL_MASK, source);
}
bool setFullScaleAccelRange(const uint8_t range) {
    if(range < 3){
        return 0;
    } else {
        return writeMaskedRegister(MPU9250_ACCEL_CONFIG, MPU9250_ACCEL_FS_SEL_MASK, range);
    }
}

bool writeMaskedRegister(const uint8_t register_addr, const uint8_t mask, const uint8_t value) {
    uint8_t masked_value = (mask & value);
    uint8_t regvalue = readRegister(register_addr);
    regvalue = regvalue & ~mask; // Zero Mask bits
    regvalue = regvalue | masked_value; //Set Mask value
    return writeRegister(register_addr, regvalue);
}


void getAccel_Data(void)
{
  getAcceleration(&ax,&ay,&az);
  Axyz[0] = (double) ax / 16384;
  Axyz[1] = (double) ay / 16384;
  Axyz[2] = (double) az / 16384;
  started = false;
}
uint8_t readRegister(const uint8_t register_addr) {
    uint8_t data = 0;
    Wire.beginTransmission(_address); 
    Wire.write(register_addr); 
    Wire.endTransmission(); 
    Wire.requestFrom((int)_address, 1);
    while(Wire.available()) {
        data = Wire.read();    // receive a byte as character
    }
    return data; //return the data returned from the register
}

uint16_t readRegisters(const uint8_t msb_register, const uint8_t lsb_register) {
    uint8_t msb = readRegister(msb_register);
    uint8_t lsb = readRegister(lsb_register);
    return (((int16_t)msb) << 8) | lsb;

}

uint8_t readMaskedRegister(const uint8_t register_addr, const uint8_t mask) {
    uint8_t data = readRegister(register_addr);
    return (data & mask);
}

bool accelerometerXIsEnabled(void) {
    uint8_t status = readMaskedRegister(MPU9250_PWR_MGMT_2, MPU9250_DISABLE_XA_MASK); //these need to be defined
    return (status != 0);
}

bool accelerometerYIsEnabled(void) {
    uint8_t status = readMaskedRegister(MPU9250_PWR_MGMT_2, MPU9250_DISABLE_YA_MASK); //these need to be defined
    return (status != 0);
}

bool accelerometerZIsEnabled(void) {
    uint8_t status = readMaskedRegister(MPU9250_PWR_MGMT_2, MPU9250_DISABLE_ZA_MASK); //these need to be defined
    return (status != 0);
}

int16_t getAccelerationX(void)
{
   if(accelerometerXIsEnabled()){
        return (int16_t) readRegisters(MPU9250_ACCEL_XOUT_H, MPU9250_ACCEL_XOUT_L);
    } else {
        return 0;
    }
}

int16_t getAccelerationY(void) {
    if(accelerometerYIsEnabled()){
        return (int16_t) readRegisters(MPU9250_ACCEL_YOUT_H, MPU9250_ACCEL_YOUT_L);
    } else {
        return 0;
    }
}

int16_t getAccelerationZ(void) {
    if(accelerometerZIsEnabled()){
        return (int16_t) readRegisters(MPU9250_ACCEL_ZOUT_H, MPU9250_ACCEL_ZOUT_L);
    } else {
        return 0;
    }
}

void getAcceleration(volatile int16_t* ax,volatile int16_t* ay,volatile int16_t* az) {
    *ax = getAccelerationX();
    *ay = getAccelerationY();
    *az = getAccelerationZ();
}
void overflow()
{
   count += 1;
  if (count >= time_interval && !overflowing) {
    overflowing = 1;
    count = 0;
    getAccel_Data();
    overflowing = 0;
  }
}



