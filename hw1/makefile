#This is the example of makefile
CC = gcc#reference gcc for CC
CFLAGS = -I../inc -g#This is compile option
LDFLAGS = -L../lib -lm#This is linking option
TARGET = a.out#This is the file what would be build
OBJS = a.o b.o main.o#This is the object file for target

$(TARGET) : $(OBJS)#make target with object files
	$(CC) -o $@ $(OBJS) $(LDFLAGS)
.SUFFIXES : .c .o#make object file by compiling c files
.c.o:
	$(CC) -c $(CFLAGS) $<
clean:#remove files
	rm -f core $(TARGET) $(OBJS)
