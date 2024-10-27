#include "graph.h"

static Node find_node_in_hash_table(Graph graph, const char* id) 
{
    uint index = hash(id, graph->table_size); // Calculate the index for the hash table.
    Node current = graph->hash_table[index];  // Get the current node at that index.

    while (current) 
    {
        if (strcmp(current->id, id) == 0) 
        { 
            return current; // Return the found node.
        }
        current = current->next_in_hash; // Move to the next node.
    }
    
    return NULL; // Return NULL if not found.
}

Graph graph_create(size_t initial_size) 
{
    // Allocate memory for the struct.
    Graph graph = (Graph)malloc(sizeof(struct graph));
    if (!graph) 
    {
        // Handle memory allocation failure.
        // Return NULL to indicate failure.
        perror("Memory allocation failed.");
        return NULL;
    }

    graph->table_size = initial_size;

    // Allocate memory for the hash table.
    graph->hash_table = (Node*)calloc(initial_size, sizeof(Node));
    if (!graph->hash_table)
    {
        // Handle memory allocation failure.
        // Free graph and return NULL to indicate failure.
        perror("Memory allocation failed.");
        free(graph);
        graph = NULL;
        return NULL;
    }

    return graph;   // Return the newly created graph.
}

Node node_insert(Graph graph, const char* id) 
{ 
    if (!graph || !id)
    {
        return NULL; // Invalid input return NULL.
    }

    // Calculate the hash index for the node.
    uint index = hash(id, graph->table_size);

    Node current = find_node_in_hash_table(graph, id);
    if (current) {
        return current; // Node exists so we return it.
    }

    // Creating new node since it does not exist.
    Node new_node = (Node)malloc(sizeof(struct node));
    if (!new_node)
    {
        // Handle memory allocation failure.
        // Return NULL to indicate failure.
        perror("Memory allocation failed.");
        return NULL;
    }

    new_node->id = strdup(id);                          // Duplicate the ID string.
    new_node->edges = NULL;                               // Initialize edges to NULL.
    new_node->next_in_hash = graph->hash_table[index];    // Link to the current first node.
    new_node->is_visited = false;                         // Initialize as not visited
    graph->hash_table[index] = new_node;                  // Insert the new node into the hash table.

    return new_node;
}

Node node_find(Graph graph, const char* id) 
{
    if (!graph || !id)
    {
        // Invalid input return NULL.
        return NULL;
    }

    return find_node_in_hash_table(graph, id);
}

// Removes all edges pointing to the specified node.
static void remove_edges_to_node(Graph graph, const char* id) 
{
    for (size_t i = 0; i < graph->table_size; i++) 
    {
        Node other = graph->hash_table[i];
        while (other) 
        {
            edge_remove(graph, other->id, id); // Safely remove edges pointing to this node.
            other = other->next_in_hash;
        }
    }
}

// Frees all edges associated with the specified node.
static void free_node_edges(Node current) 
{
    Edge edge = current->edges;
    while (edge) 
    {
        Edge next_edge = edge->next;
        free(edge); // Free the edge.
        edge = next_edge; // Move to the next edge.
    }
}

// Removes the node from the hash table.
static void remove_node_from_hash_table(Graph graph, Node current, Node prev) 
{
    if (prev) {
        prev->next_in_hash = current->next_in_hash;
    } else {
        graph->hash_table[hash(current->id, graph->table_size)] = current->next_in_hash;
    }
}

bool node_remove(Graph graph, const char* id) 
{
    if (!graph || !id) 
    {
        return false; // Invalid input.
    }

    Node current = graph->hash_table[hash(id, graph->table_size)];
    Node prev = NULL;

    while (current) 
    {
        if (strcmp(current->id, id) == 0) 
        {
            // Remove edges pointing to this node.
            remove_edges_to_node(graph, id);

            free_node_edges(current);

            remove_node_from_hash_table(graph, current, prev);

            free(current->id);
            free(current);
            return true; // Node successfully removed.
        }

        prev = current;
        current = current->next_in_hash; // Move to the next node in the list.
    }

    return false; // Node not found.
}

