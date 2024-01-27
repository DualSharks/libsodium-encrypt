# Makefile

# Compiler and compiler flags
CXX = g++
CXXFLAGS = -Wall -I/opt/homebrew/opt/libsodium/include
LDFLAGS = -L/opt/homebrew/opt/libsodium/lib -lsodium

# Define the executable name
TARGET = encryptor

# Define the source files
SRC = main.cpp src/FileEncryptor.cpp

# Define the object files directory
OBJDIR = obj

# Define the object files
OBJ = $(SRC:%.cpp=$(OBJDIR)/%.o)

# Default target
all: $(TARGET)

# Rule for making the executable
$(TARGET): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^

# Rule for making object files
$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJ) $(TARGET)
	rm -rf $(OBJDIR)




