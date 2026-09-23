# Detect the operating system
UNAME_S := $(shell uname -s)

# Default compiler flags
CFLAGS := -Wall -Wextra -std=c99 -O2

# Source files and output binary
SRCS := *.c
TARGET := chip8

# Platform-specific settings
ifeq ($(UNAME_S),Linux)
    CC := gcc
    CFLAGS += -D LINUX -lraylib
    LDFLAGS := -lm
endif
ifeq ($(UNAME_S),Darwin)
    CC := gcc
    CFLAGS += -D MACOS
    # Try pkg-config first, fallback to Homebrew paths
    ifneq ($(shell pkg-config --exists raylib && echo 1),1)
        # Fallback to Homebrew paths
        CFLAGS += -I/usr/local/include
        LDFLAGS := -L/usr/local/lib -lraylib
    else
        # Use pkg-config
        CFLAGS += $(shell pkg-config --cflags raylib)
        LDFLAGS := $(shell pkg-config --libs raylib)
    endif
    # macOS frameworks (required)
    LDFLAGS += -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
endif

# Default target
all: $(TARGET)

# Build the target
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
