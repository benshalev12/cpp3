
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
LDFLAGS =
INCLUDES = -I.

OBJS = main.o Game.o Player.o ActionManager.o
TEST_OBJS = test.o Game.o Player.o ActionManager.o

TARGET = coup_game
TEST_TARGET = test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

test: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $(TEST_OBJS) -o $(TEST_TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f *.o $(TARGET) $(TEST_TARGET)

valgrind: test
	valgrind --leak-check=full --track-origins=yes ./$(TEST_TARGET)