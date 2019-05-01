CC = g++
BUILDDIR = build
SRCDIR = src
CFLAGS = -I/usr/include/SDL2 -D_REENTRANT
INC = -L/usr/lib -lSDL2 -Iinclude
TARGET = bin/output
OBJECTS = $(shell find $(SRCDIR) -name "*.cpp" | sed -r "s|$(SRCDIR)/([a-zA-Z]+).cpp|$(BUILDDIR)/\1.o|")

default: main

main: $(OBJECTS)
	$(CC) $(CFLAGS) $(INC) -o $(TARGET) $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	@echo "Removing $(BUILDDIR)/"; rm -r $(BUILDDIR)
