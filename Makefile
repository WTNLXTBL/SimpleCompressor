CC = g++
CFLAGS = -Wall -Wextra -pedantic -std=c++11
LDFLAGS = -lz

SRCDIR = src
OBJDIR = obj
BINDIR = bin

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
DEPS = $(OBJS:.o=.d)
TARGET = $(BINDIR)/compressor

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

clean:
	$(RM) -r $(OBJDIR) $(BINDIR)

-include $(DEPS)

run:
	@echo "Usage: compressor [-c|-d] input output. -c 'file to be compressed' filename | -d 'file to be decompressed' filename"

compress:
	./$(TARGET) -c input output

decompress:
	./$(TARGET) -d input output
