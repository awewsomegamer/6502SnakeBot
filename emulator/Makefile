CXX      := g++
CXXFLAGS := -O2 -Wall -Wextra -std=c++20 -g
LDFLAGS  :=
LDLIBS   := $(shell pkg-config --libs sdl2 SDL2_ttf)
CPPFLAGS := $(shell pkg-config --cflags sdl2)

TARGET := emulator
SRC    := main.cpp Game.cpp GameRender.cpp mos6502.cpp Emulator.cpp
OBJ    := $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS) $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
