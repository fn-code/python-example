/***************************************************************************

 ***************************************************************************/
#include "MPU9250.h"
#include "TinyGPS.h"
#include "SoftwareSerial.h"
#include "Arduino.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#define windPin 2 // Receive the data from sensor
#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11 
#define BMP_CS 10
//-------------------------------Gyro---------------------"
MPU9250 mpu = MPU9250();
int gyro_X, gyro_Y, gyro_Z;
//Adafruit_BMP280 bme(BMP_CS); // hardware SPI
//Adafruit_BMP280 bme(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);
Adafruit_BMP280 bme; // I2C
// Constants definitions Anemometer
const float pi = 3.14159265; // pi number
int period = 10000; // Measurement period (miliseconds)
int delaytime = 10000; // Time between samples (miliseconds)
int radio = 90; // Distance from center windmill to outer cup (mm)
int jml_celah = 18; // jumlah celah sensor

// Variable definitions
unsigned int Sample = 0; // Sample number
unsigned int counter = 0; // B/W counter for sensor
unsigned int RPM = 0; // Revolutions per minute
float speedwind = 0; // Wind speed (m/s)
static const unsigned long REFRESH_INTERVAL = 5000; // ms  
bool state = false;

long lat, lng;
SoftwareSerial serial_connection(11, 10); //RX=pin 10, TX=pin 11
TinyGPS gps;//This is the GPS object that will pretty much do all the grunt work with the NMEA data

void setup() {
  Serial.begin(115200);
  serial_connection.begin(9600);
  // Set the pins Anemometer
     pinMode(2, INPUT);
     digitalWrite(2, HIGH);
  //Serial.println(F("BMP280 test"));
     Wire.begin();
     uint8_t temp = mpu.begin();
  if (!bme.begin()) {  
//    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
  
  
}
  
void loop() {
//Sample++;
//Serial.print(Sample);


 
 if(serial_connection.available())//While there are characters to come from the GPS
  {
    if(gps.encode(serial_connection.read()))// encode gps data 
    {  
      gps.get_position(&lat,&lng); // get latitude and longitude
      float lats = lat/1000000.0;
      float lngs = lng/1000000.0;
      
      Serial.write(0x84);
      sendData(lats);
      sendData(lngs);

      state = true;
    }
  }

if(state) {
 Anemometer();
 gyro();
 state= false;
}

}


// -------------------gyro & baro-----------------------
void gyro()
{

//Accel
  mpu.set_accel_range(RANGE_4G);
  mpu.get_accel();
//  
//  Serial.println(F("--------------ACCELEROMETER-------------"));
//  Serial.print("X: ");  Serial.print(mpu.x);
//  Serial.print("\tY: "); Serial.print(mpu.y);
//  Serial.print("\t\tZ: "); Serial.print(mpu.z);
//  Serial.println();

  Serial.write(0x81);
  sendData(mpu.x);
  sendData(mpu.y);
  sendData(mpu.z);


  mpu.get_accel_g();
//  Serial.print("\t\tX_g: "); Serial.print(mpu.x_g, 2);
//  Serial.print("\tY_g: "); Serial.print(mpu.y_g, 2);
//  Serial.print("\tZ_g: "); Serial.print(mpu.z_g, 2);  Serial.println(" G");


  //Serial.println('\n');
  //Gyro
  mpu.set_gyro_range(RANGE_GYRO_250);
  mpu.get_gyro();
  gyro_X = mpu.gx;
  gyro_Y = mpu.gy;
  gyro_Z = mpu.gz;
  
//  Serial.println(F("GYROSCOPE"));
//  Serial.print("GX: ");  Serial.print(gyro_X);
//  Serial.print("\t\tGY: "); Serial.print(gyro_Y);
//  Serial.print("\t\tGZ: "); Serial.print(gyro_Z);

  sendData(gyro_X);
  sendData(gyro_Y);
  sendData(gyro_Z);

  mpu.get_gyro_d();
//  Serial.print("\t\tGX_g: "); Serial.print(mpu.gx_d, 2);
//  Serial.print("\tGY_g: "); Serial.print(mpu.gy_d, 2);
//  Serial.print("\tGZ_g: "); Serial.print(mpu.gz_d, 2); Serial.println(" º/s");
  //Serial.println('\n');

  sendData(mpu.gx_d);
  sendData(mpu.gy_d);
  sendData(mpu.gz_d);
  
  //Mag
  mpu.set_mag_scale(SCALE_14_BITS);
  mpu.set_mag_speed(MAG_8_Hz);
  if (!mpu.get_mag()) {
//    Serial.println(F("MAGNETOMETER"));
//    Serial.print("MX: ");  Serial.print(mpu.mx);
//    Serial.print("\t\tMY: "); Serial.print(mpu.my);
//    Serial.print("\t\tMZ: "); Serial.print(mpu.mz);

    mpu.get_mag_t();
//    Serial.print("\tMX_t: "); Serial.print(mpu.mx_t, 2);
//    Serial.print("\tMY_t: "); Serial.print(mpu.my_t, 2);
//    Serial.print("\tMZ_t: "); Serial.print(mpu.mz_t, 2); Serial.println(" uT");
   // Serial.println('\n');
  }
  else {
    // |X|+|Y|+|Z| must be < 4912μT to sensor measure correctly 
//    Serial.println("Overflow no magnetometro.");
  }
  // Temp     
//  Serial.print("Temperature is ");  Serial.print((((float)mpu.get_temp()) / 333.87 + 21.0), 1);  Serial.println(" degrees C");
//  Serial.println();

  //Serial.println('\n');
  //Serial.println('\n');
  //-------------------------------------------------
//    Serial.println(F("-----------BAROMETER-------------"));
//    Serial.print("Temperature = ");
//    Serial.print(bme.readTemperature());
//    Serial.println(" *C");
//    Serial.print("Pressure = ");
//    Serial.print(bme.readPressure());
//    Serial.println(" Pa");
//    Serial.print("Approx altitude = ");
//    Serial.print(bme.readAltitude(1013.25)); // this should be adjusted to your local forcase
//    Serial.println(" m");
//    
//    Serial.println();

  Serial.write(0x82);
  sendData(bme.readTemperature());
  sendData(bme.readPressure());
  sendData(bme.readAltitude(1013.25));
  

}


//----------------------------------------Anemometer()-------------------------
void Anemometer()
{
 windvelocity();
  //Serial.println(" finished.");
//   Serial.println(F("--------------ANEMOMETER-------------"));
//  Serial.print("Counter: ");
//  Serial.print(counter);
//  Serial.print("; RPM: ");
  RPMcalc();
  Serial.write(0x83);
  sendData(RPM);
//  Serial.print(RPM);
//  Serial.print("; Wind speed: ");
  WindSpeed();
  sendData(speedwind);
//  Serial.print(speedwind);
//  Serial.print(" [m/s]");
//  Serial.println("\n");
  
}
// Measure wind speed
void windvelocity()
{
speedwind = 0;
counter = 0;
attachInterrupt(0, addcount, CHANGE);
unsigned long millis();
long startTime = millis();
while(millis() < startTime + period) {}
  detachInterrupt(1);
}

void RPMcalc()
{
RPM=((counter/jml_celah)*60)/(period/1000); // Calculate revolutions per minute (RPM)
}

void WindSpeed()
{
speedwind = ((2 * pi * radio * RPM)/60) / 1000; // Calculate wind speed on m/s
}

void addcount()
{
counter++;
}

void sendData(float val) {
  byte *data = (byte *) &val;
  for (size_t i = 0; i < sizeof(val); i++) {
    Serial.write(data[i]);
  }
}
