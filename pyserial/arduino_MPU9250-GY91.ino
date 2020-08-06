//gy-91 mit Nokia Display gesteckt



#include <Wire.h>
#include <TimerOne.h>


float xmin, xmax, ymin, ymax;
int xm, ym;

float fax[12];
float fay[12];
float faz[12];
float dax, day, daz;

int index = 0;

float sax = 0; float say = 0; float saz = 0;
float fmx[12];
float fmy[12];
float fmz[12];
float dmx, dmy, dmz;

int indexm = 0;

float smx = 0; float smy = 0; float smz = 0;
float Xh, Yh;




#define    MPU9250_ADDRESS            0x68
#define    MAG_ADDRESS                0x0C

#define    GYRO_FULL_SCALE_250_DPS    0x00
#define    GYRO_FULL_SCALE_500_DPS    0x08
#define    GYRO_FULL_SCALE_1000_DPS   0x10
#define    GYRO_FULL_SCALE_2000_DPS   0x18

#define    ACC_FULL_SCALE_2_G        0x00
#define    ACC_FULL_SCALE_4_G        0x08
#define    ACC_FULL_SCALE_8_G        0x10
#define    ACC_FULL_SCALE_16_G       0x18



// This function read Nbytes bytes from I2C device at address Address.
// Put read bytes starting at register Register in the Data array.
void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data)
{
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.endTransmission();

  // Read Nbytes
  Wire.requestFrom(Address, Nbytes);
  uint8_t index = 0;
  while (Wire.available())
    Data[index++] = Wire.read();
}


// Write a byte (Data) in device (Address) at register (Register)
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data)
{
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.write(Data);
  Wire.endTransmission();
}



// Initial time
long int ti;
volatile bool intFlag = false;


//***************************************************************************************

void setup()
//*************************************************************************************
{


  xm = 84; ym = 48;
  xmin = -1.5;    xmax = 1.5   ;
  ymin = xmin / xm * ym * 1.2; ymax = xmax / xm * ym * 1.2;
  //ymin=-1.5;ymax=1.5;
  pinMode(12, OUTPUT);  digitalWrite(12, HIGH);     //  Vcc
  pinMode(13, OUTPUT);  digitalWrite(13, LOW);    //  low Beleuchtung ein ???


  Wire.begin();
  Serial.begin(9600);

  // Set accelerometers low pass filter at 5Hz
  I2CwriteByte(MPU9250_ADDRESS, 29, 0x06);
  // Set gyroscope low pass filter at 5Hz
  I2CwriteByte(MPU9250_ADDRESS, 26, 0x06);


  // Configure gyroscope range
  I2CwriteByte(MPU9250_ADDRESS, 27, GYRO_FULL_SCALE_1000_DPS);
  // Configure accelerometers range
  I2CwriteByte(MPU9250_ADDRESS, 28, ACC_FULL_SCALE_4_G);
  // Set by pass mode for the magnetometers
  I2CwriteByte(MPU9250_ADDRESS, 0x37, 0x02);

  // Request continuous magnetometer measurements in 16 bits
  I2CwriteByte(MAG_ADDRESS, 0x0A, 0x16);

 // pinMode(13, OUTPUT);     // war schon
// not correct
  Timer1.initialize(10000);         // initialize timer1, and set a 1/2  falsch!!!!!! second period
  Timer1.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt


  // Store initial time
  ti = millis();



  index = 0; indexm = 0;


  sax = 0; say = 0; saz = 0;    // sum for accellaration
  for (int i = 0; i <= 8; i++)
  {
    fax[i] = 0;                  // arrays for 8 values acceleration  init
    fay[i] = 0;
    faz[i] = 0;
  }

  smx = 0; smy = 0; smz = 0;    // sum for mag
  for (int i = 0; i <= 8; i++)
  {
    fmx[i] = 0;                  // arrays for 8 values mag  init
    fmy[i] = 0;
    fmz[i] = 0;
  }


 

  delay(2000);

}


float headingDegrees, headingFiltered, heading, declination;


// Counter
long int cpt = 0;

void callback()
{
  intFlag = true;
 // digitalWrite(13, digitalRead(13) ^ 1);  // schaltet die Beleuchtung des Displays
}

