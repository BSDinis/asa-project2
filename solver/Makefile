CC := g++
INCLUDES := -I. -I..
CXXFLAGS := -O3 --std=c++11 -Wall -Werror -g -fdiagnostics-color=always $(INCLUDES)
LDFLAGS := -Wall -Werror -fdiagnostics-color=always $(INCLUDES) -L. -L..

SOURCES=solver.cpp
OBJECTS=$(SOURCES:.cpp=.o) ../graph.o
TARGET = solver

.PHONY: all
export GRAPH_DEBUG=1
all: $(TARGET)

$(TARGET): $(OBJECTS)

testall: $(TARGET)
	@for input in ../studen*/T*.in ; do \
		./$(TARGET) < $$input > $${input%.in}.outhyp ;\
		diff $${input%.in}.out $${input%.in}.outhyp; \
	done


.PHONY: clean
clean:
	rm -f $(OBJECTS) $(TARGET) vgcore* *outhyp

.PHONY: depend
depend: $(SOURCES)
	$(CC) $(INCLUDES) -MM $(SOURCES) > autodep
	@echo $(SOURCES)

