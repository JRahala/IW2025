#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
    struct Node *below;
    struct Node *next;
    int key;
    int rank;    
} Node;

Node *create_node(int key, int rank){
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->key = key;
    new_node->rank = rank;
    return new_node;
}

Node *empty(){
    return create_node(INT_MIN, 0);
}

Node *add_layer(){
    
    return NULL;
}

void print_node(const Node *node){
    printf("[%d] Node value: [%d]", node->rank, node->key);
}

void print_skiplist(const Node *skiplist){
    Node *curr = (Node *)skiplist;
    while (curr){
        Node *curr_copy = curr;
        while (curr_copy){
            print_node(curr_copy);
            printf(" -> ");
            curr_copy = curr_copy->next;
        }
        printf("NULL\n");  // Indicate the end of the current layer
        curr = curr->below;
    }
}

int main(){
    Node *skiplist = empty();
    print_skiplist(skiplist);
    add_layer();
    return 0;
}