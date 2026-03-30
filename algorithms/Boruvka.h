#include <iostream>
#include <vector>
#include <climits>
#include <omp.h>

#define V_BORUVKA 2000 // Number of vertices
using namespace std;

class Boruvka final {
public:

static void boruvkaMSTAdj(int graph[V_BORUVKA][V_BORUVKA], int numThreads = 2) 
{
    omp_set_num_threads(numThreads);

    vector<int> parent(V_BORUVKA);
    vector<int> cheapest(V_BORUVKA);

    // Initialize components: each vertex starts as its own component
    for (int i = 0; i < V_BORUVKA; i++) 
        parent[i] = i;

    int numComponents = V_BORUVKA;
    int mstWeight = 0;

    while (numComponents > 1) {
        // Reset cheapest edge for each component
        for (int i = 0; i < V_BORUVKA; i++) 
            cheapest[i] = -1;

        // Parallel Step: Find the cheapest edge incident to each component
        #pragma omp parallel for
        for (int u = 0; u < V_BORUVKA; u++) {
            for (int v = 0; v < V_BORUVKA; v++) {
                // If there's an edge and it connects different components
                if (graph[u][v] != 0 && parent[u] != parent[v]) {
                    int setU = parent[u];
                    int weight = graph[u][v];

                    // Use critical section to safely update shared cheapest array
                    #pragma omp critical
                    {
                        if (cheapest[setU] == -1 || graph[setU][cheapest[setU]] > weight) {
                            cheapest[setU] = v;
                        }
                    }
                }
            }
        }

        // Merge components based on found cheapest edges
        bool changed = false;
        for (int i = 0; i < V_BORUVKA; i++) {
            if (cheapest[i] != -1) {
                int u = i;
                int v = cheapest[i];
                int setU = parent[u];
                int setV = parent[v];

                if (setU != setV) {
                    mstWeight += graph[u][v];
                    //printf("Edge %d-%d (weight %d) added to MST\n", u, v, graph[u][v]);

                    // Merge setV into setU
                    int oldParent = parent[v];
                    for (int j = 0; j < V_BORUVKA; j++) {
                        if (parent[j] == oldParent) parent[j] = setU;
                    }
                    numComponents--;
                    changed = true;
                }
            }
        }
        if (!changed) break;
    }
   // cout << "Total MST Weight: " << mstWeight << endl;
}

    static int operationsCount;

    // Prevent instantiation (C++11 and later)
    Boruvka() = delete; 

private:
    // Private static data member (shared state)
    static int internalState;
};