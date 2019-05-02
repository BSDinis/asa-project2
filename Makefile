CC := g++
VPATH := .
INCLUDES := -I.
CXXFLAGS := -O3 --std=c++11 -Wall -Werror -g -fdiagnostics-color=always $(INCLUDES)
LDFLAGS := -Wall -Werror -fdiagnostics-color=always $(INCLUDES) -L.

SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

all: $(OBJECTS)

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(TARGET) vgcore* *outhyp

.PHONY: depend
depend: $(SOURCES)
	$(CC) $(INCLUDES) -MM $(SOURCES) > autodep
	@echo $(SOURCES)
