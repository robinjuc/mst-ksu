#include <iostream>
#include <climits>
#define V_PRIM 2000

using namespace std;

class Prim final {
public:

    // Find min key vertex
    static int minKey(int key[], bool mstSet[]) 
    {
        int min = INT_MAX, min_idx;
        for (int v = 0; v < V_PRIM; v++)
            if (!mstSet[v] && key[v] < min) min = key[v], min_idx = v;
        return min_idx;
    }

    // Print Minimum Spanning Tree
    static void printMST(int parent[], int graph[V_PRIM][V_PRIM]) 
    {
        cout << "Edge \tWeight\n";
        for (int i = 1; i < V_PRIM; i++)
            cout << parent[i] << " - " << i << " \t" << graph[i][parent[i]] << "\n";
    }

    // Prim's algorithm using adjacency matrix
    static void primMSTArrayAdj(int graph[V_PRIM][V_PRIM]) 
    {
        int parent[V_PRIM], key[V_PRIM];
        bool mstSet[V_PRIM];
        for (int i = 0; i < V_PRIM; i++) 
            key[i] = INT_MAX, mstSet[i] = false;

        key[0] = 0; parent[0] = -1;

        for (int count = 0; count < V_PRIM - 1; count++) {
            int u = minKey(key, mstSet);
            mstSet[u] = true;
            for (int v = 0; v < V_PRIM; v++)
                if (graph[u][v] && !mstSet[v] && graph[u][v] < key[v])
                    parent[v] = u, key[v] = graph[u][v];
        }
        //printMST(parent, graph);
    }


    // Prevent instantiation (C++11 and later)
    Prim() = delete; 

private:
    // Private static data member (shared state)
    static int m_numVertices;

};