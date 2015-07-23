
#include "include/all.h"

int verbose = false;
int readtest = false;
int writetest = false;
int calibrate_9s08 = false;

int main(int argc, char **argv)
{
  int i;
  char ts[50];//timestring
  int cc;     //current counter (9S08QG8)
  int hc[4];  //hour counter (9S08QG8)
  char tv;    //temperatur value (LM75)

  if(argc == 2){
    if(strncmp(argv[1],"-v",2) == 0){
      verbose = true;
    }else if(strncmp(argv[1],"-r",2) == 0){
      readtest = true;
    }else if(strncmp(argv[1],"-w",2) == 0){
      writetest = true;
    }else if(strncmp(argv[1],"-c",2) == 0){
      calibrate_9s08 = true;
    }else if(strncmp(argv[1],"-d",2) == 0){
      verbose = true;
      ValueToDatabase("2015-07-23 00:00:00",1,2,10,11,12,13);
    }else if(strncmp(argv[1],"-p",2) == 0){
      verbose = true;
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
  }else if(argc == 4){
    if(strncmp(argv[1],"-p",2) == 0){
      verbose = true;
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
    printf("Uhrezeit: %s\n",ts);
  }

  cc = GetCounterValue_MC9S08QG8();
  if(verbose){
    printf("counter: %d\n",cc);
  }

  for(i=0;i<4;i++){
    hc[i] = GetHourCounterValue_MC9S08QG8(i+1);
    if(verbose){
      printf("counter: %d\n",hc[i]);
    }
  }

  tv = GetTemperaturValue_LM75_8Bit(1);
  if(verbose){
    printf("LM75.1 temperature: %d\n",tv);
  }

  ValueToDatabase(ts,cc,tv,hc[0],hc[1],hc[2],hc[3]);

  return(0);
}



