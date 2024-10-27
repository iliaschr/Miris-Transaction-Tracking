#include "miris.h"

bool miris_insert(Graph graph, const char *id) {
  Node node = node_insert(graph, id);
  return (node != NULL); // Check if the node was inserted successfully.
}

bool miris_insert2(Graph graph, const char *src_id, const char *dest_id,
                   int amount, const char *date) {
  // If either node does not exist, they are created.
  Node src_node = node_insert(graph, src_id);
  Node dest_node = node_insert(graph, dest_id);
  if (!src_node || !dest_node) {
    return false;
  }

  Edge edge = edge_insert(graph, src_id, dest_id, amount, date);
  return (edge != NULL); // Check if the edge was inserted successfully.
}

bool miris_delete(Graph graph, const char *id) {
  if (!node_find(graph, id)) {
    return false;
  }

  node_remove(graph, id);
  return true;
}

bool miris_delete2(Graph graph, const char *src_id, const char *dest_id) {
  Node src_node = node_find(graph, src_id);
  if (!src_node || !edge_remove(graph, src_id, dest_id)) {
    return false;
  }

  return true;
}

bool miris_modify(Graph graph, const char *src_id, const char *dest_id,
                  int old_sum, int new_sum, const char *old_date,
                  const char *new_date) {
  return edge_modify(graph, src_id, dest_id, old_sum, new_sum, old_date,
                     new_date);
}

bool miris_find(Graph graph, const char *node_id) {
  Node node = node_find(graph, node_id);
  if (!node) {
    printf("Non-existing node: %s\n", node_id);
    return false;
  }

  Edge edge = node->edges;
  bool has_edges = false; // Track if the node has any outgoing edges.
  while (edge) {
    printf("%s %s %d %s\n", node_id, edge->destination->id, edge->amount,
           edge->date);
    edge = edge->next;
    has_edges = true;
  }

  return has_edges;
}

bool miris_receiving(Graph graph, const char *node_id) {
  Node target_node = node_find(graph, node_id);
  if (!target_node) {
    printf("Non-existing node: %s\n", node_id);
    return false;
  }

  bool has_incoming = false;
  for (size_t i = 0; i < graph->table_size; i++) {
    Node current = graph->hash_table[i];
    while (current) {
      Edge edge = current->edges;
      while (edge) {
        if (edge->destination == target_node) {
          printf("%s %s %d %s\n", current->id, node_id, edge->amount,
                 edge->date);
          has_incoming = true;
        }
        edge = edge->next;
      }
      current = current->next_in_hash;
    }
  }

  return has_incoming;
}

// Perform depth-first search to find cycles starting from start. If a cycle is
// found print it.
static void dfs_find_cycles(Node start, Node current, Node **path,
                            int *path_length, size_t max_path_length) {
  if (*path_length >= max_path_length) {
    return; // Exit if the path is too long.
  }

  current->is_visited = true;
  (*path)[(*path_length)++] = current; // Adds current node to path.
  Edge edge = current->edges;
  while (edge) {
    if (edge->destination == start) {
      for (int i = 0; i < *path_length; i++) {
        printf("%s, ", (*path)[i]->id);
      }
      printf("%s\n", start->id);
    } else if (!edge->destination->is_visited) {
      dfs_find_cycles(start, edge->destination, path, path_length,
                      max_path_length); // Recursively explore further.
    }
    edge = edge->next;
  }

  (*path_length)--; // Backtrack remove current node
  current->is_visited = false;
}

bool miris_circlefind(Graph graph, const char *node_id) {
  Node start_node = node_find(graph, node_id);
  if (!start_node) {
    printf("Non-existing node: %s\n", node_id);
    return false;
  }

  size_t max_path_length =
      graph->table_size; // Decided its better to base the size of the path on
                         // the table size to avoid reallocs
  Node *path = malloc(max_path_length * sizeof(Node));
  if (!path) {
    return false;
  }

  int path_length = 0;
  dfs_find_cycles(start_node, start_node, &path, &path_length, max_path_length);

  free(path);
  return true;
}

// Perform depth-first search to find circular transactions with a min edge sum.
// If a cycle is found print it.
static void dfs_find_circles_with_min_sum(Node start, Node current, Node **path,
                                          int *path_length, int min_sum,
                                          size_t max_path_length) {
  if (*path_length >= max_path_length) {
    return; // Exit if the path is too long.
  }

  current->is_visited = true;
  (*path)[(*path_length)++] = current;
  Edge edge = current->edges;
  while (edge) {
    if (edge->destination == start && edge->amount >= min_sum) {
      int min_cycle_sum = edge->amount;
      for (int i = 0; i < *path_length - 1; i++) {
        if ((*path)[i]->edges && (*path)[i]->edges->amount < min_cycle_sum) {
          min_cycle_sum = (*path)[i]->edges->amount;
        }
      }

      if (min_cycle_sum >= min_sum) {
        for (int i = 0; i < *path_length; i++) {
          printf("%s, ", (*path)[i]->id);
        }
        printf("%s %d\n", start->id, min_cycle_sum);
      }
    } else if (!edge->destination->is_visited && edge->amount >= min_sum) {
      dfs_find_circles_with_min_sum(start, edge->destination, path, path_length,
                                    min_sum, max_path_length);
    }
    edge = edge->next;
  }

  (*path_length)--;
  current->is_visited = false;
}

