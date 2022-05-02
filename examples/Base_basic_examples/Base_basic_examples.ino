#include "Wire.h"
#include <Base.h>

Base base;

void setup() {
  Serial.begin(115200);
  base.begin();
  base.currentOffset(-1.48);

}

void loop() {
  Serial.print("Monitor: ");
  Serial.print(base.busVoltage(),4);
  Serial.print(" ");
  Serial.print(base.shuntCurrent(),4);
  Serial.print(" ");
  Serial.println(base.busPower(),4);
  delay(100);

}
