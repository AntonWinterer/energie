//Prototypen fuer date_time.c
char* GetDateTimeString(void);
int GetElapsedSecondsToday(void);

//Prototypen fuer i2c.c
int GetCounterValue(void);
int GetCounterValue_MC9S08QG8(void);
int GetHourCounterValue_MC9S08QG8(int counternr);
double GetTemperaturValue_LM75(int lm75_devicenumber);
char GetTemperaturValue_LM75_8Bit(int lm75_devicenumber);
void ReadTest(void);
void WriteTest(void);
void Calibrate9s08(void);
void SetPWM(int ch, int val);

//Prototypen fuer database.c
void ValueToDatabase(char* date_time, int countervalue, char cabinettemperature, char wwtemperature,
                     int hourcounter1, int hourcounter2, int hourcounter3, int hourcounter4);


//Prototypen fuer main.c
