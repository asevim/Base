
#include "Base.h"

namespace{
const uint8_t RST =	15;
const uint8_t BRNG = 13;
const uint8_t PG_1 = 12;
const uint8_t PG_0 = 11;
const uint8_t BADC4 = 10;
const uint8_t BADC3	= 9;
const uint8_t BADC2	= 8;
const uint8_t BADC1	= 7;
const uint8_t SADC4	= 6;
const uint8_t SADC3	= 5;
const uint8_t SADC2	= 4;
const uint8_t SADC1	= 3;
const uint8_t MODE3	= 2;
const uint8_t MODE2	= 1;
const uint8_t MODE1	= 0;
};

#define CNVR_B 1
#define OVF_B  0

void Base::begin() {
    Wire.begin();
    configure();
    calibrate();
}

void Base::calibrate(float shunt_val, float v_shunt_max, float v_bus_max, float i_max_expected) {

    r_shunt = shunt_val;

    current_lsb = i_max_expected / 32767;
    cal = trunc((0.04096)/(current_lsb*r_shunt));
    power_lsb = current_lsb * 20;

    write16(CAL_R, cal);
}

void Base::configure(  t_range range,  t_gain gain,  t_adc  bus_adc,  t_adc shunt_adc,  t_mode mode) {
  config = 0;

  config |= (range << BRNG | gain << PG_0 | bus_adc << BADC1 | shunt_adc << SADC1 | mode);

  write16(CONFIG_R, config);
}

void Base::reset(){
  write16(CONFIG_R, 0xFFFF);
  delay(5);
}

void Base::currentOffset(float _offset) {
  _currentOffset = _offset;
}

int16_t Base::shuntVoltageRaw() const {
  return read16(V_SHUNT_R);
}

float Base::shuntVoltage() const {
  float temp;
  temp = read16(V_SHUNT_R);
  return (temp / 100000);
}

uint16_t Base::busVoltageRaw() {
  uint16_t bus_voltage_register = read16(V_BUS_R);
  _overflow = bitRead(bus_voltage_register, OVF_B);
  _ready    = bitRead(bus_voltage_register, CNVR_B);
  return bus_voltage_register;
}

float Base::busVoltage() {
  uint16_t temp;
  temp = busVoltageRaw();
  temp >>= 3;
  return (temp * 0.004);
}

int16_t Base::shuntCurrentRaw() const {
  return (read16(I_SHUNT_R));
}

float Base::shuntCurrent(){
  return (read16(I_SHUNT_R) * current_lsb + _currentOffset);
}

float Base::busPower(){
  return (read16(P_BUS_R) * power_lsb + (_currentOffset * busVoltage()));
}

void Base::reconfig() const {
  write16(CONFIG_R, config);
}

void Base::recalibrate() const {
  write16(CAL_R, cal);
}

bool Base::ready() const {
  return _ready;
}

bool Base::overflow() const {
  return _overflow;
}

void Base::write16(t_reg a, uint16_t d) const {
  uint8_t temp;
  temp = (uint8_t)d;
  d >>= 8;
  Wire.beginTransmission(I2C_ADDR);

  Wire.write(a);
  Wire.write((uint8_t)d);
  Wire.write(temp);

  Wire.endTransmission();
  delay(1);
}

int16_t Base::read16(t_reg a) const {
  uint16_t ret;

  write16(a, 0);
  
  Wire.requestFrom(I2C_ADDR, 2);
    ret = Wire.read();
    ret <<= 8;
    ret |= Wire.read();
  return ret;
}