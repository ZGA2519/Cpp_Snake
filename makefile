# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++2b \
           -Wall \
           -Wextra

# Linker flags
LDFLAGS = -L lib/ \
          -framework CoreVideo \
          -framework IOKit \
          -framework Cocoa \
          -framework GLUT \
          -framework OpenGL \
          lib/libraylib.a

# Source files
SRCS = $(wildcard *.cpp)

# Executable files (each source file without .cpp extension)
EXES = $(SRCS:.cpp=)

# Rule to build all executables
all: $(EXES)

# Rule to build each executable
%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

# Clean up generated files
.PHONY: clean
clean:
	rm -f $(EXES)