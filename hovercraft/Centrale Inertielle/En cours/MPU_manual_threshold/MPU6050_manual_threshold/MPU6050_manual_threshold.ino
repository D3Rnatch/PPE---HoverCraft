#include <Wire.h>
#include "MPU_SETTINGS.h"

#define GYRO_CAL 500
#define ACCEL_CAL 500


bool blinkState = false;
int error;
uint8_t c;
double dT;
accel_t_gyro_union accel_t_gyro;
int offset[6] = { 0,0,0,0,0,0 };
int flag_offset_detect[12] = { 0,0,0,0,0,0,
                               0,0,0,0,0,0 };
int i = 0;

void setup()
{
  Wire.begin(); // Setup I2C pin com !
  Serial.begin(9600);
  // default at power-up:
  //    Gyro at 250 degrees second
  //    Acceleration at 2g
  //    Clock source at internal 8MHz
  //    The device is in sleep mode.

  error = MPU6050_read (MPU6050_WHO_AM_I, &c, 1);
  Serial.print(F("WHO_AM_I : "));
  Serial.print(c,HEX);
  Serial.print(F(", error = "));
  Serial.println(error,DEC);

  // According to the datasheet, the 'sleep' bit
  // should read a '1'. But I read a '0'.
  // That bit has to be cleared, since the sensor
  // is in sleep mode at power-up. Even if the
  // bit reads '0'.
  error = MPU6050_read (MPU6050_PWR_MGMT_2, &c, 1);
  Serial.print(F("PWR_MGMT_2 : "));
  Serial.print(c,HEX);
  Serial.print(F(", error = "));
  Serial.println(error,DEC);

  /* Interesting paragraph to keep*/
  // Clear the 'sleep' bit to start the sensor.
  MPU6050_write_reg (MPU6050_PWR_MGMT_1, 0);
  
  // Sensitivity Set to 16g and 2000°/s
  MPU6050_write_reg (MPU6050_ACCEL_CONFIG, 8);
  MPU6050_write_reg (MPU6050_GYRO_CONFIG, 8);
  /*
  // Interrupt Enabled On Treshold Motion Detection
  MPU6050_write_reg ( MPU6050_INT_ENABLE, 64);

  // Setting Threshold ACC/GYRO
  MPU6050_write_reg ( MPU6050_MOT_THR, 1);

  // Setting Threshold Reading Time
  MPU6050_write_reg ( MPU6050_MOT_DUR, 1);
  
  // OFFSET CALCULATION AND SETTING */
  delay(2000);
  // set_OFFSET();
  
  set_MEAN_OFFSET();
  
  for(i=0;i<12;i++)
    Serial.println(offset[i], DEC);
    
  Serial.println("OFFSET set u.u : 0.60");
  Serial.println("Finished Init");
}


void loop()
{
  // Readings
  error = MPU6050_read (MPU6050_ACCEL_XOUT_H, (uint8_t *) &accel_t_gyro, sizeof(accel_t_gyro));
  if(error != 0){ 
    Serial.println(F("Reading data, error=")); 
    Serial.println(error,DEC);  
  }
  else{
     // On récupère les données 
    uint8_t swap;
    #define SWAP(x,y) swap = x; x = y; y = swap
    SWAP (accel_t_gyro.reg.x_accel_h, accel_t_gyro.reg.x_accel_l);
    SWAP (accel_t_gyro.reg.y_accel_h, accel_t_gyro.reg.y_accel_l);
    SWAP (accel_t_gyro.reg.z_accel_h, accel_t_gyro.reg.z_accel_l);
    SWAP (accel_t_gyro.reg.t_h, accel_t_gyro.reg.t_l);
    SWAP (accel_t_gyro.reg.x_gyro_h, accel_t_gyro.reg.x_gyro_l);
    SWAP (accel_t_gyro.reg.y_gyro_h, accel_t_gyro.reg.y_gyro_l);
    SWAP (accel_t_gyro.reg.z_gyro_h, accel_t_gyro.reg.z_gyro_l);
    

    // Une fois les données récupérés on réalise 
    // le filtrage
    int data = 0;
    filter_threshold_v0(&data, offset, ACCEL_CAL);
    check_concurancy();   
    delay(5);  
}
}

void filter_threshold_v0(int *data, int offset[], float precision)
{
  int m = 100;
  
  if(accel_t_gyro.value.x_accel < offset[0] - precision) flag_offset_detect[0] ++;
  else if (accel_t_gyro.value.x_accel > offset[0] + precision) flag_offset_detect[1] ++;
  
  else if(accel_t_gyro.value.y_accel < offset[1] - precision) flag_offset_detect[2] ++;
  else if (accel_t_gyro.value.y_accel > offset[1] + precision) flag_offset_detect[3] ++;
  
  else if(accel_t_gyro.value.y_gyro < offset[3] - GYRO_CAL) flag_offset_detect[6] ++;
  else if (accel_t_gyro.value.y_gyro > offset[3] + GYRO_CAL) flag_offset_detect[7] ++;
}

