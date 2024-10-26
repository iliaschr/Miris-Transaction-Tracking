#ifndef GRAPH_H
#define GRAPH_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct edge* Edge;

typedef struct node* Node;

typedef struct graph* Graph;

typedef unsigned int uint;

struct edge 
{
    Node destination;
    int amount;
    char date[11];
    Edge next;
};

struct node 
{
    char* id;
    Edge edges;
    Node next_in_hash;
    bool is_visited; // Not used in graph.c but in miris.c to help with tracking
};

struct graph 
{
    Node* hash_table;
    size_t table_size;
};

/* Function prototypes */

// Create and initialize a new graph with a specified hash table size
Graph graph_create(size_t initial_size);

// Add a node to the graph (if it does not already exist)
Node node_insert(Graph graph, const char* id);

// Find a node in the graph by ID
Node node_find(Graph graph, const char* id);

// Remove a node and all associated edges from the graph
bool node_remove(Graph graph, const char* id);

// Add an edge (transaction) between two nodes
Edge edge_insert(Graph graph, const char* src_id, const char* dest_id, int amount, const char* date);

// Remove a specific edge between two nodes
bool edge_remove(Graph graph, const char* src_id, const char* dest_id);

// Modify an existing edge's amount and date
bool edge_modify(Graph graph, const char* src_id, const char* dest_id, int old_amount, int new_amount, const char* old_date, const char* new_date);

// Free all memory associated with the graph
void graph_destroy(Graph graph);

// Hash function prototype, which will use the dynamic table size
uint hash(const char* id, size_t table_size);

#endif  // GRAPH_H
