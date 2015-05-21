CC=g++
BIN=bin
INCLUDE=-I src/connect4 -I src/interfaces -I src/mcts -I src/tools -I src/benching -I src/api -I src/api_v2 -I src/arimaa_simple -I src
CFLAGS=-g -O3 -Wpadded -fopenmp -c -Wall -std=c++11 $(INCLUDE)
LDFLAGS=-g -O3 -fopenmp -std=c++11

SOURCES_TOOLS = Random.cpp Count.cpp Bench.cpp Graph.cpp typedef.cpp 
SOURCES_API = Ai.cpp Game.cpp test_api.cpp 
SOURCES_GAMES = BitboardConnect4.cpp Connect4.cpp Bitboard.cpp Arimaa.cpp Move.cpp
SOURCES_MCTS = Mcts.cpp Node.cpp
SOURCES = $(SOURCES_TOOLS) $(SOURCES_API) $(SOURCES_GAMES) $(SOURCES_MCTS) main.cpp

OBJECTS = $(addprefix $(BIN)/, $(SOURCES:.cpp=.o))
EXECUTABLE=$(BIN)/main
vpath %.cpp src:src/connect4:src/arimaa_simple:src/tools:src/tools/benching:src/mcts:src/interfaces:src/api:src/api_v2

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
