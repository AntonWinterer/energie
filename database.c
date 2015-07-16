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


void ValueToDatabase(char* date_time, int countervalue, int hourcounter, char wwtemperature)
{
   MYSQL *conn;
   MYSQL_RES *res;

   FILE* fp;

   /* Change me */
   char server[50];
   char user[50];
   char password[50];
   char database[50];

   fp = fopen("credentials.txt","r");
   if(fp==NULL){
     printf("\r\nUnable to open credentials.txt\n");
     exit(1);
   }else{
     char zeile[250];
     int lines = 0;
     int len;
     int i;
     while(fgets(zeile,sizeof(zeile)-1,fp)) {
       //printf("Zeile %d : %s\n",lines,zeile);
       len = strlen(zeile);
       //printf("laenge: %d\n",len);
       for(i=0;i<len;i++){
         //if(!isascii(zeile[i])){
         if(!isalnum(zeile[i])){
           //printf("position: %d\n",i);
           zeile[i] = 0x00;
           break;
         }
       }
       if(lines==0){
         sprintf(server,"%s",zeile);
         printf("server: %s\n",server);
       }else if(lines==1){
         sprintf(user,"%s",zeile);
         printf("user: %s\n",user);
       }else if(lines==2){
         sprintf(password,"%s",zeile);
         printf("password: %s\n",password);
       }else if(lines==3){
         sprintf(database,"%s",zeile);
         printf("database: %s\n",database);
       }
       lines++;
     }
     fclose(fp);
   }

   conn = mysql_init(NULL);

   /* Connect to database */
   if (!mysql_real_connect(conn, server,
         user, password, database, 0, NULL, 0)) {
      //fprintf(stderr, "%s\n", mysql_error(conn));
      printf("%s\n", mysql_error(conn));
      exit(1);
   }

   if(verbose){
     printf("  Server: %s\n",server);
     printf("Database: %s\n",database);
     printf("    User: %s\n",user);
     printf("Password: %s\n",password);
   }

   /* send SQL query */
   char sql_string[2048];
   sprintf(sql_string,"INSERT INTO energieverbrauch VALUES('%s', %d, %d, %d);",date_time,countervalue,hourcounter,wwtemperature);
   if(verbose){
     printf("%s\n",sql_string);
   }
   if (mysql_query(conn, sql_string)) {
      //fprintf(stderr, "%s\n", mysql_error(conn));
      printf("%s\n", mysql_error(conn));
      exit(1);
   }
   res = mysql_use_result(conn);

   /* send SQL query */
   if (mysql_query(conn, "show tables")) {
      //fprintf(stderr, "%s\n", mysql_error(conn));
      printf("%s\n", mysql_error(conn));
      exit(1);
   }
   res = mysql_use_result(conn);


   /* close connection */
   mysql_free_result(res);
   mysql_close(conn);


}

