#include <stdio.h>
#include <stdlib.h>


int main(){
    int n = 15;
    int res = ~n ^ ~(n+1);
    printf("Result: %d\n", res);
    return 0;

}