#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct DSU{
    struct DSU * parent;
    int sum;
    int rang;
} DSU;

DSU* make_set(int weight){
    DSU * tmp = (DSU *) malloc(sizeof (DSU));
    tmp->sum = weight;
    tmp->parent = tmp;
    tmp->rang = 1;
    return tmp;
}

DSU * find_set(DSU * node){
    if (node->parent == node){
        return node;
    }
    else{
        node->parent = find_set(node->parent);
        return node->parent;
    }
}


DSU * union_set(DSU * a, DSU * b, int weight){
    a = find_set(a);
    b = find_set(b);
    if (a == b){
        a->sum+=weight;
        return a;
    }
    if (a->rang < b ->rang){
        DSU * tmp = a;
        a = b;
        b = tmp;
    }
    b->parent = a;
    if (a->rang == b->rang){
        a->rang++;
    }
    a->sum += b->sum + weight;
    return a;
}

int main(void){
    int n = 0, m = 0;
    scanf("%d %d", &n, &m);
    DSU ** dsu = (DSU**) malloc(n * sizeof (DSU*));
    for (int i = 0; i < n; i++){
        dsu[i] = make_set((0));
    }

    for (int i = 0; i < m; i++){
        int a,b,c, command;
        scanf("%d", &command);
        if (command == 1) {
            scanf("%d %d %d", &a, &b, &c);
            a--;
            b--;
            union_set(dsu[a],dsu[b], c);
        }
        else{
            int starting_vertex;
            scanf("%d", &starting_vertex);
            starting_vertex--;
            printf("%d\n", find_set((dsu[starting_vertex]))->sum);
        }
    }
    for (int i = 0; i < n; i++){
        free(dsu[i]);
    }
    free(dsu);

}