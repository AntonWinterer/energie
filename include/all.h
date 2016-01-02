#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>       /* time_t, struct tm, time, gmtime */
#include <errno.h>
#include <unistd.h>     // needed for sleep()
#include <ctype.h>      //for isalnum()

#include <wiringPi.h>
#include <wiringPiI2C.h>

#include <mysql/mysql.h>

#include "fdef.h"

#define true  1
#define false 0

extern int verbose;
extern int verbose_max;
