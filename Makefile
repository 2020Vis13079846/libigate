CC        = gcc
CFLAGS    = -Iinclude -std=c89 -Wall -Wextra -pedantic-errors

Q         = @

SRCS      := $(shell find -name "*.c" -print -type f)
OBJS       = $(patsubst %.c, %.o, $(SRCS))

.PHONY: all test clean

all: libigate.a

libigate.a: $(OBJS)
	$(Q) echo [Linking] $@
	$(Q) $(AR) rcs $@ $(OBJS)
	$(Q) echo done

test: test/main.c
	$(Q) echo [Compile] $<
	$(Q) $(CC) $< -o main $(CFLAGS) -ligate -L. -Iinclude
	$(Q) echo done

clean:
	$(Q) rm -f $(OBJS) main libigate.a

%.o: %.c
	$(Q) echo [Compile] $<
	$(Q) $(CC) -c $< -o $@ $(CFLAGS)
