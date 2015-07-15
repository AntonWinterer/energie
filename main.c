
#include "include/all.h"

int verbose = false;
int readtest = false;
int writetest = false;
int calibrate_9s08 = false;

int main(int argc, char **argv)
{
  char ts[50];
  int v1,v2;
  char v3;

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
      ValueToDatabase("ts",1,2,'a');
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

  //v = GetCounterValue(); //vom Uhrenbaustein
  v1 = GetCounterValue_MC9S08QG8();
  if(verbose){
    printf("counter: %d\n",v1);
  }

  v2 = GetHourCounterValue_MC9S08QG8();
  if(verbose){
    printf("counter: %d\n",v2);
  }

  v3 = GetTemperaturValue_LM75_8Bit(1);
  if(verbose){
    printf("LM75.1 temperature: %d\n",v3);
  }

  ValueToDatabase(ts,v1,v2,v3);

  return(0);
}