// Main loop, read and display data
//********************************************************************************************
void loop()
//********************************************************************************************
{
  // delay(100);
  // intFlag=true;


  while (!intFlag);
  intFlag = false;

  // Display time
 

  // _______________
  // ::: Counter :::

  // Display data counter
  //  Serial.print (cpt++,DEC);
  //  Serial.print ("\t");



  // ____________________________________
  // :::  accelerometer and gyroscope :::

  // Read accelerometer and gyroscope
  uint8_t Buf[14];
  I2Cread(MPU9250_ADDRESS, 0x3B, 14, Buf);

  // Create 16 bits values from 8 bits data

  // Accelerometer
  int16_t ax = -(Buf[0] << 8 | Buf[1]);
  int16_t ay = -(Buf[2] << 8 | Buf[3]);
  int16_t az = Buf[4] << 8 | Buf[5];


  int maxindex = 8;
  int maxindexm = 8;
  float roll;
  float pitch;

  sax = sax - fax[index];     // one value subtract
  fax[index] = ax / 8192.0;          // new value insert  Vorzeichen/sign!!!!!!!!!!!
  sax = sax + fax[index]; // oder ax           new sum
  dax = sax / maxindex;         // average



  say = say - fay[index];     // same for acc y
  fay[index] =- ay / 8192.0;
  say = say + fay[index]; // oder ay
  day = say / maxindex;


  saz = saz - faz[index];      // same for acc z , not used
  faz[index] = az / 8192.0;
  saz = saz + faz[index]; // oder az
  daz = saz / maxindex;


  index++;
  if (index >= maxindex) {        //   wrap index
    index = 0;
  }


  roll = asin(day);
  pitch = asin(dax); //

 // roll   = atan2(  day ,  sqrt(sq(dax)+sq(daz)));
 // pitch  = atan2(  dax ,  sqrt(sq(day)+sq(daz)));


  
 //  Serial.print(roll*180/PI, 5); Serial.print(" pitch "); Serial.println(pitch*180/PI, 5);
  /*
    Serial.println();
    Serial.println();
    Serial.println(roll);
    Serial.println(pitch);
    Serial.println();
  */
  
  float cosRoll = cos(roll);
  float sinRoll = sin(roll);
  float cosPitch = cos(pitch);
  float sinPitch = sin(pitch);


  // Gyroscope
  int16_t gx = -(Buf[8] << 8 | Buf[9]);
  int16_t gy = -(Buf[10] << 8 | Buf[11]);
  int16_t gz = Buf[12] << 8 | Buf[13];

  // Display values

  // Accelerometer
 /*
 Serial.print (ax);
  Serial.print ("\t");

  Serial.print (ay);
  Serial.print ("\t");
  Serial.print (az);
  Serial.print ("\t");
Serial.print("     Durch ");
  
  Serial.print (dax);
  Serial.print ("\t");

  Serial.print (day);
  Serial.print ("\t");
  Serial.print (daz);
  Serial.print ("\t");
*/
  /*

    // Gyroscope
    Serial.print (gx,DEC);
    Serial.print ("\t");
    Serial.print (gy,DEC);
    Serial.print ("\t");
    Serial.print (gz,DEC);
    Serial.print ("\t");

  */
  // _____________________
  // :::  Magnetometer :::


  // Read register Status 1 and wait for the DRDY: Data Ready

  uint8_t ST1;
  do
  {
    I2Cread(MAG_ADDRESS, 0x02, 1, &ST1);
  }
  while (!(ST1 & 0x01));

  // Read magnetometer data
  uint8_t Mag[7];
  I2Cread(MAG_ADDRESS, 0x03, 7, Mag);


  // Create 16 bits values from 8 bits data

  // Magnetometer
  int16_t mx = -(Mag[3] << 8 | Mag[2]);
  int16_t my = -(Mag[1] << 8 | Mag[0]);
  int16_t mz = -(Mag[5] << 8 | Mag[4]);

  
  mx = mx+220 ;//+220                   very important !!!!!!!!!!!!!!
  my = my - 60;  //-60      m-values have to be the same for both sides of measurements
  mz = mz + 170;    // 170


  smx = smx - fmx[indexm];     // one value subtract
  fmx[indexm] = mx;            // new value insert
  smx = smx + fmx[indexm]; // oder mx           new sum
  dmx = -smx / maxindexm;         // average



  smy = smy - fmy[indexm];     // same for mag y
  fmy[indexm] = my;
  smy = smy + fmy[indexm]; // oder my
  dmy = -smy / maxindexm;


  smz = smz - fmz[indexm];      // same for mag z
  fmz[indexm] = mz;
  smz = smz + fmz[indexm]; // oder mz
  dmz = smz / maxindexm;



  indexm++;
  if (indexm >= maxindexm) {        //   wrap index
    indexm = 0;
  }



  heading = atan2(-(my*0.00092), -(mx*0.00092)) * 18 / PI;

  Xh = dmx * cosPitch + dmz * sinPitch;
  Yh = dmx * sinRoll * sinPitch + dmy * cosRoll - dmz * sinRoll * cosPitch;

//  float heading = atan2(Yh, Xh);

  declination = 0.24; 
  heading += declination;


  if (heading < 0) {
    heading += 2 * PI;
  }
  if (heading > 2 * PI) {
    heading -= 2 * PI;
  }

  headingDegrees = heading * 180 / PI;
  headingFiltered = headingFiltered*0.85 + headingDegrees*0.15;

//  if (heading >= 0) {
//    Serial.print(round(heading));
//    Serial.print(round(roll*180/PI));
//    Serial.print(round(pitch/PI*180));
//  }


  Serial.print(roll*180/PI);
  Serial.print("/");
  Serial.println(pitch*180/PI);
  
//  Serial.println(round(headingFiltered));
  delay(50);
}