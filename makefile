#
# makefile for subproject energie
#

# *** MACROS

INSTPATH=./bin/myapp
INCPATH=./include
OBJPATH=./obj

CC=gcc
CFLAGS=-g -Wall -I$(INCPATH) -I/usr/include/mysql -DBIG_JOINS=1 -fno-strict-aliasing -L/usr/lib/arm-linux-gnueabihf -lmysqlclient -lpthread -lz -lm -lrt -ldl -lwiringPi

COND1=`stat app 2>/dev/null | grep Modify`
COND2=`stat $(INSTPATH) 2>/dev/null | grep Modify`

# *** Targets

all: getobj app install putobj

app: main.o i2c.o date_time.o database.o
	@echo "building target" $@ "..."
	$(CC) $(CFLAGS) -o app main.o i2c.o date_time.o database.o

.c.o:
	@echo "compiling" $< "..."
	$(CC) $(CFLAGS) -c $<

getobj:
	@echo "get object files ..."
	@if [ -d "$(OBJPATH)" ]; then echo "obj-dir exists"; else mkdir obj/; echo "obj-dir created"; fi
	-mv $(OBJPATH)/*.o . 2>/dev/null

putobj:
	@echo "put object files ..."
	@if [ -d "$(OBJPATH)" ]; then echo "obj-dir exists"; else mkdir obj/; echo "obj-dir created"; fi
	-mv *.o $(OBJPATH) 2>/dev/null

# Process only when app timestamp is changed
install:
	@echo "move applictaion to app folder ..."
	@if [ "$(COND1)" != "$(COND2)" ];\
	then\
		cp -p ./app $(INSTPATH) 2>/dev/null;\
		chmod 700 $(INSTPATH);\
		echo "installed in" $(INSTPATH);\
	fi

# This one is used for housekepping
cleanall:
	@echo "housekeeping ..."
	-rm -f app
	-rm -f $(OBJPATH)/*.o
#	-rm -f $(INSTPATH)

