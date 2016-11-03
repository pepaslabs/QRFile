BIN = qrfile

DEPS = $(wildcard deps/*/*.c)
SRCS = $(wildcard src/*.c)
OBJS = $(DEPS:.c=.o) $(SRCS:.c=.o)

CFLAGS = -Wall -Werror
INCLUDES = -Isrc -Ideps
CC = cc

default: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(BIN)

.PHONY: default clean
