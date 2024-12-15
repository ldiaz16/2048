# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = `wx-config --cxxflags` -std=c++17
# Linker flags
LDFLAGS = `wx-config --libs`
# Target executable
TARGET = Game2048

# Source files
SRCS = 2048.cpp main.cpp

# Object files (each .cpp file corresponds to a .o file)
OBJS = $(SRCS:.cpp=.o)

# Default rule
all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

# Rule to link the target executable
$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Rule to compile .cpp files to .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remove generated files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
