# This file is part of the Bulb Interpreter and is released under the terms
# of the MIT/Expat License - see LICENSE.

# bulb version
VERSION = 0.1

# Optional flags
CFLAGS = -std=c99 -Wall -Wextra -Os -s

# Required flags
override CFLAGS += -Iinclude -fcommon
override LDFLAGS += -lm -lpthread

SRC = src/core.c \
      src/gc.c \
      src/parser.c \
      src/xxhash.c \
      src/hashmap.c \
      src/environment.c \
      src/evaluator.c \
      src/std.c

OBJ = $(SRC:.c=.o)

all: bulb libbulb.a
bulb: src/bulb.o $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
libbulb.a: $(OBJ)
	ar rcs $@ $(OBJ)
clean:
	$(RM) $(OBJ) bulb libbulb.a src/bulb.o
