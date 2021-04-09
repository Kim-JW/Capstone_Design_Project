
#include "Wire.h"
#include "detector.h"

volatile boolean led_on;
#define LIS3DH
#ifdef LIS3DH
#define REG_WHO_AM_I 0x0F
#define VAL_WHO_AM_I 0x33
#define REG_CONFIG 0x20
#define VAL_DEFAULT_CONFIG 0x47
#define REG_ACC_XOUT_H 0x29
#define REG_ACC_XOUT_L 0x28
#define REG_ACC_YOUT_H 0x2B
#define REG_ACC_YOUT_L 0x2A
#define REG_ACC_ZOUT_H 0x2D
#define REG_ACC_ZOUT_L 0x2C
#define DEVICE_ADDRESS 0x19
#endif

#define SAMPLING_RATE 50

uint8_t address;
uint8_t cfg = VAL_DEFAULT_CONFIG;

float acc[3];

int16_t ra, ry, rz;
unsigned long current_time = 0;
unsigned long tf = 0;
unsigned long previous_time = 0;
unsigned long theta = 20000; // 20millisecond

//detector d;
//reference url: http://ko7m.blogspot.kr/2015/01/arduino-due-timers-part-1.html

/////////////////////////////////////////////

uint8_t read_register(const uint8_t addr) {
  uint8_t data = 0;
  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(addr);
  Wire.endTransmission();
  Wire.requestFrom((int)DEVICE_ADDRESS, 1);
  while(Wire.available()) {
    data = Wire.read();
  }
  return data;
}

uint16_t read_registers(const uint8_t msb_addr, const uint8_t lsb_addr) {
  uint8_t msb = read_register(msb_addr);
  uint8_t lsb = read_register(lsb_addr);
  return ((uint16_t) msb << 8) | (uint16_t)lsb;
}

uint16_t get_rx() {
  return read_registers(REG_ACC_XOUT_H, REG_ACC_XOUT_L);
}

uint16_t get_ry() {
  return read_registers(REG_ACC_YOUT_H, REG_ACC_YOUT_L);
}

uint16_t get_rz() {
  return read_registers(REG_ACC_ZOUT_H, REG_ACC_ZOUT_L);
}

void get_ra(int16_t* x, int16_t* y, int16_t* z) {
  *x = get_rx();
  *y = get_ry();
  *z = get_rz();
}

void get_acc() {
  get_ra(&ra, &ry, &rz);
  acc[0] = (float) ra / 16384;
  acc[1] = (float) ry / 16384;
  acc[2] = (float) rz / 16384;
}

uint8_t get_device_id() {
  return read_register(REG_WHO_AM_I);
}

bool test_connection() {
  return get_device_id() == VAL_WHO_AM_I;
}
///////////////////////////////////////////////////

void TC3_Handler() {
  TC_GetStatus(TC1, 0);
  //digitalWrite(13, led_on = !led_on);
  current_time = millis();
  get_acc();
  Serial.print(current_time, DEC);
  Serial.print(", ");
  Serial.print(acc[0], 6);
  Serial.print(", ");
  Serial.print(acc[1], 6);
  Serial.print(", ");
  Serial.println(acc[2], 6);
  tf = millis();
  Serial.println(tf - current_time, DEC);
  previous_time = current_time;
}

void timer_start(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t freq) {
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk(irq);
  TC_Configure(tc, channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC |
                            TC_CMR_TCCLKS_TIMER_CLOCK4);
  uint32_t rc = VARIANT_MCK / 128 / freq;
  //TC_SetRA(tc, channel, rc >> 1);
  TC_SetRC(tc, channel, rc);
  TC_Start(tc, channel);
  tc->TC_CHANNEL[channel].TC_IER = TC_IER_CPCS;
  tc->TC_CHANNEL[channel].TC_IDR = ~TC_IER_CPCS;
  NVIC_EnableIRQ(irq);
}

void setup() {
  // put your setup code here, to run once:
  
  Wire.begin();
  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(REG_CONFIG);
  Wire.write(cfg);
  Wire.endTransmission();
  
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  timer_start(TC1, 0, TC3_IRQn, SAMPLING_RATE);
}

void loop() {
  // put your main code here, to run repeatedly:

}
