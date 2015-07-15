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

  /* Change me */
   char server[50];
   char user[50];
   char password[50];
   char database[50];

   conn = mysql_init(NULL);

   /* Connect to database */
   if (!mysql_real_connect(conn, server,
         user, password, database, 0, NULL, 0)) {
      //fprintf(stderr, "%s\n", mysql_error(conn));
      printf("%s\n", mysql_error(conn));
      exit(1);
   }

   if(verbose){
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