bool miris_findcircles(Graph graph, const char *node_id, int min_sum) {
  Node start_node = node_find(graph, node_id);
  if (!start_node) {
    printf("Non-existing node: %s\n", node_id);
    return false;
  }

  size_t max_path_length = graph->table_size;
  Node *path = malloc(max_path_length * sizeof(Node));
  if (!path) {
    return false;
  }

  int path_length = 0;
  dfs_find_circles_with_min_sum(start_node, start_node, &path, &path_length,
                                min_sum, max_path_length);

  free(path);
  return true;
}

bool is_in_path(Node node, Node *path[], int path_length) {
  for (int i = 0; i < path_length; i++) {
    if ((*path)[i] == node) {
      return true;
    }
  }
  return false;
}

// Depth-first search to recursively trace the flow of transactions starting
// from current.
static void dfs_traceflow(Node current, Node **path, int path_length,
                          int max_length) { // Max length is considere the number of edges for example 1 - 2 - 5 has a length of 2
  if (path_length > 0) {
    for (int i = 0; i < path_length; i++) {
      printf("%s", (*path)[i]->id);
      if (i < path_length - 1) {
        printf(", ");
      }
    }
    printf("\n");
  }

  if (path_length >= max_length + 1) {
    return;
  }

  current->is_visited = true;

  Edge edge = current->edges;
  while (edge) {
    Node destination = edge->destination;
    if (!is_in_path(destination, path, path_length)) {
      (*path)[path_length] = destination;
      dfs_traceflow(destination, path, path_length + 1, max_length);
    }
    edge = edge->next;
  }

  current->is_visited = false;
}

bool miris_traceflow(Graph graph, const char *node_id, int max_length) {
  Node start_node = node_find(graph, node_id);
  if (!start_node) {
    printf("Non-existing node: %s\n", node_id);
    return false;
  }

  Node *path = malloc((max_length + 1) * sizeof(Node));
  if (!path) {
    return false;
  }

  path[0] = start_node;
  dfs_traceflow(start_node, &path, 1, max_length);

  free(path);
  path = NULL;
  return true;
}

// Depth-first search to find path from current to target.
static bool dfs_find_path(Node current, Node target, Node *path,
                          int path_length) {
  if (!current || !target) {
    return false;
  }

  current->is_visited = true;

  path[path_length] = current;
  path_length++;
  if (current == target) {
    for (int i = 0; i < path_length; i++) {
      printf("%s", path[i]->id);
      if (i < path_length - 1) {
        printf(", ");
      }
    }
    printf("\n");
    return true;
  }

  // Recursively visit each edge from the current node.
  Edge edge = current->edges;
  while (edge) {
    if (edge->destination != NULL &&
        !edge->destination->is_visited) { // Has it been visited?
      if (dfs_find_path(edge->destination, target, path, path_length)) {
        return true;
      }
    }
    edge = edge->next;
  }

  path_length--; // Backtrack.
  current->is_visited = false;
  return false;
}

bool miris_connected(Graph graph, const char *src_id, const char *dest_id) {
  Node src_node = node_find(graph, src_id);
  Node dest_node = node_find(graph, dest_id);

  if (!src_node) {
    printf("Non-existing node: %s\n", src_id);
    return false;
  }
  if (!dest_node) {
    printf("Non-existing node: %s\n", dest_id);
    return false;
  }

  Node *path = malloc(INIT_SIZE * sizeof(Node));
  if (!path) {
    return false;
  }

  bool found_path = dfs_find_path(src_node, dest_node, path, 0);

  for (size_t i = 0; i < graph->table_size; i++) {
    Node node = graph->hash_table[i];
    while (node) {
      node->is_visited = false;
      node = node->next_in_hash;
    }
  }

  free(path);
  path = NULL;
  return found_path;
}

void miris_exit(Graph graph, const char *output_filename) {
  if (!graph) {
    return;
  }

  FILE *output_file = fopen(output_filename, "w");
  if (!output_file) {
    return;
  }

  size_t bytes_released = 0;

  for (size_t i = 0; i < graph->table_size; i++) {
    Node current = graph->hash_table[i];
    while (current) {
      fprintf(output_file, "Node %s:\n", current->id); // Print node id.
      Edge edge = current->edges;
      while (edge) {
        fprintf(output_file, "  Edge %s (with amount: %d,and date: %s)\n",
                edge->destination->id, edge->amount, edge->date);
        edge = edge->next;
      }
      current = current->next_in_hash;
    }
  }

  for (size_t i = 0; i < graph->table_size; i++) {
    Node current = graph->hash_table[i];
    while (current) {
      Edge edge = current->edges;
      while (edge) {
        Edge next_edge = edge->next;
        bytes_released += sizeof(Edge);
        free(edge);
        edge = next_edge;
      }
      bytes_released += strlen(current->id) + 1;
      free(current->id);
      Node next_node = current->next_in_hash;
      bytes_released += sizeof(Node);
      free(current);
      current = next_node;
    }
    graph->hash_table[i] = NULL; // Clear the hash table.
  }
  bytes_released += sizeof(Node) * graph->table_size;
  free(graph->hash_table);
  bytes_released += sizeof(Graph);
  free(graph);
  fclose(output_file);
  printf("%zu Bytes released\n", bytes_released);
}