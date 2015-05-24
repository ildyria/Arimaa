#CC=g++
CC=/home-reseau/pazat/bin/mpic++
BIN=bin
# LIB_AI=lib_ai
# LIB_G=lib_g
LIBRARY=../MCTS_array/lib_ai/ai.a ../MCTS_array/lib_g/game.a
INCLUDE=-I DistributedMCTS
CFLAGS=-g -O3 -Wpadded -fopenmp -c -Wall -std=c++11 $(INCLUDE)
LDFLAGS=-g -O3 -fopenmp -std=c++11

SOURCES = voteAi.cpp WorkerAI.cpp DistributedMCTS.cpp
# SOURCES_AI = $(SOURCES_TOOLS) $(SOURCES_GAMES) $(SOURCES_MCTS) Ai_v2.cpp
# SOURCES_G = $(SOURCES_TOOLS) $(SOURCES_GAMES) Game_v2.cpp


OBJECTS = $(addprefix $(BIN)/, $(SOURCES:.cpp=.o))
# OBJECTS_LIB_AI = $(addprefix $(LIB_AI)/, $(SOURCES_AI:.cpp=.o))
# OBJECTS_LIB_G = $(addprefix $(LIB_G)/, $(SOURCES_G:.cpp=.o))

EXECUTABLE=$(BIN)/main
# LIBRARY_AI=$(LIB_AI)/ai.a
# LIBRARY_G=$(LIB_G)/game.a

vpath %.cpp DistributedMCTS




all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBRARY) -o $@

-include $(BIN)/$(OBJECTS:.o=.d)

$(BIN)/%.o: %.cpp
	$(CC) -c $(CFLAGS) $(LIBRARY) $< -o $(BIN)/$*.o
	$(CC) -MM $(CFLAGS) $< > $(BIN)/$*.d 
	@mv -f $(BIN)/$*.d $(BIN)/$*.d.tmp
	@sed -e 's|.*:|$(BIN)/$*.o:|' < $(BIN)/$*.d.tmp > $(BIN)/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $(BIN)/$*.d.tmp | fmt -1 | \
	sed -e 's/^ *//' -e 's/$$/:/' >> $(BIN)/$*.d
	@rm -f $(BIN)/$*.d.tmp




clean:
	rm -f $(BIN)/*.o $(BIN)/*.d $(EXECUTABLE)




# ai: $(LIBRARY_AI)

# $(LIBRARY_AI): $(OBJECTS_LIB_AI)
# 	ar rcs $@ $(OBJECTS_LIB_AI)

# -include $(LIB_AI)/$(OBJECTS_LIB_AI:.o=.d)

# $(LIB_AI)/%.o: %.cpp
# 	$(CC) -c $(CFLAGS) $< -o $(LIB_AI)/$*.o
