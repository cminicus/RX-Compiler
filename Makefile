##########################
# Makefile for RX Compiler
# Clayton Minicus
# February 29th, 2016
#
##########################

##########################
# paths
##########################
# where to look for .cpp files
SRC_DIR = src
# where to look for .o files
BUILD_DIR = build
# where to look for test files
TEST_DIR = test

# main target
TARGET = bin/compiler
# test target
TEST_TARGET = bin/test
# object file with main
MAIN = build/main.o

# find source files and object files
SRC_EXT = cpp
SOURCES = $(shell find $(SRC_DIR) -type f -name *.$(SRC_EXT))
OBJECTS_MAIN = $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(SOURCES:.$(SRC_EXT)=.o))
# separate out main so we can include this in the test files
OBJECTS = $(filter-out $(MAIN),$(OBJECTS_MAIN))

TEST_SOURCES = $(shell find $(TEST_DIR) -type f -name *.$(SRC_EXT))
TEST_OBJECTS = $(patsubst $(TEST_DIR)/%,$(BUILD_DIR)/%,$(TEST_SOURCES:.$(SRC_EXT)=.o))

##########################
# universal flags
##########################
# c++ compiler
CXX = g++

# standard compile flags
CXXFLAGS = -std=c++14 -Wall -Wextra -pedantic -O0 -g #-Werror

# include files
INC = -I include

##########################
# executable targets
##########################

# default target
$(TARGET): $(OBJECTS) $(MAIN)
	@echo "Linking..."
	@echo $(MAIN)
	@echo "$(CXX) $^ -o $(TARGET)"; $(CXX) $^ -o $(TARGET)

# test target
test: $(TEST_OBJECTS) $(OBJECTS)
	@echo "Linking..."
	@echo "$(CXX) $^ -o $(TEST_TARGET)"; $(CXX) $^ -o $(TEST_TARGET)
	@echo "Running tests...";
	./bin/test

# object build rules
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.$(SRC_EXT)
	@mkdir -p $(BUILD_DIR)
	@echo "$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<"; $(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.$(SRC_EXT)
	@mkdir -p $(BUILD_DIR)
	@echo "$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<"; $(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<

# clean generated files
clean:
	@echo "Cleaning..."; 
	@echo "$(RM) -r $(BUILD_DIR) $(TARGET)"; $(RM) -r $(BUILD_DIR) $(TARGET) $(TEST_TARGET)

.PHONY: clean
