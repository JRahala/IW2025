#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct Node{
    struct Node *below;
    struct Node *next;
    int key;
    int rank;    
} Node;


void print_node(const Node *node){
    printf("(%d) Node value: [%d]", node->rank, node->key);
}

void print_skiplist(const Node *skiplist){
    printf("Printing Skiplist\n");
    Node *curr = (Node *)skiplist;
    while (curr){
        Node *curr_copy = curr;
        while (curr_copy){
            print_node(curr_copy);
            printf(" -> ");
            curr_copy = curr_copy->next;
        }
        printf("NULL\n");
        curr = curr->below;
    }
}

Node *create_node(int key, int rank){
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->below = NULL;
    new_node->next = NULL;
    new_node->key = key;
    new_node->rank = rank;
    return new_node;
}

int get_max_layer(int elements){
    return (int)log2(elements)+1;
}

int is_power_of_two(int x){
    if (x==0) return 1;
    if (x < 0) return 0;
    return ((x & (~(x-1))) == x);
}

Node *empty(){
    return create_node(INT_MIN, 0);
}

// Check this
Node *add_layer(const Node *skiplist){
    Node *old_skiplist = (Node *)skiplist;
    Node *new_skiplist = create_node(old_skiplist->key, old_skiplist->rank+1);
    new_skiplist->below = old_skiplist;
    Node *prev = new_skiplist;
    old_skiplist = old_skiplist->next;

    while (old_skiplist){
        int promote = rand()%2;
        if (promote){
            Node *copy = create_node(old_skiplist->key, old_skiplist->rank+1);
            prev->next = copy;
            copy->below = old_skiplist;
            prev = copy;
        }
        old_skiplist = old_skiplist->next;
    }
    
    return new_skiplist;
}

Node *search(const Node *skiplist, int key){
    Node *curr = (Node *)skiplist;
    while (curr){
        if (curr->next && curr->next->key == key) return curr->next;
        if (curr->next && curr->next->key < key) curr = curr->next;
        else curr = curr->below;
    }
    return NULL;
}

Node *insert(const Node *skiplist, int elements, int key){
    Node *new_skiplist = (Node *)skiplist;
    // can replace with a nice bit trick of (x ^ x--) == 0 (for powers of 2)
    //if (is_power_of_two(elements+1)){???
    if (get_max_layer(elements+1) != get_max_layer(elements)){
        new_skiplist = add_layer(skiplist);
    }

    Node **history = (Node **)calloc(new_skiplist->rank+1, sizeof(Node *));
    Node *curr = (Node *)new_skiplist;
    while (curr){
        if (curr->next && curr->next->key == key){
            curr = curr->next;
            break;
        }
        if (curr->next && curr->next->key < key){
            curr = curr->next;
        }
        else{
            history[curr->rank] = curr;
            curr = curr->below;
        }
    }

    while (curr){
        history[curr->rank] = curr;
        curr = curr->below;
    }

    Node *prev = NULL;
    for (int layer=0; layer<=new_skiplist->rank; layer++){
        int promote = (rand() % 2) || (layer == 0);
        if (!promote) break;
        
        Node *new_node = create_node(key, layer);
        Node *old_next = history[layer]->next;
        history[layer]->next = new_node;
        new_node->next = old_next;
        new_node->below = prev;
        prev = new_node;
    }

    free(history);
    return new_skiplist;
}

Node *remove_layer(const Node *skiplist){
    Node *new_skiplist = skiplist->below;
    Node *old_skiplist = (Node *)skiplist;
    while (old_skiplist){
        Node *prev = old_skiplist;
        old_skiplist = old_skiplist->next;
        free(prev);
    }
    return new_skiplist;
}

Node *delete(const Node *skiplist, int elements, int key){
    Node *curr = (Node *)skiplist;
    while (curr){
        if (curr->next && curr->next->key == key){
            Node *old_node = curr->next;
            curr->next = curr->next->next;
            free(old_node);
            curr = curr->below;
            continue;
        }
        if (curr->next && curr->next->key < key) curr = curr->next;
        else curr = curr->below;
    }

    Node *new_skiplist = (Node *)skiplist;
    if (get_max_layer(elements-1) != get_max_layer(elements)){
        new_skiplist = remove_layer(skiplist);
    }    

    return new_skiplist;
}

void free_skiplist(Node *skiplist){
    Node *curr = (Node *)skiplist;
    while (curr){    
        Node *curr_copy = curr->next;
        while (curr_copy){
            Node *tmp = curr_copy->next;
            free(curr_copy);
            curr_copy = tmp;
        }
        Node *tmp = curr->below;
        free(curr);
        curr = tmp;
    }
}

int main(){

    srand(time(NULL));
    Node *skiplist = empty();
    for (int elements = 0; elements < 5; elements++){
        skiplist = insert(skiplist, elements, elements+1);
    }
    print_skiplist(skiplist);

    skiplist = delete(skiplist, 5, 2);
    skiplist = delete(skiplist, 4, 1);
    print_skiplist(skiplist);
    free_skiplist(skiplist);
    return 0;
}