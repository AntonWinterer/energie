/*
Quelle:
http://www.raspberrypi.org/forums/viewtopic.php?t=30984&p=289545

-Einmalige Vorbereitung:
    cd
    sudo apt-get install libi2c-dev
    git clone git://git.drogon.net/wiringPi
    cd wiringPi
    ./build

-I2C Modul laden:
    gpio load i2c

-I2C Test:
    i2cdetect -y 1

    i2cget [-f] [-y] I2CBUS CHIP-ADDRESS [DATA-ADDRESS [MODE]]
      I2CBUS is an integer or an I2C bus name
      ADDRESS is an integer (0x03 - 0x77)
      MODE is one of:
        b (read byte data, default)
        w (read word data)
        c (write byte/read byte)
        Append p for SMBus PEC

-Quellcode uebersetzen:
NOTE: To compile programs with wiringPi, you need to add:
    -lwiringPi
  to your compile line(s) To use the Gertboard, MaxDetect, etc.
  code (the devLib), you need to also add:
    -lwiringPiDev
  to your compile line(s).

gcc -o i2c i2c.c -lwiringPi

*/

/*
 * q2w.c:
 ***********************************************************************
 */

#include "include/all.h"

int GetCounterValue(void)
{

  int q2w;
  int i2c_address = 0x50; //0x50 = RTC PCF8583; 0x68 = RTC DS1307
  int i;
  char value_hlp[20];
  int r[5];
  int value;

  if ((q2w = wiringPiI2CSetup (i2c_address)) == -1){
    printf("q2w: Unable to initialise I2C: %s\n", strerror (errno)) ;
    exit(1);
  }

  wiringPiI2CWrite(q2w, 0x00); //Addresspointer auf 0x00 setzen

  for(i=0;i<4;i++){
    r[i] = wiringPiI2CRead(q2w); //lesen von Addrespointer++
    if(verbose){
      printf("register value [%d]: %02X\n",i,r[i]);
    }
  }
  sprintf(value_hlp,"%x%x%x",r[3],r[2],r[1]);
  value = atoi(value_hlp);

  return(value);
}

int GetCounterValue_MC9S08QG8(void)
{

  int q2w;
  int i2c_address = 0x50; //0x50 = RTC PCF8583; 0x68 = RTC DS1307
  int i;
  int r[5];
  int value = 0;

  if ((q2w = wiringPiI2CSetup (i2c_address)) == -1){
    printf("q2w: Unable to initialise I2C: %s\n", strerror (errno)) ;
    exit(1);
  }

  if(verbose){
    printf("\n\ncurrent counter\n");
  }

  for(i=0;i<4;i++){
    r[i] = wiringPiI2CReadReg8(q2w, i);
    if(verbose){
      printf("register value [%d]: %02X\n",i,r[i]);
    }
  }
  for(i=0;i<4;i++){
    value <<= 8;
    value |= (r[3-i]);
    if(verbose){
      printf("value (i=%d): %08X\n",i,value);
    }

  }
  if(verbose){
    printf("value: %08d / %08x\n",value,value);
  }

  return(value);
}


int GetHourCounterValue_MC9S08QG8(void)
{

  int q2w;
  int i2c_address = 0x50; //0x50 = RTC PCF8583; 0x68 = RTC DS1307
  int i;
  int r[5];
  int value = 0;

  if ((q2w = wiringPiI2CSetup (i2c_address)) == -1){
    printf("q2w: Unable to initialise I2C: %s\n", strerror (errno)) ;
    exit(1);
  }

  if(verbose){
    printf("\n\nhour counter\n");
  }

  for(i=0;i<4;i++){
    r[i] = wiringPiI2CReadReg8(q2w, (i+4));
    if(verbose){
      printf("register value [%d]: %02X\n",i,r[i]);
    }
  }
  for(i=0;i<4;i++){
    value <<= 8;
    value |= (r[3-i]);
    if(verbose){
      printf("value (i=%d): %08X\n",i,value);
    }

  }
  if(verbose){
    printf("value: %08d / %08x\n",value,value);
  }

  return(value);
}


