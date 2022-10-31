include ..\Makefile.inc

TARGET = ..\..\targets\objs

all		:	$(TARGET)\video.o $(TARGET)\utility.o
		
$(TARGET)\video.o : Video.cpp $(INCLUDE)\Video.h
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -c $< -o $@

$(TARGET)\utility.o : Utility.cpp $(INCLUDE)\Utility.h
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -c $< -o $@