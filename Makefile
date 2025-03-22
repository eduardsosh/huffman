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
ifeq ($(OS),Windows_NT)
	@if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"
else
	@mkdir -p "$(OBJ_DIR)"
endif
	@$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
ifeq ($(OS),Windows_NT)
	@if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"
else
	@mkdir -p "$(OBJ_DIR)"
endif
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
ifeq ($(OS),Windows_NT)
	@if exist "$(OBJ_DIR)\*.o" del /F "$(OBJ_DIR)\*.o"
	@if exist "$(TARGET)" del /F "$(TARGET)"
else
	@rm -rf $(OBJ_DIR)/*.o $(TARGET)
endif

.PHONY: all clean
