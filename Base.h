#ifndef Base_h_
#define Base_h_

#include "Arduino.h"
#include <Wire.h>

#define I2C_ADDR           0x40

#define D_SHUNT            0.0005
#define D_V_BUS_MAX        32
#define D_V_SHUNT_MAX      0.05
#define D_I_MAX_EXPECTED   100

class Base
{
    public:

    enum t_gain{
        GAIN_1_40MV = 0,
        GAIN_2_80MV = 1,
        GAIN_4_160MV = 2,
        GAIN_8_320MV = 3
    };

    enum t_range{
        RANGE_16V = 0, ///< Range 0-16 volts
        RANGE_32V = 1  ///< Range 0-32 volts
    };

    enum t_adc{
        ADC_9BIT    = 0,  ///<  9bit converion time  84us.
        ADC_10BIT   = 1,  ///< 10bit converion time 148us.
        ADC_11BIT   = 2,  ///< 11bit converion time 2766us.
        ADC_12BIT   = 3,  ///< 12bit converion time 532us.
        ADC_2SAMP   = 9,  ///< 2 samples converion time 1.06ms.
        ADC_4SAMP   = 10, ///< 4 samples converion time 2.13ms.
        ADC_8SAMP   = 11, ///< 8 samples converion time 4.26ms.
        ADC_16SAMP  = 12, ///< 16 samples converion time 8.51ms
        ADC_32SAMP  = 13, ///< 32 samples converion time 17.02ms.
        ADC_64SAMP  = 14, ///< 64 samples converion time 34.05ms.
        ADC_128SAMP = 15, ///< 128 samples converion time 68.10ms.
    };

     enum t_mode{
        PWR_DOWN    = 0,
        /*
        TRIG_SH     = 1,
        TRIG_BUS    = 2,
        TRIG_SH_BUS = 3,
        */
        ADC_OFF     = 4,
        CONT_SH     = 5, ///<Shunt Continuous.
        CONT_BUS    = 6, ///<Bus Continuous.
        CONT_SH_BUS = 7  ///<Shunt and Bus, Continuous.
    };

    void begin( );

    void calibrate( float r_shunt        = D_SHUNT,         ///< Value of shunt in Ohms.
    		        float v_shunt_max    = D_V_SHUNT_MAX,   ///< Maximum value of voltage across shunt.
    		        float v_bus_max      = D_V_BUS_MAX,     ///< Maximum voltage of bus.
    		        float i_max_expected = D_I_MAX_EXPECTED ///< Maximum current draw of bus + shunt.
    );

    void configure( t_range range   = RANGE_32V,   ///< Range for bus voltage.
    		        t_gain gain     = GAIN_2_80MV,///< Set Gain for shunt voltage.
    		        t_adc bus_adc   = ADC_12BIT,   ///< Configure bus voltage conversion.
    		        t_adc shunt_adc = ADC_12BIT,   ///< Configure shun voltage conversion.
    		        t_mode mode     = CONT_SH_BUS  ///< Sets operation mode.
    );

    void reset();

    void currentOffset(float _offset);

    int16_t shuntVoltageRaw() const;

    uint16_t busVoltageRaw();

    int16_t shuntCurrentRaw() const;

    float shuntVoltage() const;

    float busVoltage();

    float shuntCurrent();

    float busPower();
    
    void reconfig() const;

    void recalibrate() const;

    bool ready() const;

    bool overflow() const;

private:

    enum t_reg{
        CONFIG_R  = 0x00,    ///< configuration register.
        V_SHUNT_R = 0x01,    ///< differential shunt voltage.
        V_BUS_R   = 0x02,    ///< bus voltage (wrt to system/chip GND).
        P_BUS_R   = 0x03,    ///< system power draw (= V_BUS * I_SHUNT).
        I_SHUNT_R = 0x04,    ///< shunt current.
        CAL_R     = 0x05     ///< calibration register.
    };

    float r_shunt, current_lsb, power_lsb;
    uint16_t config, cal;
    bool _ready, _overflow;
    float _currentOffset;
    
    int16_t read16( t_reg addr ) const;

    void write16( t_reg addr, uint16_t data ) const;

};

#endif