CC := g++
INCLUDES := -I. -I..
CXXFLAGS := -O3 --std=c++11 -Wall -Werror -g -fdiagnostics-color=always $(INCLUDES)
LDFLAGS := -Wall -Werror -fdiagnostics-color=always $(INCLUDES) -L. -L..

SOURCES=test.cpp
OBJECTS=$(SOURCES:.cpp=.o) ../graph.o
TARGET = test

.PHONY: all
export GRAPH_DEBUG=1
all: $(TARGET)

$(TARGET): $(OBJECTS)

testall: $(TARGET)
	@for input in ../studen*/T*.in ; do \
		echo "./$(TARGET)" < $$input;\
		./$(TARGET) < $$input;\
	done


.PHONY: clean
clean:
	rm -f $(OBJECTS) $(TARGET) vgcore* *outhyp

.PHONY: depend
depend: $(SOURCES)
	$(CC) $(INCLUDES) -MM $(SOURCES) > autodep
	@echo $(SOURCES)
