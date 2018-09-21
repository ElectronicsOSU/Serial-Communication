//Pins used 2.1 -- SCL
//          2.2 -- SDL
//          GND -- GND
//          VCC -- VCC

#include <Wire.h>
#define button 5
#define LED 2

#define Aa 6
#define Bb 7
#define Cc 15
#define Dd 11


volatile int flag = HIGH;
int thermo_address = 72; //I2C Address of the device
int slave = 2;
byte read_temp_command = 0x00; //Command to be sent to the thermometer
void setup() {
  pinMode(Aa, OUTPUT);
  pinMode(Bb, OUTPUT);
  pinMode(Cc, OUTPUT);
  pinMode(Dd, OUTPUT);

  // put your setup code here, to run once:
  Serial.begin(9600); //Start Serial and output the news
  Serial.println("Serial Started");
  Wire.begin(); //Start wire library
  Serial.println("Wire Started");
  pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW);
  attachInterrupt(button,buttonpress,FALLING);
  flag = LOW;
}

void loop() {
  //Start Transmission
  Wire.beginTransmission(thermo_address);
  //Write the command to the thermometer
  Wire.write(read_temp_command);
  //End the transmission
  Wire.endTransmission();

  //Request 1 byte from the thermo_address
  Wire.requestFrom(thermo_address, 1);

  //While there is no info available
  while (Wire.available() == 0);

  //Temperature that is read in celsius
  int c = Wire.read(); 

  //Print info to the user
  Serial.print("Temperature: ");
  Serial.print(c);
  Serial.print("\n");

  int lastDigit = c%10;

  if(lastDigit == 0) {
    zero();
    delay(1000);
  }
  if(lastDigit == 1) {
    one(); 
        delay(1000);
  }
  if(lastDigit == 2) {
    two(); 
        delay(1000);
  }
  if(lastDigit == 3) {
    three(); 
        delay(1000);
  }
  if(lastDigit == 4) {
    four(); 
        delay(1000);
  }
  if(lastDigit == 5) {
    five(); 
        delay(1000);
  }
  if(lastDigit == 6) {
    six(); 
        delay(1000);
  }
  if(lastDigit == 7) {
    seven(); 
        delay(1000);
  }
  if(lastDigit == 8) {
    eight(); 
        delay(1000);
  }
  if(lastDigit == 9) {
    nine();
        delay(1000);
  }

  

  
  
  //Delay between readings
  delay(500);
  if(flag==HIGH)
  {
    Wire.beginTransmission(slave);
    Wire.write(2);
    Wire.endTransmission();
    digitalWrite(LED,LOW);
    flag = LOW;
  }
}
void buttonpress()
{
  digitalWrite(LED,HIGH);
  flag = HIGH;
}



void zero()
{
  digitalWrite(Aa, LOW);
  digitalWrite(Bb, LOW);
  digitalWrite(Cc, LOW);
  digitalWrite(Dd, LOW);
}

void one()
{
  digitalWrite(Aa, HIGH);
  digitalWrite(Bb, LOW);
  digitalWrite(Cc, LOW);
  digitalWrite(Dd, LOW);
}

void two()
{
  digitalWrite(Aa, LOW);
  digitalWrite(Bb, HIGH);
  digitalWrite(Cc, LOW);
  digitalWrite(Dd, LOW);
}

void three()
{
  digitalWrite(Aa, HIGH);
  digitalWrite(Bb, HIGH);
  digitalWrite(Cc, LOW);
  digitalWrite(Dd, LOW);
}

void four()
{
  digitalWrite(Aa, LOW);
  digitalWrite(Bb, LOW);
  digitalWrite(Cc, HIGH);
  digitalWrite(Dd, LOW);
}

void five()
{
  digitalWrite(Aa, HIGH);
  digitalWrite(Bb, LOW);
  digitalWrite(Cc, HIGH);
  digitalWrite(Dd, LOW);
}

void six()
{
  digitalWrite(Aa, LOW);
  digitalWrite(Bb, HIGH);
  digitalWrite(Cc, HIGH);
  digitalWrite(Dd, LOW);
}

void seven()
{
  digitalWrite(Aa, HIGH);
  digitalWrite(Bb, HIGH);
  digitalWrite(Cc, HIGH);
  digitalWrite(Dd, LOW);
}

void eight()
{
  digitalWrite(Aa, LOW);
  digitalWrite(Bb, LOW);
  digitalWrite(Cc, LOW);
  digitalWrite(Dd, HIGH);
}

void nine()
{
  digitalWrite(Aa, HIGH);
  digitalWrite(Bb, LOW);
  digitalWrite(Cc, LOW);
  digitalWrite(Dd, HIGH);
}








