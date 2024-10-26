#include "src/Miris/miris.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 256
#define INIT_SIZE 10

// Load graph data from file and insert into the graph
void load_graph(Graph graph, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open input file");
        return;
    }

    int src, dest, weight;
    char date[11];

    while (fscanf(file, "%d %d %d %10s", &src, &dest, &weight, date) == 4) {
        char src_id[12], dest_id[12];
        snprintf(src_id, sizeof(src_id), "%d", src);
        snprintf(dest_id, sizeof(dest_id), "%d", dest);

        miris_insert(graph, src_id);
        miris_insert(graph, dest_id);
        miris_insert2(graph, src_id, dest_id, weight, date);
    }

    fclose(file);
}

// Counts lines in the file for determining graph size
size_t count_lines(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open file for line counting");
        return 0;
    }

    size_t line_count = 0;
    int ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            line_count++;
        }
    }

    fclose(file);
    return line_count;
}

// Helper function to parse and execute commands
void execute_command(Graph graph, char* command) {
    char cmd[16];
    char args[MAX_COMMAND_LENGTH];

    if (sscanf(command, "%15s %[^\n]", cmd, args) < 1) {
        printf("Format error: %s\n", command);
        return;
    }

    // Commands that need output handling here
    if (strcmp(cmd, "i") == 0 || strcmp(cmd, "insert") == 0) {
        char *node_id = strtok(args, " ");
        while (node_id) {
            if (!miris_insert(graph, node_id)) {
                printf("IssueWith: %s\n", node_id);
            } else {
                printf("Succ: %s\n", node_id);
            }
            node_id = strtok(NULL, " ");
        }
    } else if (strcmp(cmd, "n") == 0 || strcmp(cmd, "insert2") == 0) {
        char src[32], dest[32], date[16];
        int amount;
        if (sscanf(args, "%31s %31s %d %15s", src, dest, &amount, date) == 4) {
            if (!miris_insert2(graph, src, dest, amount, date)) {
                printf("IssueWith: %s %s\n", src, dest);
            }
        } else {
            printf("Format error: %s\n", command);
        }
    } else if (strcmp(cmd, "d") == 0 || strcmp(cmd, "delete") == 0) {
        char *node_id = strtok(args, " ");
        while (node_id) {
            if (!miris_delete(graph, node_id)) {
                printf("Non-existing node(s): %s\n", node_id);
            }
            node_id = strtok(NULL, " ");
        }
    } else if (strcmp(cmd, "l") == 0 || strcmp(cmd, "delete2") == 0) {
        char src[32], dest[32];
        if (sscanf(args, "%31s %31s", src, dest) == 2) {
            if (!miris_delete2(graph, src, dest)) {
                printf("Non-existing node(s): %s\n", src);
            }
        } else {
            printf("Format error: %s\n", command);
        }
    } else if (strcmp(cmd, "m") == 0 || strcmp(cmd, "modify") == 0) {
        char src[32], dest[32], old_date[16], new_date[16];
        int old_sum, new_sum;
        if (sscanf(args, "%31s %31s %d %d %15s %15s", src, dest, &old_sum, &new_sum, old_date, new_date) == 6) {
            if (!miris_modify(graph, src, dest, old_sum, new_sum, old_date, new_date)) {
                printf("Non-existing edge: %s %s %d %s\n", src, dest, old_sum, old_date);
            }
        } else {
            printf("Format error: %s\n", command);
        }
    }
    // Commands that handle their own output in miris.c
    else if (strcmp(cmd, "f") == 0 || strcmp(cmd, "find") == 0) {
        miris_find(graph, args);
    } else if (strcmp(cmd, "r") == 0 || strcmp(cmd, "receiving") == 0) {
        miris_receiving(graph, args);
    } else if (strcmp(cmd, "c") == 0 || strcmp(cmd, "circlefind") == 0) {
        miris_circlefind(graph, args);
    } else if (strcmp(cmd, "fi") == 0 || strcmp(cmd, "findcircles") == 0) {
        char node_id[32];
        int min_sum;
        if (sscanf(args, "%31s %d", node_id, &min_sum) == 2) {
            miris_findcircles(graph, node_id, min_sum);
        }
    } else if (strcmp(cmd, "t") == 0 || strcmp(cmd, "traceflow") == 0) {
        char node_id[32];
        int max_length;
        if (sscanf(args, "%31s %d", node_id, &max_length) == 2) {
            miris_traceflow(graph, node_id, max_length);
        }
    } else if (strcmp(cmd, "o") == 0 || strcmp(cmd, "connected") == 0) {
        char src[32], dest[32];
        if (sscanf(args, "%31s %31s", src, dest) == 2) {
            miris_connected(graph, src, dest);
        }
    } else {
        printf("Unrecognized command\n");
    }
}

int main(int argc, char *argv[]) {
    const char *input_filename = NULL;
    const char *output_filename = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            input_filename = argv[++i];
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_filename = argv[++i];
        } else {
            fprintf(stderr, "Usage: %s -i inputfile -o outputfile\n", argv[0]);
            return EXIT_FAILURE;
        }
    }

    size_t initial_size = input_filename ? count_lines(input_filename) : INIT_SIZE;
    Graph graph = graph_create(initial_size);

    if (input_filename) {
        load_graph(graph, input_filename);
    }

    printf("Enter commands (type 'exit' or 'e' to quit):\n");
    char command[MAX_COMMAND_LENGTH];
    while (true) {
        printf("> ");
        if (!fgets(command, sizeof(command), stdin)) break;

        command[strcspn(command, "\n")] = 0;
        if (strcmp(command, "exit") == 0 || strcmp(command, "e") == 0) break;

        execute_command(graph, command);
    }

    miris_exit(graph, output_filename ? output_filename : "output.txt");
    return EXIT_SUCCESS;
}
