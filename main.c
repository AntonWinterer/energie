
#include "include/all.h"

int verbose = false;
int verbose_max = false;
int readtest = false;
int writetest = false;
int calibrate_9s08 = false;

int main(int argc, char **argv)
{
  int i;
  char ts[50];  //timestring
  int cc;       //current counter (9S08QG8)
  int hc[4];    //hour counter (9S08QG8)
  char tv[2];   //temperatur value (LM75)

  if(argc == 2){
    if(strncmp(argv[1],"-V",2) == 0){
      verbose_max = true;
      verbose = true;
    }else if(strncmp(argv[1],"-v",2) == 0){
      verbose = true;
    }else if(strncmp(argv[1],"-r",2) == 0){
      readtest = true;
    }else if(strncmp(argv[1],"-w",2) == 0){
      writetest = true;
    }else if(strncmp(argv[1],"-c",2) == 0){
      verbose = true;
      calibrate_9s08 = true;
    }else if(strncmp(argv[1],"-d",2) == 0){
      ValueToDatabase("2015-07-23 00:00:00",1,2,3,10,11,12,13);
    }
  }else if(argc == 4){
    if(strncmp(argv[1],"-p",2) == 0){
      printf("v1: %s\n",argv[1]);
      printf("v2: %s\n",argv[2]);
      printf("v3: %s\n",argv[3]);
      if(strncmp(argv[2],"1",1) == 0){
        int val = atoi(argv[3]);
        printf("val: %d\n",val);
        SetPWM(1,val);
      }else if(strncmp(argv[2],"2",1) == 0){
        int val = atoi(argv[3]);
        SetPWM(2,val);
      }
      return(0);
    }
  }


  if(readtest){
    ReadTest();
    return(0);
  }
  if(writetest){
    WriteTest();
    return(0);
  }
  if(calibrate_9s08){
    Calibrate9s08();
    return(0);
  }


  strcpy(ts,GetDateTimeString());
  if(verbose){
    printf("Uhrzeit: %s\n",ts);
  }

  cc = GetCounterValue_MC9S08QG8();
  if(verbose){
    printf("counter: %d\n",cc);
  }

  for(i=0;i<4;i++){
    hc[i] = GetHourCounterValue_MC9S08QG8(i);
    if(verbose){
      printf("hourcounter [%d]: %d\n",i,hc[i]);
    }
  }

  for(i=0;i<2;i++){
    tv[i] = GetTemperaturValue_LM75_8Bit(i);
    if(verbose){
      printf("LM75.%d temperature: %d\n",i,tv[i]);
    }
  }

  ValueToDatabase(ts,cc,tv[0],tv[1],hc[0],hc[1],hc[2],hc[3]);

  return(0);
}