void check_concurancy()
{
   if(flag_offset_detect[0] > 10){
     Serial.println("On recule !!");
     flag_offset_detect[0] = 0;
   }
  
  if(flag_offset_detect[1] > 10){
     Serial.println("On avance !!");
     flag_offset_detect[1] = 0;
   }
   
   if(flag_offset_detect[2] > 10){
     Serial.println("Droite !!");
     flag_offset_detect[2] = 0;
   }
  
  if(flag_offset_detect[3] > 10){
     Serial.println("Gauche !!");
     flag_offset_detect[3] = 0;
   }
   
   /*
     GYRO ZONE
   */
   if(flag_offset_detect[6] > 10){
     Serial.println("On tourne à gauche");
     flag_offset_detect[6] = 0;
   }
  
  if(flag_offset_detect[7] > 10){
     Serial.println("On tourne à droite");
     flag_offset_detect[7] = 0;
   }
}

void set_MEAN_OFFSET()
{   
 // Reading data
  error = MPU6050_read (MPU6050_ACCEL_XOUT_H, (uint8_t *) &accel_t_gyro, sizeof(accel_t_gyro));
  if(error != 0){ 
    Serial.println(F("Reading data, error=")); 
    Serial.println(error,DEC);  
  }
  
  // Swapping moment
  uint8_t swap;
    #define SWAP(x,y) swap = x; x = y; y = swap
    SWAP (accel_t_gyro.reg.x_accel_h, accel_t_gyro.reg.x_accel_l);
    SWAP (accel_t_gyro.reg.y_accel_h, accel_t_gyro.reg.y_accel_l);
    SWAP (accel_t_gyro.reg.z_accel_h, accel_t_gyro.reg.z_accel_l);
    SWAP (accel_t_gyro.reg.t_h, accel_t_gyro.reg.t_l);
    SWAP (accel_t_gyro.reg.x_gyro_h, accel_t_gyro.reg.x_gyro_l);
    SWAP (accel_t_gyro.reg.y_gyro_h, accel_t_gyro.reg.y_gyro_l);
    SWAP (accel_t_gyro.reg.z_gyro_h, accel_t_gyro.reg.z_gyro_l);
   
   offset[0] = accel_t_gyro.value.x_accel;
   offset[1] = accel_t_gyro.value.y_accel;
   offset[2] = accel_t_gyro.value.z_accel;
   offset[3] = accel_t_gyro.value.z_gyro;
   offset[4] = accel_t_gyro.value.y_gyro;
   offset[5] = accel_t_gyro.value.x_gyro;   
}

// --------------------------------------------------------
// MPU6050_read
//
// This is a common function to read multiple bytes
// from an I2C device.
//
// It uses the boolean parameter for Wire.endTransMission()
// to be able to hold or release the I2C-bus.
// This is implemented in Arduino 1.0.1.
//
// Only this function is used to read.
// There is no function for a single byte.
//
int MPU6050_read(int start, uint8_t *buffer, int size)
{
  int i, n, error;

  Wire.beginTransmission(MPU6050_I2C_ADDRESS);
  n = Wire.write(start);
  if (n != 1)
    return (-10);

  n = Wire.endTransmission(false);    // hold the I2C-bus
  if (n != 0)
    return (n);

  // Third parameter is true: relase I2C-bus after data is read.
  Wire.requestFrom(MPU6050_I2C_ADDRESS, size, true);
  i = 0;
  while(Wire.available() && i<size)
  {
    buffer[i++]=Wire.read();
  }
  if ( i != size)
    return (-11);

  return (0);  // return : no error
}

int MPU6050_read_reg(int reg, uint8_t *data)
{
  int error;

  error = MPU6050_read(reg, data, 1);

  return error; 
}

//--------------------------------------------
// CHECKS IF MOTION THRESHOLD HAS BEEN PASSED
// 
/*
int MPU6050_check_motion_treshold(uint8_t * t){
  uint8_t data = 0;
  int error;
  error = MPU6050_read_reg(MPU6050_INT_STATUS,&data);

  *t = data & 0b01000000; 

  return error; 
}*/

// --------------------------------------------------------
// MPU6050_write
//
// This is a common function to write multiple bytes to an I2C device.
//
// If only a single register is written,
// use the function MPU_6050_write_reg().
//
// Parameters:
//   start : Start address, use a define for the register
//   pData : A pointer to the data to write.
//   size  : The number of bytes to write.
//
// If only a single register is written, a pointer
// to the data has to be used, and the size is
// a single byte:
//   int data = 0;        // the data to write
//   MPU6050_write (MPU6050_PWR_MGMT_1, &c, 1);
//
int MPU6050_write(int start, const uint8_t *pData, int size)
{
  int n, error;

  Wire.beginTransmission(MPU6050_I2C_ADDRESS);
  n = Wire.write(start);        // write the start address
  if (n != 1)
    return (-20);

  n = Wire.write(pData, size);  // write data bytes
  if (n != size)
    return (-21);

  error = Wire.endTransmission(true); // release the I2C-bus
  if (error != 0)
    return (error);

  return (0);         // return : no error
}

// --------------------------------------------------------
// MPU6050_write_reg
//
// An extra function to write a single register.
// It is just a wrapper around the MPU_6050_write()
// function, and it is only a convenient function
// to make it easier to write a single register.
//
int MPU6050_write_reg(int reg, uint8_t data)
{
  int error;

  error = MPU6050_write(reg, &data, 1);

  return (error);
}
