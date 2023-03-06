/***************************************************************************/
/* sourcefile for "htm502I2c.h" module */
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

// Includes
//-----------------------------------------------------------------------------
#include "htm502I2c.h"
#include <Arduino.h>
#include "Wire.h"


enum Errorcode
{
    OKAY = 0,
    ERR_CKSUM = 1
};


htm502I2c::htm502I2c(void)
{
}

htm502I2c::htm502I2c(unsigned char i2cAdress)
{
  address = i2cAdress;
}

uint8_t htm502I2c::singleShotTempHum(float &temperature, float &humidity)
{
  unsigned char i2cResponse[6] = {};
  unsigned char Command[] = {(HTM502_COMMAND_READ_SINGLE_SHOT >> 8), (HTM502_COMMAND_READ_SINGLE_SHOT & 0xFF)};  wireWrite(Command, 1, false);
  delay(2);
  wireRead(i2cResponse, 6);
  if (i2cResponse[2] == calcCrc8(i2cResponse, 0, 1) && i2cResponse[5] == calcCrc8(i2cResponse, 3, 4))
  {
    temperature = ((float)(i2cResponse[0]) * 256 + i2cResponse[1]);
    if (temperature > 55536)
    {
      temperature = (temperature - 65536) / 100; 
    }
    else 
    {
      temperature = temperature / 100;
    }
    humidity = ((float)(i2cResponse[3]) * 256 + i2cResponse[4]) / 100;
    return 0;
  }
  else
  {
    return 1;
  }
}

uint8_t htm502I2c::singleShotTempHumClockStretchingDisabaled(float &temperature, float &humidity)
{
  unsigned char i2cResponse[6] = {};
  unsigned char Command[] = {(HTM502_COMMAND_READ_SINGLE_SHOT_DIS >> 8), (HTM502_COMMAND_READ_SINGLE_SHOT_DIS & 0xFF)};
  wireWrite(Command, 1, false);
  delay(2);
  wireRead(i2cResponse, 6);
  if (i2cResponse[2] == calcCrc8(i2cResponse, 0, 1) && i2cResponse[5] == calcCrc8(i2cResponse, 3, 4))
  {
    temperature = ((float)(i2cResponse[0]) * 256 + i2cResponse[1]);
    if (temperature > 55536)
    {
      temperature = (temperature - 65536) / 100; 
    }
    else 
    {
      temperature = temperature / 100;
    }
    humidity = ((float)(i2cResponse[3]) * 256 + i2cResponse[4]) / 100;
    return 0;
  }
  else
  {
    return 1;
  }
}

uint8_t htm502I2c::getPeriodicMeasurementTempHum(float &temperature, float &humidity)
{
  unsigned char i2cResponse[6] = {};
  unsigned char Command[] = {(HTM502_COMMAND_READ_PERIODIC_MEASUREMENT >> 8), (HTM502_COMMAND_READ_PERIODIC_MEASUREMENT & 0xFF)};
  wireWrite(Command, 1, false);
  wireRead(i2cResponse, 6);
  if (i2cResponse[2] == calcCrc8(i2cResponse, 0, 1) && i2cResponse[5] == calcCrc8(i2cResponse, 3, 4))
  {
    temperature = ((float)(i2cResponse[0]) * 256 + i2cResponse[1]);
    if (temperature > 55536)
    {
      temperature = (temperature - 65536) / 100; 
    }
    else 
    {
      temperature = temperature / 100;
    }
    humidity = ((float)(i2cResponse[3]) * 256 + i2cResponse[4]) / 100;
    return 0;
  }
  else
  {
    return 1;
  }
}

void htm502I2c::startPeriodicMeasurement(void)
{
  unsigned char Command[] = {(HTM502_COMMAND_START_PERIODIC_MEASUREMENT >> 8), (HTM502_COMMAND_START_PERIODIC_MEASUREMENT & 0xFF)};
  wireWrite(Command, 1, true);
}

void htm502I2c::endPeriodicMeasurement(void)
{
  unsigned char Command[] = {(HTM502_COMMAND_END_PERIODIC_MEASUREMENT >> 8), (HTM502_COMMAND_END_PERIODIC_MEASUREMENT & 0xFF)};
  wireWrite(Command, 1, true);
}

uint8_t htm502I2c::readIdentification(unsigned char identification[])
{
  unsigned char i2cResponse[9] = {};
  unsigned char Command[] = {(HTM502_COMMAND_READ_IDENTIFICATION >> 8), (HTM502_COMMAND_READ_IDENTIFICATION & 0xFF)};
  wireWrite(Command, 1, false);
  wireRead(i2cResponse, 9);
  if (i2cResponse[8] == calcCrc8(i2cResponse, 0, 7))
  {
    for (int i = 0; i < 8; i++)
    {
      identification[i] = i2cResponse[i];
    }
    return 0;
  }
  else
  {
    return 1;
  }
}

uint8_t htm502I2c::readStatusRegister(unsigned char statusRegister[])
{
  unsigned char i2cResponse[3] = {};
  unsigned char Command[] = {(HTM502_COMMAND_READ_REGISTER >> 8), (HTM502_COMMAND_READ_REGISTER & 0xFF)};
  wireWrite(Command, 1, false);
  wireRead(i2cResponse, 3);
  if (i2cResponse[2] == calcCrc8(i2cResponse, 0, 1))
  {
    for (int i = 0; i < 2; i++)
    {
      statusRegister[i] = i2cResponse[i];
    }
    return 0;
  }
  else
  {
    return 1;
  }
}

void htm502I2c::clearStatusregister(void)
{
  unsigned char Command[] = {(HTM502_COMMAND_CLEAR_REGISTER >> 8), (HTM502_COMMAND_CLEAR_REGISTER & 0xFF)};
  wireWrite(Command, 1, true);
}

void htm502I2c::reset(void)
{
  unsigned char Command[] = {(HTM502_COMMAND_SOFT_RESET >> 8), (HTM502_COMMAND_SOFT_RESET & 0xFF)};
  wireWrite(Command, 1, true);
}

void htm502I2c::wireWrite(unsigned char buf[], int to, bool stopmessage)
{
  Wire.beginTransmission(address);
  for (int i = 0; i <= to; i++)
  {
    Wire.write(buf[i]);
  }
  Wire.endTransmission(stopmessage);
}

void htm502I2c::wireRead(unsigned char buf[], uint8_t to)
{
  int i = 0;
  Wire.requestFrom(address, to);
  while (Wire.available())
  {
    buf[i++] = Wire.read();
  }
}

unsigned char htm502I2c::calcCrc8(unsigned char buf[], unsigned char from, unsigned char to)
{
  unsigned char crcVal = CRC8_ONEWIRE_START;
  unsigned char i = 0;
  unsigned char j = 0;
  for (i = from; i <= to; i++)
  {
    int curVal = buf[i];
    for (j = 0; j < 8; j++)
    {
      if (((crcVal ^ curVal) & 0x80) != 0) //If MSBs are not equal
      {
        crcVal = ((crcVal << 1) ^ CRC8_ONEWIRE_POLY);
      }
      else
      {
        crcVal = (crcVal << 1);
      }
      curVal = curVal << 1;
    }
  }
  return crcVal;
}

void htm502I2c::getErrorString(uint8_t Status, char errorString[])
{
  memset(errorString, '\0', sizeof(errorString));
  switch (Status)
  {
  case OKAY:
    strcpy(errorString, "Success");
    break;
  case ERR_CKSUM:
    strcpy(errorString, "Checksum error");
    break;
  default:
    strcpy(errorString, "unknown error");
    break;
  }
}