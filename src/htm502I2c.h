/***************************************************************************/
/* headerfile for "htm502I2c.cpp" module */
/***************************************************************************/
/*
Read functions for measurement values of the HTM502 Sensor via I2C interface.

Copyright 2022 E+E Elektronik Ges.m.b.H.

Disclaimer:
This application example is non-binding and does not claim to be complete with regard
to configuration and equipment as well as all eventualities. The application example
is intended to provide assistance with the HTM502 sensor module design-in and is provided "as is".
You yourself are responsible for the proper operation of the products described.
This application example does not release you from the obligation to handle the product safely
during application, installation, operation and maintenance. By using this application example,
you acknowledge that we cannot be held liable for any damage beyond the liability regulations
described.

We reserve the right to make changes to this application example at any time without notice.
In case of discrepancies between the suggestions in this application example and other E+E
publications, such as catalogues, the content of the other documentation takes precedence.
We assume no liability for the information contained in this document.
*/

#ifndef htm502I2c_H
#define htm502I2c_H

#include "Arduino.h"
#include "Wire.h"

// Defines
//-----------------------------------------------------------------------------
#define CRC8_ONEWIRE_POLY                           0x31
#define CRC8_ONEWIRE_START                          0xFF
#define HTM502_COMMAND_READ_SINGLE_SHOT             0x2C1B
#define HTM502_COMMAND_READ_SINGLE_SHOT_DIS         0x241D //DIS = clock stretching disabled
#define HTM502_COMMAND_READ_PERIODIC_MEASUREMENT    0xE000
#define HTM502_COMMAND_CLEAR_REGISTER               0x3041
#define HTM502_COMMAND_READ_REGISTER                0xF32D
#define HTM502_COMMAND_START_PERIODIC_MEASUREMENT   0x201E
#define HTM502_COMMAND_END_PERIODIC_MEASUREMENT     0x3093
#define HTM502_COMMAND_SOFT_RESET                   0x30A2
#define HTM502_COMMAND_READ_IDENTIFICATION          0x7029


// declaration of functions
//-----------------------------------------------------------------------------

class htm502I2c
{
public:
    htm502I2c(void);
    htm502I2c(unsigned char i2cAdress);
    uint8_t singleShotTempHum(float &temperature, float &humidity);
    uint8_t singleShotTempHumClockStretchingDisabaled(float &temperature, float &humidity);
    uint8_t getPeriodicMeasurementTempHum(float &temperature, float &humidity);
    void startPeriodicMeasurement(void);
    void endPeriodicMeasurement(void);
    uint8_t readIdentification(unsigned char identification[]);
    uint8_t readStatusRegister(unsigned char statusRegister[]);
    void clearStatusregister(void);
    void reset(void);
    unsigned char address = 0x40;
    void wireWrite(unsigned char buf[], int to, bool stopmessage);
    void wireRead(unsigned char buf[], uint8_t to);
    unsigned char calcCrc8(unsigned char buf[], unsigned char from, unsigned char to);
    void getErrorString(uint8_t Status, char errorString[]);
};

#endif