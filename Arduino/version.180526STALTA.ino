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
#define MPU9250_WHO_AM_I                0x75
#define WHOAMI_DEFAULT_VAL              0x71
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
#define MPU9250_ADDRESS 0x68
uint8_t _address;
uint8_t _whoami;
float Axyz[3];
int16_t ax, ay, az;
unsigned long time_stamp = 16;
unsigned long currentTime = 0;
unsigned long previousTime = 0;
unsigned long time_interval = 20000; //(microsecond) = 20millisecond
unsigned long check =0;
float rX1;
float rY1;
float rZ1;
float staX1;
float staY1;
float staZ1;
     float ltaX1;
     float ltaY1;
     float ltaZ1;
     float accX1;
     float accY1;
     float accZ1;
     float valX1;
     float valY1;
     float valZ1;

    // STA/LTA variables
    float threshold;
    int   stanum, ltanum;
void setup() {
  Serial.begin(19200);
  Wire.begin();
  setClockSource(0);
  setFullScaleGyroRange(MPU9250_GYRO_FULL_SCALE_250DPS);
  setFullScaleAccelRange(MPU9250_FULL_SCALE_8G);
  setSleepEnabled(false);
  //Serial.println(testConnection() ? "MPU9250 connection successful" : "MPU9250 connection failed");
  accX1 = 0;
        accY1 = 0;
        accZ1 = 0;
        valX1 = 0;
        valY1 = 0;
        valZ1 = 0;
        rX1 = 0;
        rY1 = 0;
        rZ1 = 0;
        staX1 = 0.0001f;
        ltaX1 = 0.0001f;
        staX1 = 0.0f;
        ltaX1 = 0.0f;
        staY1 = 0.0001f;
        ltaY1 = 0.0001f;
        staY1 = 0.0f;
        ltaY1 = 0.0f;
        staZ1 = 0.0001f;
        ltaZ1 = 0.0001f;
        staZ1 = 0.0f;
        ltaZ1 = 0.0f;
        threshold = 5;
        stanum = 5;
        ltanum = 30;
}

void loop() {
  currentTime = micros();
  if(currentTime - previousTime >= time_interval){
    previousTime = currentTime;
    getAccel_Data();
    if(!checkTriggerThreshold(Axyz[0],Axyz[1],Axyz[2])){
       Serial.println("Detect!!");
       while(1);
    }
    Serial.print(currentTime/1000, DEC);
    Serial.print(", ");
    Serial.print(Axyz[0],6);
    Serial.print(", ");
    Serial.print(Axyz[1],6);
    Serial.print(", ");
    Serial.println(Axyz[2],6);
    //Serial.print(", ");
    //Serial.println(currentTime/1000-previousTime/1000, DEC);
    
   }
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
    Wire.beginTransmission(MPU9250_ADDRESS); //open communication with 
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
 // Serial.print(micros()/1000, DEC);
 // Serial.print(", ");
 // Serial.print(Axyz[0],6);
 // Serial.print(", ");
 // Serial.print(Axyz[1],6);
//  Serial.print(", ");
//  Serial.println(Axyz[2],6);
}
uint8_t readRegister(const uint8_t register_addr) {
    uint8_t data = 0;
    Wire.beginTransmission(MPU9250_ADDRESS); 
    Wire.write(register_addr); 
    Wire.endTransmission(); 
    Wire.requestFrom((int)MPU9250_ADDRESS, 1);
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
int16_t getAccelerationX(void)
{
        return (int16_t) readRegisters(MPU9250_ACCEL_XOUT_H, MPU9250_ACCEL_XOUT_L);
}

int16_t getAccelerationY(void) {
        return (int16_t) readRegisters(MPU9250_ACCEL_YOUT_H, MPU9250_ACCEL_YOUT_L);
}

int16_t getAccelerationZ(void) {
        return (int16_t) readRegisters(MPU9250_ACCEL_ZOUT_H, MPU9250_ACCEL_ZOUT_L);
}

void getAcceleration(int16_t* ax, int16_t* ay, int16_t* az) {
    *ax = getAccelerationX();
    *ay = getAccelerationY();
    *az = getAccelerationZ();
}

float* stalta_rt (float z0, float z1, float r1_local, float sta1_local, float lta1_local) {

        float alpsta, alplta, nsta, nlta, dr0, thp, cf0, tk;
        float temp[3];
        thp = 0.994f;
        tk = 3.0f;
        nsta = stanum;
        nlta = ltanum;

        alpsta = 1 - 1 / nsta;
        alplta = 1 - 1 / nlta;

        dr0 = z0 - z1;
        float r0_local = (thp * r1_local) + dr0;
        cf0 = r0_local * r0_local + tk * (dr0 * dr0);

        float sta0_local = alpsta * sta1_local + (1 - alpsta) * cf0;
        float lta0_local = alplta * lta1_local + (1 - alplta) * cf0;
        temp[0] = sta0_local;
        temp[1] = lta0_local;
        temp[2] = r0_local;
        return temp;
    }

   bool checkTriggerThreshold(float valX0 , float valY0, float valZ0 ) {
        // High pass filter data
        float accX0 = toground_rt(valX0, valX1, accX1);
        float accY0 = toground_rt(valY0, valY1, accY1);
        float accZ0 = toground_rt(valZ0, valZ1, accZ1);

        // get the STA, LTA value from the function
        float* xtemp = stalta_rt(accX0, accX1, rX1, staX1, ltaX1);
        float* ytemp = stalta_rt(accY0, accY1, rY1, staY1, ltaY1);
        float* ztemp = stalta_rt(accZ0, accZ1, rZ1, staZ1, ltaZ1);

        // Get the current STA, LTA value
        float staX0 = xtemp[0];
        float staY0 = ytemp[0];
        float staZ0 = ztemp[0];
        float ltaX0 = xtemp[1];
        float ltaY0 = ytemp[1];
        float ltaZ0 = ztemp[1];
        float rX0 = xtemp[2];
        float rY0 = ytemp[2];
        float rZ0 = ztemp[2];

        // Assign back of the current value
        valX1 = valX0;
        valY1 = valY0;
        valZ1 = valZ0;

        accX1 = accX0;
        accY1 = accY0;
        accZ1 = accZ0;

        rX1 = rX0;
        staX1 = staX0;
        ltaX1 = ltaX0;
        rY1 = rY0;
        staY1 = staY0;
        ltaY1 = ltaY0;
        rZ1 = rZ0;
        staZ1 = staZ0;
        ltaZ1 = ltaZ0;

        // Calculate the STA/LTA ratio at this data point
        float ratioX = staX0 / ltaX0;
        float ratioY = staY0 / ltaY0;
        float ratioZ = staZ0 / ltaZ0;
        
        // Determine if there's a trigger:
        if (ratioX > threshold || ratioY > threshold || ratioZ > threshold) {
          Serial.println("ration X Y Z");
          Serial.println(ratioX);
          Serial.println(ratioY);
          Serial.println(ratioZ);
            return true;
        } else {
            return false;
        }
    }
   float toground_rt(float x0, float x1, float a1) {
        float q = 0.8f;
        float b0 = (float) (2.0 / (1 + q));
        return (x0 - x1) / (b0) + q * a1;
    }
