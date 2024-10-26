#include "graph.h"

Graph graph_create(size_t initial_size) 
{
    // Allocate memory for the struct
    Graph graph = (Graph)malloc(sizeof(struct graph));
    if (!graph) 
    {
        perror("Memory allocation failed.");
        return NULL;
    }

    graph->table_size = initial_size;

    graph->hash_table = (Node*)calloc(initial_size, sizeof(Node));
    if (!graph->hash_table)
    {
        perror("Memory allocation failed.");
        free(graph);
        graph = NULL;
        return NULL;
    }

    return graph;
}

Node node_insert(Graph graph, const char* id) 
{
    // Error handling 
    if (!graph || !id)
    {
        return NULL;
    }

    uint index = hash(id, graph->table_size);

    Node current = graph->hash_table[index];

    while (current)
    {
        if(strcmp(current->id, id) == 0) // Node already exists
        {
            return current;
        }

        current = current->next_in_hash;
    }

    // Creating new node since it does not exist
    Node new_node = (Node)malloc(sizeof(struct node));
    if (!new_node)
    {
        perror("Memory allocation failed.");
        return NULL;
    }

    new_node->id = strdup(id);
    new_node->edges = NULL;
    new_node->next_in_hash = graph->hash_table[index];
    graph->hash_table[index] = new_node;

    return new_node;
}

Node node_find(Graph graph, const char* id) 
{
    if (!graph || !id)
    {
        return NULL;
    }

    uint index = hash(id, graph->table_size);

    Node current = graph->hash_table[index];
    while (current)
    {
        if (strcmp(current->id, id) == 0) // Node found
        {
            return current;
        }
        current = current->next_in_hash;
    }

    // Node not found
    return NULL;
}

void node_remove(Graph graph, const char *id)
{
    if (!graph || !id)
    {
        return;
    }

    uint index = hash(id,graph->table_size);

    Node current = graph->hash_table[index];
    Node prev = NULL;

    while (current)
    {
        // Free Node

        if (strcmp(current->id, id) == 0)
        {
            if (prev)
            {
                prev->next_in_hash = current->next_in_hash;
            } else {
                graph->hash_table[index] = current->next_in_hash;
            }

            // Free Edges

            Edge edge = current->edges;
            while (edge) {
                Edge temp = edge;
                edge = edge->next;
                free(temp);
                temp = NULL;
            }

            free(current->id);
            current->id = NULL;
            free(current);
            current = NULL;
            return;
        }

        prev = current;
        current = current->next_in_hash;
    }
}

Edge edge_insert(Graph graph, const char* src_id, const char* dest_id, int amount, const char* date) 
{
    if (!graph || !src_id || !dest_id || !date)
    {
        return NULL;
    }

    Node src_node = node_insert(graph, src_id);
    Node dest_node = node_insert(graph, dest_id);
    if (!src_node || !dest_node)
    {
        return NULL;
    }

    Edge new_edge = (Edge)malloc(sizeof(struct edge));
    if (!new_edge)
    {
        perror("Memory allocation failed");
        return NULL;
    }

    new_edge->amount = amount;
    strncpy(new_edge->date, date, 10);
    new_edge->date[10] = '\0';
    new_edge->destination = dest_node;
    new_edge->next = src_node->edges;
    src_node->edges = new_edge;

    return new_edge;
}

void edge_remove(Graph graph, const char *src_id, const char *dest_id)
{
    if (!graph || !src_id || !dest_id)
    {
        return;
    }

    Node src_node = node_find(graph, src_id);
    if (!src_node) {
        printf("Node %s not found\n", src_id);
        return;
    }

    Edge current = src_node->edges;
    Edge prev = NULL;
    while (current) {
        if (current->destination && strcmp(current->destination->id, dest_id) == 0)
        {
            if (prev)
            {
                prev->next = current->next;
            } else {
                src_node->edges = current->next;
            }

            free(current);
            current = NULL;
            return;
        }

        prev = current;
        current = current->next;
    }
}

bool edge_modify(Graph graph, const char *src_id, const char *dest_id, int old_amount, int new_amount, const char *old_date, const char *new_date)
{
    if (!graph || !src_id || !dest_id || !old_date || !new_date)
    {
        return false;
    }

    Node src_node = node_find(graph, src_id);
    if (!src_node)
    {
        printf("Source node %s not found\n", src_id);
        return false;
    }

    Edge edge = src_node->edges;
    while (edge)
    {
        if (edge->destination && strcmp(edge->destination->id, dest_id) == 0 &&
        edge->amount == old_amount && strcmp(edge->date, old_date) == 0)
        {
            edge->amount = new_amount;
            strncpy(edge->date, new_date, 10);
            edge->date[10] = '\0';
            return true;
        }
        edge = edge->next;
    }

    return false;
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