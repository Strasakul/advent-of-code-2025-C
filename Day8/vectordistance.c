#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct Vertex {
    struct Vertex *parent;
    long long x, y, z;
    long long size;
} Vertex;

typedef struct {
    double weight;
    Vertex *A, *B;
} Edge;

double distance(Vertex *a, Vertex *b){
    double dx = (double)a->x - (double)b->x;
    double dy = (double)a->y - (double)b->y;
    double dz = (double)a->z - (double)b->z;

    return sqrt(dx*dx + dy*dy + dz*dz);
}

int edgecmp(const void *a, const void *b) {
    const Edge *ea = (const Edge *)a;
    const Edge *eb = (const Edge *)b;

    if (isnan(ea->weight) || isnan(eb->weight)) {
        fprintf(stderr, "NaN detected in edge weight!\n");
        exit(1);
    }

    if (ea->weight < eb->weight) return -1;         
    else if (ea->weight > eb->weight) return 1;
    else return 0;
}

int cmp_desc_ll(const void *a, const void *b) {
    long long va = *(const long long *)a;
    long long vb = *(const long long *)b;

    if (va > vb) return -1;   // descending
    else if (va < vb) return 1;
    else return 0;
}


// ----------- Disjoint Set Union -----------

void make_set(struct Vertex *v){
    v->parent = v;
    v->size = 1;
}

Vertex* find_set(Vertex *v){
    if(v != v->parent){
        v->parent = find_set(v->parent);
    }
    return v->parent;
}

void union_sets(Vertex *a, Vertex *b){
    a = find_set(a);
    b = find_set(b);
    if(a != b){
        if(a->size < b->size){
            Vertex *tmp = a;
            a = b;
            b = tmp;
        }
        b->parent = a;
        a->size += b->size;
    }
}

// ------------------------------------------

int main(int argc, char* argv[]){
     FILE* fp;

    if(argc < 3){
        perror("Pls add txt file and num of pairs");
        exit(1);
    }

    // can't bother to do inout checking
    int num_pairs = atoi(argv[2]);

    fp = fopen(argv[1], "r");
    
    if(fp == NULL){
        perror("Error opening file");
        exit(1);
    }


    // Get Vertices
    int num_vertices = 0;
    Vertex vertices[1000];

    char buff[255];

    while(fgets(buff, sizeof buff, fp)){
        // parse x, y, z
        long long x, y, z;
        sscanf(buff, "%lld,%lld,%lld", &x, &y, &z);

        vertices[num_vertices].x = x;
        vertices[num_vertices].y = y;
        vertices[num_vertices].z = z;

        make_set(&vertices[num_vertices]);

        num_vertices++;
    }

    fclose(fp);

    // Compute edges
    int edge_count = num_vertices * (num_vertices - 1) / 2;
    Edge *edges = malloc(edge_count * sizeof(Edge));

    // not very optimal but enough for like 1000 Vertices
    int k = 0;
    for (int i = 0; i < num_vertices; i++) {
        for (int j = i + 1; j < num_vertices; j++) {
            edges[k].A = &vertices[i];
            edges[k].B = &vertices[j];
            edges[k].weight = distance(&vertices[i], &vertices[j]);
            k++;
        }
    }

    // sort lowest to highest
    qsort(edges, edge_count, sizeof(Edge), edgecmp);

    if (num_pairs > edge_count) {
        num_pairs = edge_count;         // clamp to valid range
    }

    // make DSU
    for(int i = 0; i < num_pairs; ++i){
        union_sets(edges[i].A, edges[i].B);
    }

    long long *sizes = malloc(num_vertices * sizeof(long long));
    int size_count = 0;

    for(int i = 0; i < num_vertices; ++i){
        if(vertices[i].parent == &vertices[i]){  // root
            sizes[size_count++] = vertices[i].size;
        }
    }

    qsort(sizes, size_count, sizeof(long long), cmp_desc_ll);

    long long res = sizes[0] * sizes[1] * sizes[2];

    for(int i = 0; i < size_count; i++){
        printf("%lld ", sizes[i]);
    }
    printf("\n");
    printf("Res: %lld\n", res);


    free(edges);
    return 0;
}