SRC = .
INCLUDE = $(SRC)/Graph
MIRIS_INCLUDE = $(SRC)/Miris
TEST_SRC = $(SRC)/Test

CC = gcc
CCFLAGS = -Wall -Werror -I$(INCLUDE) -I$(MIRIS_INCLUDE)
LDFLAGS =

# Source files
GRAPH_SRC = $(SRC)/Graph/graph.c
MIRIS_SRC = $(SRC)/Miris/miris.c
MAIN_SRC = $(SRC)/main.c

# Object files
MAIN_OBJS = $(MAIN_SRC:.c=.o) $(GRAPH_SRC:.c=.o) $(MIRIS_SRC:.c=.o)
GRAPH_TEST_OBJS = $(TEST_SRC)/graph_test.o $(GRAPH_SRC:.c=.o)
MIRIS_TEST_OBJS = $(TEST_SRC)/miris_test.o $(GRAPH_SRC:.c=.o) $(MIRIS_SRC:.c=.o)

# Executables
MAIN_EXEC = main_exec
GRAPH_TEST_EXEC = graph_test_exec
MIRIS_TEST_EXEC = miris_test_exec

# Default target: build main program and tests
all: $(MAIN_EXEC) $(GRAPH_TEST_EXEC) $(MIRIS_TEST_EXEC)

# Target to build and run main program
run: $(MAIN_EXEC)
	./$(MAIN_EXEC)

# Targets to build and run individual tests
graph_test: $(GRAPH_TEST_EXEC)
	./$(GRAPH_TEST_EXEC)

miris_test: $(MIRIS_TEST_EXEC)
	./$(MIRIS_TEST_EXEC)

# Compile and link main program
$(MAIN_EXEC): $(MAIN_OBJS)
	$(CC) $(MAIN_OBJS) -o $(MAIN_EXEC)

# Compile and link graph_test
$(GRAPH_TEST_EXEC): $(GRAPH_TEST_OBJS)
	$(CC) $(GRAPH_TEST_OBJS) -o $(GRAPH_TEST_EXEC)

# Compile and link miris_test
$(MIRIS_TEST_EXEC): $(MIRIS_TEST_OBJS)
	$(CC) $(MIRIS_TEST_OBJS) -o $(MIRIS_TEST_EXEC)

# Clean all compiled files, executables, and output .txt files
# Clean all compiled files, executables, and output .txt files
clean:
	rm -f $(MAIN_OBJS) $(GRAPH_TEST_OBJS) $(MIRIS_TEST_OBJS) $(MAIN_EXEC) $(GRAPH_TEST_EXEC) $(MIRIS_TEST_EXEC) $(TEST_SRC)/test_output.txt output.txt

# Valgrind check for memory leaks on both tests
valgrind: $(GRAPH_TEST_EXEC) $(MIRIS_TEST_EXEC)
	valgrind --leak-check=full --show-leak-kinds=all ./$(GRAPH_TEST_EXEC)
	valgrind --leak-check=full --show-leak-kinds=all ./$(MIRIS_TEST_EXEC)

# Pattern rule for compiling .o files from .c files
%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@
