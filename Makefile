
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SRC = main.cpp Player.cpp Game.cpp Governor.cpp Spy.cpp Baron.cpp General.cpp Judge.cpp Merchant.cpp
OBJ = $(SRC:.cpp=.o)

TARGET = coup_gui

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)
