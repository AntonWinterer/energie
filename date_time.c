
#include "include/all.h"

static char datetime[30];

char* GetDateTimeString(void)
{
  time_t rawtime;
  struct tm* ptm;

  time( &rawtime );
  ptm = gmtime( &rawtime );
  //ptm beginnt am 1.1.1900
  //Monat Januar hat den Wert 0!
  sprintf(datetime,"%04d-%02d-%02d %02d:%02d:%02d",(ptm->tm_year+1900),(ptm->tm_mon+1),ptm->tm_mday,
                                                   ptm->tm_hour,ptm->tm_min,ptm->tm_sec);

  return(datetime);
}

int GetElapsedSecondsToday(void)
{
  time_t rawtime;
  struct tm* ptm;
  int seconds = 0;

  time( &rawtime );
  ptm = gmtime( &rawtime );
  //ptm beginnt am 1.1.1900
  //Monat Januar hat den Wert 0!
  seconds =  ptm->tm_hour * 3600;
  seconds += ptm->tm_min * 60;
  seconds += ptm->tm_sec;
  return(seconds);
}

int IsPowerdownBackupPossible(void)
{
  time_t rawtime;
  struct tm* ptm;

  time( &rawtime );
  ptm = gmtime( &rawtime );

  return(ptm->tm_sec > 3);
}

