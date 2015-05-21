CC=g++
BIN=bin
LIB_AI=lib_ai
LIB_G=lib_g
INCLUDE=-I src/connect4 -I src/interfaces -I src/mcts -I src/tools -I src/benching -I src/api -I src/api_v2 -I src/arimaa_simple -I src
CFLAGS=-g -O3 -Wpadded -fopenmp -c -Wall -std=c++11 $(INCLUDE)
LDFLAGS=-g -O3 -fopenmp -std=c++11

SOURCES_TOOLS = Random.cpp Count.cpp Bench.cpp Graph.cpp typedef.cpp 
SOURCES_API_V1 = Ai.cpp Game.cpp test_api.cpp
SOURCES_API_V2 =Ai_v2.cpp Game_v2.cpp test_v2.cpp
SOURCES_GAMES = BitboardConnect4.cpp Connect4.cpp Bitboard.cpp Arimaa.cpp Move.cpp
SOURCES_MCTS = Mcts.cpp Node.cpp
SOURCES = $(SOURCES_TOOLS) $(SOURCES_API_V2) $(SOURCES_GAMES) $(SOURCES_MCTS) main.cpp
SOURCES_AI = $(SOURCES_TOOLS) $(SOURCES_GAMES) $(SOURCES_MCTS) Ai_v2.cpp
SOURCES_G = $(SOURCES_TOOLS) $(SOURCES_GAMES) Game_v2.cpp


OBJECTS = $(addprefix $(BIN)/, $(SOURCES:.cpp=.o))
OBJECTS_LIB_AI = $(addprefix $(LIB_AI)/, $(SOURCES_AI:.cpp=.o))
OBJECTS_LIB_G = $(addprefix $(LIB_G)/, $(SOURCES_G:.cpp=.o))

EXECUTABLE=$(BIN)/main
LIBRARY_AI=$(LIB_AI)/ai.a
LIBRARY_G=$(LIB_G)/game.a

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
	rm -f $(LIB_AI)/*.o $(LIB_AI)/*.a
	rm -f $(LIB_G)/*.o $(LIB_G)/*.a




ai: $(LIBRARY_AI)

$(LIBRARY_AI): $(OBJECTS_LIB_AI)
	ar rcs $@ $(OBJECTS_LIB_AI)

-include $(LIB_AI)/$(OBJECTS_LIB_AI:.o=.d)

$(LIB_AI)/%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $(LIB_AI)/$*.o





game: $(LIBRARY_G)

$(LIBRARY_G): $(OBJECTS_LIB_G)
	ar rcs $@ $(OBJECTS_LIB_G)

-include $(LIB_G)/$(OBJECTS_LIB_G:.o=.d)

$(LIB_G)/%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $(LIB_G)/$*.o

