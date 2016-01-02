
#include "include/all.h"

int verbose = false;
int verbose_max = false;
int readtest = false;
int writetest = false;
int calibrate_9s08 = false;
int restoredata_9S08 = false;


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
      ValueToDatabase(false,"2015-07-23 00:00:00",1,2,3,10,11,12,13);
    }else if(strncmp(argv[1],"-t",2) == 0){
      verbose = true;
      ThreadPowerOff();
      return(0);
    }else if(strncmp(argv[1],"-T",2) == 0){
      //verbose_max = true;
      verbose = true;
      restoredata_9S08 = true;
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

  if(restoredata_9S08){

    BackupValueFromDatabase(&cc,&hc[0],&hc[1],&hc[2],&hc[3]);

    SetCounterValue_MC9S08QG8(cc);
    if(verbose){
      printf("counter: %d\n",cc);
    }

    for(i=0;i<4;i++){
      SetHourCounterValue_MC9S08QG8(i,hc[i]);
      if(verbose){
        printf("hourcounter [%d]: %d\n",i,hc[i]);
      }
    }

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

  ValueToDatabase(false,ts,cc,tv[0],tv[1],hc[0],hc[1],hc[2],hc[3]);

  return(0);
}


void ThreadPowerOff(void)
{
  int x = piThreadCreate(ThreadPowerCheck);
  if(x != 0){
    printf("it didn't start\n");
  }
  if(verbose){
    printf("start succesfully\n");
  }
  while(true);
}


PI_THREAD(ThreadPowerCheck)
{
  int pinstate;
  int wait4low = false;
  int i;
  char ts[50];  //timestring
  int cc;       //current counter (9S08QG8)
  int hc[4];    //hour counter (9S08QG8)
  char tv[2];   //temperatur value (LM75)



  wiringPiSetupGpio();
  pinMode(4,INPUT);
  pullUpDnControl(4, PUD_UP) ;

  pinstate = digitalRead(4);
  if(verbose){
    printf("pin 4 state is: %d\n",pinstate);
  }

  for(;;){
    pinstate = digitalRead(4);
    if(pinstate == 1){
      if(wait4low == false){
        if(verbose){
          printf("pin is high\n");
        }
        if(IsPowerdownBackupPossible()){

          strcpy(ts,GetDateTimeString());
          cc = GetCounterValue_MC9S08QG8();
          for(i=0;i<4;i++){
            hc[i] = GetHourCounterValue_MC9S08QG8(i);
          }
          for(i=0;i<2;i++){
            tv[i] = GetTemperaturValue_LM75_8Bit(i);
          }
          ValueToDatabase(true,ts,cc,tv[0],tv[1],hc[0],hc[1],hc[2],hc[3]);
          if(verbose){
            printf("data stored\n");
          }
          wait4low = true; //d.h. Spanung muss auch wirklich weg gehen

        }else{
          if(verbose){
            printf("blocked, wait 1sec\n");
          }
        }
      }
    }else{ //pin is low
      wait4low = false;
    }
    delay(1000);
  }
  return(0);
}

