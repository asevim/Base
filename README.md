# Base
 Arduino library for the Degz Base module
 
![6S-BASE](https://user-images.githubusercontent.com/45417323/166447315-1acc6923-9325-41ed-b268-c1b2426ef63f.jpeg)

## Specification

- Input Voltage : 12 - 26 VDC

- Continuous Current : 100 A

- At least two more 5A mosfet switch (M1 - M2)

- HALL sensor switch

## Pin Map

### 6 pin JST-SH connector for switch and I2c

| Base JST Pin  | MCU pin |
| ------------- | ------------- |
| 3v3  | 5V or 3v3 power supply  |
| GND | GND  |
| SDA | MCU SDA pin  |
| SCL | MCU SCL pin  |
| S2 | MCU any IO pin to switch M2  |
| S1 | MCU any IO pin to switch M1  |

### 3 pin JST-SH connector for HALL sensor

| Base JST Pin  | HALL sensor |
| ------------- | ------------- |
| H1  | VCC  |
| H2 | OUT  |
| H3 | GND  |
