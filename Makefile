
vpath %.c src
vpath %.h include

CC       = gcc
CFLAGS   = -std=c17 -Wall -Wextra -Wpedantic
CPPFLAGS = -D_POSIX_C_SOURCE -D_GNU_SOURCE
LDFLAGS  = -lm 

TARGET   = hex2dec

all: $(TARGET)

$(TARGET): main.o
	$(CC) $(CFLAGS) $(CPPFLAGS) -I include -o $@ $^ $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -I include -c -o $@ $^

.PHONY: clean
clean:
	$(RM) ./*.{o,asm,lst} $(TARGET)


