/*
See url for more info:
http://www.cyberciti.biz/tips/linux-unix-connect-mysql-c-api-program.html


gcc -o output-file $(mysql_config --cflags) mysql-c-api.c $(mysql_config --libs)


Systemvorbereitung:
-mysql installieren!!!
-Kommando: mysql -p -u root
 in SQL-Shell die Datenbank erzeugen:
 -CREATE DATABASE heizung;
 exit
-Kommando: mysql -p -u root -D heizung
 in SQL-Shell die Tabelle erzeugen:
 CREATE TABLE energieverbrauch(uhrzeit DATETIME, counter INTEGER);
 exit


*/

#include "include/all.h"


void ValueToDatabase(char powerdownbackup, char* date_time, int countervalue, char cabinettemperature, char wwtemperature,
                     int hourcounter1, int hourcounter2, int hourcounter3, int hourcounter4)
{
   MYSQL *conn;
   MYSQL_RES *res;

   FILE* fp;
   char server[50];
   char user[50];
   char password[50];
   char database[50];

   //fp = fopen("credentials.txt","r");
   fp = fopen("/home/pi/energie/credentials.txt","r");
   if(fp==NULL){
     printf("\r\nUnable to open credentials.txt\n");
     exit(1);
   }else{
     char zeile[250];
     int lines = 0;
     int len;
     int i;
     while(fgets(zeile,sizeof(zeile)-1,fp)) {
       len = strlen(zeile);
       //printf("%s\n",zeile);
       for(i=0;i<len;i++){
         if(isalnum(zeile[i]) || zeile[i] == '_' ){
           continue;
         }else{
           zeile[i] = 0x00;
           break;
         }
       }
       if(lines==0){
         sprintf(server,"%s",zeile);
       }else if(lines==1){
         sprintf(user,"%s",zeile);
       }else if(lines==2){
         sprintf(password,"%s",zeile);
       }else if(lines==3){
         sprintf(database,"%s",zeile);
       }
       lines++;
     }
     fclose(fp);
   }

   if(verbose_max){
     printf("  Server: %s\n",server);
     printf("Database: %s\n",database);
     printf("    User: %s\n",user);
     printf("Password: %s\n",password);
   }

   conn = mysql_init(NULL);

   // Connect to database
   if (!mysql_real_connect(conn, server,
         user, password, database, 0, NULL, 0)) {
      printf("%s\n", mysql_error(conn));
      exit(1);
   }


   // send SQL query
   char sql_string[2048];
   sprintf(sql_string,"INSERT INTO energieverbrauch (powerdownbackup, uhrzeit, counter, warmwassertemperatur, cabinettemperatur, hourcounter1, hourcounter2, hourcounter3, hourcounter4) VALUES(%d, '%s', %d, %d, %d, %d, %d, %d, %d);",
                       powerdownbackup,date_time,countervalue,wwtemperature,cabinettemperature,hourcounter1,hourcounter2,hourcounter3,hourcounter4);

   if(verbose){
     printf("%s\n",sql_string);
   }
   if (mysql_query(conn, sql_string)) {
      printf("%s\n", mysql_error(conn));
      exit(1);
   }
   res = mysql_use_result(conn);

   // close connection
   mysql_free_result(res);
   mysql_close(conn);

}

void BackupValueFromDatabase(int* countervalue, int* hourcounter1, int* hourcounter2, int* hourcounter3, int* hourcounter4)
{
   MYSQL *conn;
   MYSQL_RES *result;
   unsigned int num_fields;
   unsigned int num_rows;
//   unsigned int i;
//   MYSQL_FIELD *fields;
   MYSQL_ROW row;

   FILE* fp;
   char server[50];
   char user[50];
   char password[50];
   char database[50];

   //fp = fopen("credentials.txt","r");
   fp = fopen("/home/pi/energie/credentials.txt","r");
   if(fp==NULL){
     printf("\r\nUnable to open credentials.txt\n");
     exit(1);
   }else{
     char zeile[250];
     int lines = 0;
     int len;
     int i;
     while(fgets(zeile,sizeof(zeile)-1,fp)) {
       len = strlen(zeile);
       //printf("%s\n",zeile);
       for(i=0;i<len;i++){
         if(isalnum(zeile[i]) || zeile[i] == '_' ){
           continue;
         }else{
           zeile[i] = 0x00;
           break;
         }
       }
       if(lines==0){
         sprintf(server,"%s",zeile);
       }else if(lines==1){
         sprintf(user,"%s",zeile);
       }else if(lines==2){
         sprintf(password,"%s",zeile);
       }else if(lines==3){
         sprintf(database,"%s",zeile);
       }
       lines++;
     }
     fclose(fp);
   }

   if(verbose_max){
     printf("  Server: %s\n",server);
     printf("Database: %s\n",database);
     printf("    User: %s\n",user);
     printf("Password: %s\n",password);
   }

   conn = mysql_init(NULL);

   // Connect to database
   if (!mysql_real_connect(conn, server,
         user, password, database, 0, NULL, 0)) {
      printf("%s\n", mysql_error(conn));
      exit(1);
   }

   // send SQL query
   char sql_string[2048];
   //sprintf(sql_string,"SELECT MAX(uhrzeit), powerdownbackup, counter, hourcounter1, hourcounter2, hourcounter3, hourcounter4 FROM energieverbrauch WHERE powerdownbackup = 1;");
   sprintf(sql_string,"SELECT uhrzeit, powerdownbackup, counter, hourcounter1, hourcounter2, hourcounter3, hourcounter4 FROM energieverbrauch WHERE uhrzeit = (SELECT MAX(uhrzeit) FROM energieverbrauch WHERE powerdownbackup = 1);");
   if(verbose_max){
     printf("%s\n",sql_string);
   }

   if(mysql_query(conn, sql_string)){
      printf("%s\n", mysql_error(conn));
      exit(1);
   }

   result = mysql_store_result(conn);
   if(result){

     num_fields = mysql_num_fields(result);
     row = mysql_fetch_row(result);
     num_rows = mysql_num_rows(result);

     if(verbose){
       printf("fields %d\n",num_fields);
       printf("rows %d\n",num_rows);
     }

     if(num_fields == 7 && num_rows == 1){
       char* end;
       *countervalue = strtol(row[2], &end, 10);
       *hourcounter1 = strtol(row[3], &end, 10);
       *hourcounter2 = strtol(row[4], &end, 10);
       *hourcounter3 = strtol(row[5], &end, 10);
       *hourcounter4 = strtol(row[6], &end, 10);
     }

//     fields = mysql_fetch_fields(result);
//     for(i = 0; i < num_fields; i++){
//       printf("Field %u is %s\n", i, fields[i].name);
//     }
//     while ((row = mysql_fetch_row(result))){
//       for(i = 0; i < num_fields; i++){
//         printf("%s ", row[i] ? row[i] : "NULL");
//       }
//       printf("\n");
//     }

   }
   //res = mysql_use_result(conn);

   // close connection
   mysql_free_result(result);
   mysql_close(conn);

}