double GetTemperaturValue_LM75(int lm75_devicenumber)
{

  int q2w;
  int i2c_address = 0x48 + lm75_devicenumber;
  int value = 0;
  double temperatur = 0.0;

  if ((q2w = wiringPiI2CSetup (i2c_address)) == -1){
    printf("q2w: Unable to initialise I2C: %s\n", strerror (errno)) ;
    exit(1);
  }

  if(verbose){
    printf("\n\nlm75 device\n");
  }

  value = wiringPiI2CReadReg16(q2w, 0);
  if(verbose){
    printf("register value : %04X\n",value);
  }

  temperatur = (double)(value&0x00ff);
  if(value&0x8000){
    temperatur += 0.5;
  }

  return(temperatur);
}

char GetTemperaturValue_LM75_8Bit(int lm75_devicenumber)
{

  int q2w;
  int i2c_address = 0x48 + lm75_devicenumber;
  int value = 0;
  char temperatur = 0;

  if ((q2w = wiringPiI2CSetup (i2c_address)) == -1){
    printf("q2w: Unable to initialise I2C: %s\n", strerror (errno)) ;
    exit(1);
  }

  if(verbose){
    printf("\n\nlm75 device\n");
  }

  value = wiringPiI2CReadReg16(q2w, 0);
  if(verbose){
    printf("register value : %04X\n",value);
  }

  temperatur = (char)(value&0x00ff);

  return(temperatur);
}


//////////////////////////////////
//
// Testfunktionen zum Debuggen
//
//
void WriteTest(void)
{
  int q2w;
  int i2c_address = 0x50;

  if ((q2w = wiringPiI2CSetup (i2c_address)) == -1){
    printf("q2w: Unable to initialise I2C: %s\n", strerror (errno)) ;
    exit(1);
  }

//  wiringPiI2CWrite(q2w, 0xA5);
//  printf("writing 0xA5 to address 0x%02X\n",i2c_address);

  wiringPiI2CWriteReg8(q2w, 0x01, 0x5A);
  printf("writing 0x5A to address 0x%02X register 0x01\n",i2c_address);
}

void ReadTest(void)
{
  int q2w;
  int i2c_address = 0x49;
  int r;
  char t;

  if ((q2w = wiringPiI2CSetup (i2c_address)) == -1){
    printf("q2w: Unable to initialise I2C: %s\n", strerror (errno)) ;
    exit(1);
  }

//  r = wiringPiI2CRead(q2w);
//  printf("reading 0x%02X from 0x%02X\n",r,i2c_address);

//  r = wiringPiI2CReadReg8(q2w, 0x00);
//  printf("reading 0x%02X from 0x%02X register 0x00\n",r,i2c_address);
//
//  r = wiringPiI2CReadReg8(q2w, 0x01);
//  printf("reading 0x%02X from 0x%02X register 0x01\n",r,i2c_address);
//
//  r = wiringPiI2CReadReg8(q2w, 0x02);
//  printf("reading 0x%02X from 0x%02X register 0x02\n",r,i2c_address);
//
//  r = wiringPiI2CReadReg8(q2w, 0x03);
//  printf("reading 0x%02X from 0x%02X register 0x03\n",r,i2c_address);

//  double t = GetTemperaturValue_LM75(2);
//  printf("Temperatur LM75 [%d]: %.1f\n",2,t);

  t = GetTemperaturValue_LM75_8Bit(2);
  printf("Temperatur LM75 [%d]: %d\n",2,t);

//  r = wiringPiI2CReadReg16(q2w, 0x00);
//  printf("reading 0x%04X from 0x%02X\n",r,i2c_address);


}


void Calibrate9s08(void)
{

  int wait = 200;
  int start,end;

//  int q2w;
//  int i2c_address = 0x50;
//
//  if ((q2w = wiringPiI2CSetup (i2c_address)) == -1){
//    printf("q2w: Unable to initialise I2C: %s\n", strerror (errno)) ;
//    exit(1);
//  }


  start = GetElapsedSecondsToday();
  printf("Elapsed Seconds Today: %d\n",start);
//  wiringPiI2CWriteReg8(q2w, 0x03, 0); //Kalibrierung starten
  do{
    if((wait%20)==0){
      printf("%4d\n",wait);
    }
    //printf(".");
    //puts(".");
    putchar('.');
    sleep(1);
  }while(--wait);
  end = GetElapsedSecondsToday();
//  wiringPiI2CWriteReg8(q2w, 0x03, 200); //Kalibrierung fertigstellen
  printf("ElapsedSecondsToday: %d\n",end);
}
