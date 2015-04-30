CC=g++
BIN=bin
INCLUDE=-I src/
CFLAGS=-O2 -fopenmp -c -g -Wall -std=c++11 $(INCLUDE)
LDFLAGS=-O2 -g -fopenmp -std=c++11
SOURCES=Graph.cpp Bench.cpp main.cpp
OBJECTS=$(addprefix $(BIN)/, $(SOURCES:.cpp=.o))
EXECUTABLE=$(BIN)/plaque
vpath %.cpp src:src/

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

-include $(BIN)/$(OBJECTS:.o=.d)

$(BIN)/%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $(BIN)/$*.o
	$(CC) -MM $(CFLAGS) $< > $(BIN)/$*.d 
	@mv -f $(BIN)/$*.d $(BIN)/$*.d.tmp
	@sed -e 's|.*:|$(BIN)/$*.o:|' < $(BIN)/$*.d.tmp > $(BIN)/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $(BIN)/$*.d.tmp | fmt -1 | \
	sed -e 's/^ *//' -e 's/$$/:/' >> $(BIN)/$*.d
	@rm -f $(BIN)/$*.d.tmp

clean:
	rm -f $(BIN)/*.o $(BIN)/*.d $(EXECUTABLE)
