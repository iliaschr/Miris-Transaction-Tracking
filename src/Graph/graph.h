///////////////////////////////////////////////////////////////////
//
// Graph
//
// A directed graph. It provides functionalities for adding/removing
// nodes and edges, finding nodes and modifying edges.
// It also uses a hash function to achieve better complexity.
//
///////////////////////////////////////////////////////////////////

#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// An edge is represented by the Edge type. Users do not need to know
// the contents of this type; they simply use the functions edge_<foo>.
typedef struct edge *Edge;

// Same for a node.
typedef struct node *Node;

// And for a graph.
typedef struct graph *Graph;

// Added as a common type for the hash function.
typedef unsigned int uint;

// An Edge is a pointer to this struct.
struct edge {
  Node destination; // a pointer to the Node this edge points to.
  int amount;       // representing the weight or the value
  char date[11];    // a string representing the date
  Edge next; // a pointer to the next Edge in the list, allowing for multiple
             // edges from the same source node.
};

// A Node is a pointer to this struct.
struct node {
  char *id;          // A unique identifier for the Node
  Edge edges;        // Points to the first edge associated with this Node.
  Node next_in_hash; // A pointer for the next Node in the hash table.
  bool is_visited;   // Indicates whether a Node has been visited during
                     // traversal, used in miris.c
};

// A Graph is a pointer to this struct.
struct graph {
  Node *hash_table; // An array of pointers to Nodes, representing the hash
                    // table for quick access to Nodes.
  size_t table_size;
};

// Creates and returns a new graph of size initial_size, initializing the
// hash table with NULL pointers to indicate that no nodes exist yet.
// Memory for the graph structure and its hash table is allocated
// dynamically, and the function will return NULL if memory allocation fails.
Graph graph_create(size_t initial_size);

// Adds a node with the given id to the graph. If a node with the same
// id already exists, it returns the existing node. The function returns
// NULL if the operation fails.
Node node_insert(Graph graph, const char *id);

// Finds and returns a node with the specified id from the graph.
// If the node is not found, it returns NULL.
Node node_find(Graph graph, const char *id);

// Removes a node with the specified id from the graph along with
// all its associated edges. Returns true if the node was removed
// successfully, false otherwise.
bool node_remove(Graph graph, const char *id);

// Adds an edge (transaction) between two nodes identified by src_id
// and dest_id with the specified amount and date. Returns a pointer
// to the new edge if successful, or NULL if it fails.
Edge edge_insert(Graph graph, const char *src_id, const char *dest_id,
                 int amount, const char *date);

// Removes a specific edge between the nodes identified by src_id
// and dest_id. Returns true if the edge was successfully removed,
// false otherwise.
bool edge_remove(Graph graph, const char *src_id, const char *dest_id);

// Modifies an existing edge's amount and date based on the provided
// parameters. Returns true if successful, false otherwise.
bool edge_modify(Graph graph, const char *src_id, const char *dest_id,
                 int old_amount, int new_amount, const char *old_date,
                 const char *new_date);

// Free all memory associated with the graph
void graph_destroy(Graph graph);

// Hash function
uint hash(const char *id, size_t table_size);

#endif // GRAPH_H
