/*
Example script reading measurement values from the HTM502 sensor via I2C interface.

Copyright 2023 E+E Elektronik Ges.m.b.H.

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

#include <htm502I2c.h>
#include <Wire.h>
htm502I2c htm(0x40);
#define REQUEST_INTERVAL_MS 2000 //Time interval between measurements in ms
#define CSV_DELIMITER ','

bool measurementReady;
float temperature, humidity, measurementTime;
unsigned char identification[8];
char errorString[200];
uint8_t errorcode;

void setup()
{
  Serial.begin(9600); //Start Serial communication
  Wire.begin();       //initialize I2C peripheral (SDA..A4, SCL..A5)
  delay(1000);
  errorcode = htm.readIdentification(identification);
  if (errorcode != 0)
  {
    htm.getErrorString(errorcode, errorString);
    Serial.println(errorString);
  }
  else
  {
    Serial.print("Identificationnumber: ");
    for (int i = 0; i < 8; i++)
    {
      Serial.print(identification[i] < 16 ? "0" : "");
      Serial.print(identification[i], HEX);
    }
  }
  Serial.println("");
  htm.startPeriodicMeasurement();
  delay(2000);
  Serial.print("temperature");
  Serial.print(CSV_DELIMITER);
  Serial.println(" relative humidity");
  delay(1000);
}

void loop()
{
  errorcode = htm.getPeriodicMeasurementTempHum(temperature, humidity);
  if (errorcode != 0)
  {
    htm.getErrorString(errorcode, errorString);
    Serial.println(errorString);
  }
  else
  {
    Serial.print(temperature);
    Serial.print(" °C");
    Serial.print(CSV_DELIMITER);
    Serial.print(humidity);
    Serial.println(" %RH");
  }
  delay(REQUEST_INTERVAL_MS); //time until the next loop start
}