// Creates a new edge and returns it.
static Edge create_edge(Node src_node, Node dest_node, int amount, const char* date) 
{
    Edge new_edge = (Edge)malloc(sizeof(struct edge));
    if (!new_edge) 
    {
        perror("Memory allocation failed");
        return NULL;
    }

    new_edge->amount = amount;
    strncpy(new_edge->date, date, 10);
    new_edge->date[10] = '\0'; // Ensure null-termination.
    new_edge->destination = dest_node;
    new_edge->next = src_node->edges; // Link to the existing edges.
    return new_edge;
}

// Finds an edge between two nodes.
static Edge find_edge(Node src_node, const char *dest_id) 
{
    Edge current = src_node->edges;
    while (current) 
    {
        if (current->destination && strcmp(current->destination->id, dest_id) == 0) 
        {
            return current;
        }
        current = current->next;
    }
    return NULL; // Edge not found.
}

// Removes a specific edge from the list of edges.
static bool remove_edge_from_list(Node src_node, Edge edge_to_remove) 
{
    Edge current = src_node->edges;
    Edge prev = NULL;

    while (current) 
    {
        if (current == edge_to_remove) 
        {
            // Update the pointers to remove the edge.
            if (prev) {
                prev->next = current->next;
            } else {
                src_node->edges = current->next; // Update head of the list.
            }
            free(current); // Free the edge memory.
            return true;   // Edge successfully removed.
        }
        prev = current;
        current = current->next;
    }
    return false; // Edge not found.
}

// Edge insertion.
Edge edge_insert(Graph graph, const char* src_id, const char* dest_id, int amount, const char* date) 
{
    if (!graph || !src_id || !dest_id || !date) 
    {
        return NULL; // Error handling for invalid inputs.
    }

    Node src_node = node_insert(graph, src_id);
    Node dest_node = node_insert(graph, dest_id);
    if (!src_node || !dest_node) 
    {
        return NULL; // Node creation failed.
    }

    Edge new_edge = create_edge(src_node, dest_node, amount, date);
    if (!new_edge) 
    {
        return NULL; // Edge creation failed.
    }

    src_node->edges = new_edge; // Link the new edge to the source node.
    return new_edge; // Return the newly created edge.
}

// Edge removal.
bool edge_remove(Graph graph, const char *src_id, const char *dest_id) {
    if (!graph || !src_id || !dest_id) 
    {
        return false; // Error handling for invalid inputs.
    }

    Node src_node = node_find(graph, src_id);
    if (!src_node) 
    {
        return false; // Source node not found.
    }

    Edge edge_to_remove = find_edge(src_node, dest_id);
    if (!edge_to_remove) 
    {
        return false; // Edge not found.
    }

    return remove_edge_from_list(src_node, edge_to_remove); // Remove the edge and return success status.
}

// Edge modification.
bool edge_modify(Graph graph, const char *src_id, const char *dest_id, int old_amount, int new_amount, const char *old_date, const char *new_date) 
{
    if (!graph || !src_id || !dest_id || !old_date || !new_date) 
    {
        return false; // Error handling for invalid inputs.
    }

    Node src_node = node_find(graph, src_id);
    if (!src_node) 
    {
        return false; // Source node not found.
    }

    Edge edge = src_node->edges;
    while (edge) 
    {
        if (edge->destination && strcmp(edge->destination->id, dest_id) == 0 &&
            edge->amount == old_amount && strcmp(edge->date, old_date) == 0) 
        {
            edge->amount = new_amount; // Modify amount.
            strncpy(edge->date, new_date, 10); // Modify date.
            edge->date[10] = '\0'; // Ensure null-termination.
            return true; // Modification successful.
        }
        edge = edge->next; // Move to the next edge.
    }

    return false; // Edge not found.
}

void graph_destroy(Graph graph)
{
    if (!graph)
    {
        return;
    }

    for (size_t i = 0; i < graph->table_size; i++)
    {
        Node current = graph->hash_table[i];
        while (current)
        {
            Node next_node = current->next_in_hash;
            Edge edge = current->edges;
            while (edge)
            {
                Edge temp = edge;
                edge = edge->next;
                free(temp);
            }

            free(current->id);
            current->id = NULL;
            free(current);
            current = next_node;
        }
    }

    free(graph->hash_table);
    graph->hash_table = NULL;
    free(graph);
    graph = NULL;
}

// hash function djb2 from http://www.cse.yorku.ca/~oz/hash.html
uint hash(const char* id, size_t table_size)
{
    unsigned long hash = 5381;
    int c;
    while((c = *id++))
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % table_size;
}