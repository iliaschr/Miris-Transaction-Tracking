#ifndef MIRIS_H
#define MIRIS_H

#include "../Graph/graph.h"

#define INIT_SIZE 10

// Insert new Ni or multiple new nodes like Ni, Nj, Nk
bool miris_insert(Graph graph, const char* id);

// Insert one edge from Ni to Nj with date and sum. Ni and Nj can
// appear for the first time in the graph
bool miris_insert2(Graph graph, const char* src_id, const char* dest_id, int amount, const char* date);

// Delete from the graph Ni or multiple nodes Ni, Nj, Nk, ...
// All receiving/outgoing edges are deleted
bool miris_delete(Graph graph, const char* id);

// Delete the edge between Ni and Nj, if there are multiple delete one
// of them
bool miris_delete2(Graph graph, const char* src_id, const char* dest_id);

// Change the sum and the date if there multiple edges with the same sum value
// change the first one you find
bool miris_modify(Graph graph, const char* src_id, const char* dest_id, int old_sum, int new_sum, const char* old_date, const char* new_date);

// Find all the external transactions that this node has
bool miris_find(Graph graph, const char* node_id);

// Find all the transactions Ni receives as input
bool miris_receiving(Graph graph, const char* node_id);

// Finds if node Ni is involved in simple circles and show those circles
bool miris_circlefind(Graph graph, const char* node_id);

// Find if Ni is involved in other circular transactions with others
// but the min ammount its edge must have is k.
bool miris_findcircles(Graph graph, const char* node_id, int min_sum);

// 
bool miris_traceflow(Graph graph, const char* node_id, int max_length);

bool miris_connected(Graph graph, const char* src_id, const char* dest_id);

void miris_exit(Graph graph, const char* output_filename);

#endif // MIRIS_H
