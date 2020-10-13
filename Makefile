CC        = gcc
CFLAGS    = -Iinclude

Q         = @

SRCS      := $(shell find -name "*.c" -print -type f)
OBJS       = $(patsubst %.c, %.o, $(SRCS))

LDFLAGS    = -L. -ligate -lusb-1.0

.PHONY: all test clean

all: libigate.a

libigate.a: $(OBJS)
	$(Q) echo [Linking] $@
	$(Q) $(AR) rcs $@ $(OBJS)
	$(Q) echo done

test: test/main.c
	$(Q) echo [Compile] $<
	$(Q) $(CC) $< -o main $(CFLAGS) $(LDFLAGS)
	$(Q) echo done

clean:
	$(Q) rm -f $(OBJS) main libigate.a

%.o: %.c
	$(Q) echo [Compile] $<
	$(Q) $(CC) -c $< -o $@ $(CFLAGS)
