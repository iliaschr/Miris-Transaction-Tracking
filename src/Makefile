# Project directories
SRC = .
INCLUDE = $(SRC)/Graph
MIRIS_INCLUDE = $(SRC)/Miris
TEST_SRC = $(SRC)/Test

# Compiler settings
CC = gcc
CCFLAGS = -Wall -Werror -I$(INCLUDE) -I$(MIRIS_INCLUDE)

# Source files
GRAPH_SRC = $(SRC)/Graph/graph.c
MIRIS_SRC = $(SRC)/Miris/miris.c
MAIN_SRC = $(SRC)/main.c
GRAPH_TEST_SRC = $(TEST_SRC)/graph_test.c
MIRIS_TEST_SRC = $(TEST_SRC)/miris_test.c

# Object files
GRAPH_OBJ = $(GRAPH_SRC:.c=.o)
MIRIS_OBJ = $(MIRIS_SRC:.c=.o)
MAIN_OBJ = $(MAIN_SRC:.c=.o)
GRAPH_TEST_OBJ = $(GRAPH_TEST_SRC:.c=.o)
MIRIS_TEST_OBJ = $(MIRIS_TEST_SRC:.c=.o)

# Executables
MAIN_EXEC = miris               # Main executable name
GRAPH_TEST_EXEC = graph_test_exec
MIRIS_TEST_EXEC = miris_test_exec

# Default target: build main program and tests
all: $(MAIN_EXEC) $(GRAPH_TEST_EXEC) $(MIRIS_TEST_EXEC)

# Target to build and run main program with custom arguments
build_miris: $(MAIN_EXEC)  # Changed target name to avoid conflict
	./$(MAIN_EXEC)

# Target to run main program (without specific arguments)
run: $(MAIN_EXEC)
	./$(MAIN_EXEC)

# Targets to build and run individual tests
graph_test: $(GRAPH_TEST_EXEC)
	./$(GRAPH_TEST_EXEC)

miris_test: $(MIRIS_TEST_EXEC)
	./$(MIRIS_TEST_EXEC)

# Valgrind targets for tests
valgrind_graph_test: $(GRAPH_TEST_EXEC)
	valgrind --leak-check=full --show-leak-kinds=all ./$(GRAPH_TEST_EXEC)

valgrind_miris_test: $(MIRIS_TEST_EXEC)
	valgrind --leak-check=full --show-leak-kinds=all ./$(MIRIS_TEST_EXEC)

valgrind_test: valgrind_graph_test valgrind_miris_test

# Compile and link main program
$(MAIN_EXEC): $(MAIN_OBJ) $(GRAPH_OBJ) $(MIRIS_OBJ)
	$(CC) $(MAIN_OBJ) $(GRAPH_OBJ) $(MIRIS_OBJ) -o $(MAIN_EXEC)

# Compile and link graph_test
$(GRAPH_TEST_EXEC): $(GRAPH_TEST_OBJ) $(GRAPH_OBJ)
	$(CC) $(GRAPH_TEST_OBJ) $(GRAPH_OBJ) -o $(GRAPH_TEST_EXEC)

# Compile and link miris_test
$(MIRIS_TEST_EXEC): $(MIRIS_TEST_OBJ) $(GRAPH_OBJ) $(MIRIS_OBJ)
	$(CC) $(MIRIS_TEST_OBJ) $(GRAPH_OBJ) $(MIRIS_OBJ) -o $(MIRIS_TEST_EXEC)

# Clean all compiled files, executables, and output .txt files
clean:
	rm -f $(MAIN_OBJ) $(GRAPH_OBJ) $(MIRIS_OBJ) $(GRAPH_TEST_OBJ) $(MIRIS_TEST_OBJ) \
	$(MAIN_EXEC) $(GRAPH_TEST_EXEC) $(MIRIS_TEST_EXEC) $(TEST_SRC)/test_output.txt output.txt

# Pattern rule for compiling .o files from .c files
%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@
