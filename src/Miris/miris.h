///////////////////////////////////////////////////////////////////////
//
// Miris
//
// This module provides functionalities for managing a directed graph,
// specifically for inserting and deleting nodes and edges, modifying
// existing edges, and finding transactions associated with nodes.
// It also includes functions for detecting cycles and tracing the flow
// of transactions, facilitating comprehensive graph analysis.
//
//////////////////////////////////////////////////////////////////////

#ifndef MIRIS_H
#define MIRIS_H

#include "../Graph/graph.h"

#define INIT_SIZE 10

// Inserts a new node with the specified ID into the graph.
// Returns true if the insertion is successful; false otherwise.
bool miris_insert(Graph graph, const char *id);

// Inserts a directed edge from the source node (src_id) to the destination node
// (dest_id) with the specified amount and date. If either node is not present
// in the graph, they will be added. Returns true if the edge is successfully
// inserted; false otherwise.
bool miris_insert2(Graph graph, const char *src_id, const char *dest_id,
                   int amount, const char *date);

// Deletes the specified node (id) from the graph, including all associated
// edges. Returns true if the deletion is successful; false if the node does not
// exist.
bool miris_delete(Graph graph, const char *id);

// Deletes the directed edge between the source node (src_id) and the
// destination node (dest_id). Returns true if the edge is successfully removed;
// false if either node does not exist or the edge is not found.
bool miris_delete2(Graph graph, const char *src_id, const char *dest_id);

// Modifies an existing edge between the source node (src_id) and destination
// node (dest_id). Updates the amount and date for the first matching edge found
// with the specified old_sum and old_date. Returns true if the edge is
// successfully modified; false if not found.
bool miris_modify(Graph graph, const char *src_id, const char *dest_id,
                  int old_sum, int new_sum, const char *old_date,
                  const char *new_date);

// Finds and displays all external transactions associated with the specified
// node (node_id). Returns true if the node exists and has outgoing edges; false
// otherwise.
bool miris_find(Graph graph, const char *node_id);

// Finds and displays all incoming transactions that the specified node
// (node_id) receives. Returns true if the node exists and has incoming edges;
// false otherwise.
bool miris_receiving(Graph graph, const char *node_id);

// Identifies and displays simple cycles in which the specified node (node_id)
// is involved. Returns true if cycles are found; false otherwise.
bool miris_circlefind(Graph graph, const char *node_id);

// Finds circular transactions involving the specified node (node_id) that meet
// a minimum amount criterion (min_sum). Returns true if circles are found;
// false otherwise.
bool miris_findcircles(Graph graph, const char *node_id, int min_sum);

// Traces the flow of transactions starting from the specified node (node_id) up
// to a maximum length (max_length). Returns true if the tracing operation is
// successful; false if the node does not exist.
bool miris_traceflow(Graph graph, const char *node_id, int max_length);

// Checks if there is a connection (direct or indirect) between two nodes
// (src_id and dest_id). Returns true if a path exists; false otherwise.
bool miris_connected(Graph graph, const char *src_id, const char *dest_id);

// Exits the program and writes the current state of the graph to the specified
// output file (output_filename).
void miris_exit(Graph graph, const char *output_filename);

#endif // MIRIS_H
