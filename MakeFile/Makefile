# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11

# Directories
SRC_DIR = ../SourceFiles/
INC_DIR = ../HeaderFiles/
LIB_DIR = ../Libraries/

# Source and object files for main.cpp and Sphere
SRC = $(SRC_DIR)/main.cpp $(SRC_DIR)/Sphere.cpp $(LIB_DIR)/glad.c
OBJ = Main.o Sphere.o glad.o
LIBS = -dl -lglfw -lassimp

# Output file
OUTPUT = ../light_sphere_face

# Default rule
all: $(OUTPUT)

# Rule to compile main.cpp into sphereMain.o
Main.o: $(SRC_DIR)/main.cpp $(INC_DIR)/Shader.h $(INC_DIR)/Camera.h $(INC_DIR)/stb_image.h  $(INC_DIR)/Sphere.h
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $(SRC_DIR)/main.cpp -o Main.o

# Rule to compile Sphere.cpp into Sphere.o
Sphere.o: $(SRC_DIR)/Sphere.cpp $(INC_DIR)/Sphere.h
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $(SRC_DIR)/Sphere.cpp -o Sphere.o

# Rule to compile glad.c into glad.o
glad.o: $(LIB_DIR)/glad.c
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $(LIB_DIR)/glad.c -o glad.o

# Link the object files to create the main2 executable
$(OUTPUT): $(OBJ)
	$(CXX) $(OBJ) $(LIBS) -o $(OUTPUT)

# Clean up object and executable files
clean:
	rm -f $(OBJ) $(OUTPUT)
