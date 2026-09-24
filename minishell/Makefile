CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11
TARGET  = mini_shell
SRC     = mini_shell.c
 
.PHONY: all clean
 
all: $(TARGET)
 
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)
 
clean:
	rm -f $(TARGET)
 
