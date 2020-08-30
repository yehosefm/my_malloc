# this is my target
TARGET =test.out


# General compilation flags

# The compiler will be gcc("CC" is MAKE variable)
CC = gcc
# The compiler flags will be... ("CFLAGS" is MAKE variable)
CFLAGS = -ansi -pedantic -Wall -g 


# All .c files in this directory are my sources
SOURCES =  test.c mem_manager.c

# All .o files in this directory are my objects
OBJECTS = test.o mem_manager.o block_list.o

# The default command, compile all the sources
$(TARGET): $(OBJECTS)
	$(CC)  -o $(TARGET) $(OBJECTS)

test.o: test.c  mem_manager.h
	$(CC) $(CFLAGS) -c test.c

mem_manager.o: mem_manager.c mem_manager.h 
	$(CC) $(CFLAGS) -c mem_manager.c

block_list.o: block_list.c block_list.h
	$(CC) $(CFLAGS) -c block_list.c

clean:
	-rm $(TARGET) *.o *.a

run:  $(TARGET)

	 ./$(TARGET)

gdb:  $(TARGET)
	 gdb -q ./$(TARGET)
