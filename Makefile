CXX       := g++            # The C++ compiler to use
CXXFLAGS  := -Wall -Wextra -std=c++17 -Iinclude -g
SRC_DIR   := src
OBJ_DIR   := bin
TARGET    := $(OBJ_DIR)/my_program

SOURCES   := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS   := $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(TARGET)
	@$(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(OBJ_DIR)
	@$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)

.PHONY: all clean
