CC := g++
SRCDIR := src
BUILDDIR := build
BINDIR := bin
TARGET := $(BINDIR)/generator
DBG_TARGET := $(BINDIR)/dbg_generator

SRCEXT := cc
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
DBG_OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.dbg.o))
CFLAGS := -Wall -O3 -std=c++0x -D NDEBUG 
DBGCFLAGS := -Wall -O0 -std=c++0x -g
LIB := -lpthread -ljemalloc -L lib
INC := -I include

$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $^"; $(CC) $(CFLAGS) $(INC) -c -o $@ $^

debug: $(DBG_TARGET)

$(DBG_TARGET): $(DBG_OBJECTS)
	@mkdir -p $(BINDIR)
	@echo " Build with debug..."
	@echo " $(CC) $^ -o $(DBG_TARGET) $(LIB)"; $(CC) $^ -o $(DBG_TARGET) $(LIB)

$(BUILDDIR)/%.dbg.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(DBGCFLAGS) -g $(INC) -c -o $@ $^"; $(CC) $(DBGCFLAGS) $(INC) -c -o $@ $^

clean:
	@echo " Cleaning..."
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

target:
	@echo " Build Spike..."


.PHONY: clean
