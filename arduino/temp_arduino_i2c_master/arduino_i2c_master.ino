/*
 * Simple demonstration of I2C
 * Connects to a temperature sensor (TC74A0)
 * Prints the readings to serial in Celsius and Farenheit
 * Datasheet for sensor: https://my.mouser.com/datasheet/2/268/21462c-73653.pdf
 */
#include <Wire.h>
#define THERMO_ADDRESS 72 //I2C Address of the device

#define READ_TEMP_COMMAND 0x00 //Command to be sent to the thermometer
void setup() {
  Serial.begin(115200);
  Serial.println("Serial Started");
  Wire.begin(); //Start wire library
  Serial.println("Wire Started");
}

void loop() {
  //Start Transmission
  Wire.beginTransmission(THERMO_ADDRESS);
  //Write the command to the thermometer
  Wire.write(READ_TEMP_COMMAND);
  //End the transmission
  Wire.endTransmission();

  //Request 1 byte from the thermo_address
  Wire.requestFrom(THERMO_ADDRESS, 1);

  //While there is no info available
  while (Wire.available() == 0);

  //Temperature that is read in celsius
  int c = Wire.read();

  //Print info to the user
  Serial.print("Temperature(C): ");
  Serial.print(c);
  Serial.print("\n");
  Serial.print("Temperature(F): ");
  Serial.print((c * 9 / 5) + 32);
  Serial.print("\n");

  //Delay between readings
  delay(500);

}
