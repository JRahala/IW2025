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

    /*
    printf("Printing search history\n");
    for (int layer=0; layer<=new_skiplist->rank; layer++){
        printf("Layer %d: ", layer);
        print_node(history[layer]);
        printf(" -> ");
        if (history[layer]->next) print_node(history[layer]->next);
        else printf("NULL");
        printf("\n");
    }
    */

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
    
    return new_skiplist;
}


int main(){

    srand(time(NULL));
    Node *skiplist = empty();
    for (int elements = 0; elements < 5; elements++){
        skiplist = insert(skiplist, elements, elements+1);
    }
    print_skiplist(skiplist);

    /*
    SHOULD MAKE SOMETHING WHERE I CAN READ COMMANDS FROM A TEXT FILE RATHER THAN RECOMPILE
    Node *skiplist = empty();
    Node *a = create_node(10, 0);
    Node *b = create_node(20, 0);
    Node *c = create_node(30, 0);
    skiplist->next = a;
    a->next = b;
    b->next = c;

    print_skiplist(skiplist);

    skiplist = add_layer(skiplist);
    skiplist = add_layer(skiplist);
    print_skiplist(skiplist);

    Node *search_result = search(skiplist, 35);
    printf("Search result\n");
    if (!search_result) printf("No result\n");
    else print_node(search_result);

    skiplist = insert(skiplist, 3, 25);
    print_skiplist(skiplist);
    */

    return 0;
}