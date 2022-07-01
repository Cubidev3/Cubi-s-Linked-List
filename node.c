#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node
{
    int value;
    struct node *next;
}
node;

node *new_node(int value);
void insert_at_start(node **base, node *n);
void insert_at_end(node **base, node *n);
node *get_node(node *base, int idx);
void insert_at(node **base, node *n, int idx);
void free_nodes(node *base);
void remove_at(node **base, int idx);
void remove_at_end(node **base);
void remove_at_start(node **base);
node *pop_at(node **base, int idx);
node *pop_at_start(node **base);
node *pop_at_end(node **base);
void insert_all_at(node **base, node *root, int idx);
void insert_all_at_start(node **base, node *root);
void insert_all_at_end(node **base, node *root);
node *slice_at(node **base, int idx);
node *slice_section(node **base, int start, int end);
void print_list(node *base);
bool has_value(node *base,int value);
bool has_node(node *base, node *find);
void invert(node **base, node *start);

int main(void)
{
    node *root = new_node(9);
    node *root2 = new_node(3);

    insert_at_end(&root, new_node(10));
    insert_at_end(&root, new_node(12));
    insert_at_end(&root, new_node(15));

    insert_at_end(&root2, new_node(4));
    insert_at_end(&root2, new_node(6));
    insert_at_end(&root2, new_node(8));

    print_list(root);
    print_list(root2);

    insert_all_at_start(&root, root2);
    invert(&root, root);

    print_list(root);

    root2 = slice_at(&root, 4);

    print_list(root);
    print_list(root2);

    free_nodes(root);
    free_nodes(root2);
}

node *new_node(int value)
{
    node *n = malloc(sizeof(node));
    if (n == NULL)
    {
        printf("There is no space available");
        return NULL;
    }

    n->value = value;
    n->next = NULL;
}

void insert_at_start(node **base, node *n) // FAST O(1)
{
    if (base == NULL || *base == NULL || n == NULL)
    {
        return;
    }

    n->next = *base;
    *base = n; 
}

void insert_at_end(node **base, node *n) // SLOW O(n)
{
    if (base == NULL || *base == NULL || n == NULL)
    {
        return;
    }

    if ((*base)->next == NULL)
    {
        (*base)->next = n;
        return;
    }

    insert_at_end(&((*base)->next), n);
}

void insert_at(node **base, node *n, int idx)
{
    if (base == NULL || *base == NULL || n == NULL || idx < 0)
    {
        return;
    }

    if (idx == 0)
    {
        insert_at_start(base, n);
        return;
    }

    node *parent = get_node(*base, idx - 1);
    n->next = parent->next;
    parent->next = n;
}

node *get_node(node *base, int idx)
{
    if (idx < 0 || base == NULL)
    {
        return NULL;
    }

    if (idx == 0)
    {
        return base;
    }

    if (base->next == NULL)
    {
        return NULL;
    }

    return get_node(base->next, idx - 1);
}

void free_nodes(node *base)
{
    if (base->next != NULL)
    {
        free_nodes(base->next);
    }

    free(base);
}

void remove_at(node **base, int idx)
{
    if (base == NULL || *base == NULL || idx < 0)
    {
        return;
    }

    if (idx == 0)
    {
        remove_at_start(base);
        return;
    }

    node *parent = get_node(*base, idx - 1);

    if (parent->next == NULL) // Index out of bounds
    {
        return;
    }

    node *tmp = parent->next->next;
    free(parent->next);
    parent->next = tmp;
}

void remove_at_end(node **base)
{
    if (base == NULL || *base == NULL)
    {
        return;
    }

    if ((*base)->next->next == NULL) // Found End node's Parent
    {
        free((*base)->next);
        (*base)->next = NULL;
        return;
    }
    
    return remove_at_end(&((*base)->next));
}

void remove_at_start(node **base)
{
    if (base == NULL || *base == NULL)
    {
        return;
    }

    node *tmp = (*base)->next;
    free(*base);
    *base = tmp;
}

node *pop_at(node **base, int idx)
{
    if (base == NULL || *base == NULL)
    {
        return NULL;
    }

    node *pop = slice_section(base, idx, idx);
    return pop;
}

node *pop_at_start(node **base)
{
    return pop_at(base, 0);
}

node *pop_at_end(node **base)
{
    if (base == NULL || *base == NULL)
    {
        return NULL;
    }

    if ((*base)->next == NULL)
    {
        return *base;
    }

    if ((*base)->next != NULL && (*base)->next->next == NULL) // Is end node's parent
    {
        return pop_at(base, 1);
    }

    return pop_at_end(&((*base)->next));
}

void insert_all_at(node **base, node *root, int idx)
{
    if (base == NULL || *base == NULL || root == NULL)
    {
        return;
    }

    if (root->next != NULL)
    {
        insert_all_at(base, root->next, idx);
    }

    insert_at(base, root, idx);
}

void insert_all_at_start(node **base, node *root)
{
    if (base == NULL || *base == NULL || root == NULL)
    {
        return;
    }

    node *root_end = root;
    while (root_end->next != NULL)
    {
        root_end = root_end -> next;
    }

    root_end->next = (*base);
    *base = root;
}

void insert_all_at_end(node **base, node *root)
{
    if (base == NULL || *base == NULL || root == NULL)
    {
        return;
    }

    node *base_end = *base;
    while (base_end->next != NULL)
    {
        base_end = base_end->next;
    }

    base_end->next = root;
}

node *slice_at(node **base, int idx)
{
    if (base == NULL || *base == NULL || idx < 0)
    {
        return NULL;
    }

    if (idx == 0)
    {
        return *base;
    }

    node *start_parent = get_node(*base, idx - 1);
    if (start_parent == NULL || start_parent->next == NULL) // Is either out of bounds or 
    {
        return NULL;
    }

    node *start = start_parent->next;
    start_parent->next = NULL;
    return start;
}

node *slice_section(node **base, int start, int end)
{
    if (base == NULL || *base == NULL || start < 0 || end < 0 || end < start)
    {
        return NULL;
    }

    if (start == 0)
    {
        node *tmp = slice_at(base, end + 1);
        node *slice = *base;
        *base = tmp;
        return slice;
    }

    node *start_node = slice_at(base, start);
    node *connect = slice_at(&start_node, end - start + 1);
    insert_all_at_end(base, connect);

    return start_node;
}

void print_list(node *base)
{
    if (base == NULL)
    {
        return;
    }

    printf("[");

    for (node *tmp = base; tmp != NULL; tmp = tmp->next)
    {
        printf("%i,", tmp->value);
    }

    printf("]\n");
}

bool has_value(node *base,int value)
{
    if (base->value == value)
    {
        return true;
    }

    if (base->next == NULL)
    {
        return false;
    }

    return has_value(base->next, value);
}

bool has_node(node *base, node *find)
{
    if (base->next == find->next && base->value == find->value)
    {
        return true;
    }

    if (base->next == NULL)
    {
        return false;
    }

    return has_node(base->next, find);
}

void invert(node **base, node *start)
{
    if (base == NULL || *base == NULL || start == NULL)
    {
        return;
    }

    if (start->next != NULL)
    {
        invert(base, start->next);
    }
    else
    {
        *base = start;
        return;
    }

    start->next->next = start;
    start->next = NULL;
}