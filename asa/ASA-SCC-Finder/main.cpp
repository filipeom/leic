#include <stdio.h>
#include <vector>
#include <stack>
#include <algorithm>
#include <iterator>

int visited, R = 0, L = 0;
int *d, *low, *scc;
bool *stacked;
std::stack<int> stack;
std::vector<std::vector<int>> scc_tree;

int 
insert(std::vector<int> &vec, int v) {
    std::vector<int>::iterator it = std::lower_bound(vec.begin(), vec.end(), v);
   
    if(it != vec.end() && *it == v) 
        return 0;
    
    vec.insert(it, v);
    return 1;
}

void
tarjan_visit(std::vector<std::vector<int>> &graph, unsigned u) {
    unsigned v, w;
    d[u] = low[u] = visited++;
    stack.push(u);
    stacked[u] = true;
    
    for(v = 0; v < graph[u].size(); v++) {
        w = graph[u][v];
        if(d[w] == -1) {
            tarjan_visit(graph, w);
            low[u] = std::min(low[u], low[w]);
        } else if(stacked[w]) {
            low[u] = std::min(low[u], d[w]);
        }
    }
    if(d[u] == low[u]) {
        std::vector<int> component;
        do {
            w = stack.top();
            stack.pop();
            stacked[w] = false;
            insert(component, w);
            scc[w] = R;
        } while(u != w);
        scc_tree.push_back(component);
        R++;
    }
}

void
scc_tarjan(std::vector<std::vector<int>> &graph, unsigned V) {
    unsigned u;
    visited = 1;
    for(u = 0; u < V; u++) {
        d[u] = -1;
    }
    for(u = 0; u < V; u++) {
        if(d[u] == -1) 
            tarjan_visit(graph, u);
    }
}

void
find_conncections(std::vector<std::vector<int>> &graph, 
        std::vector<std::vector<int>> &connections, int V) {
    int i, u;
    unsigned j;

    for(i = 0; i < V; i++)
    for (j = 0;j < graph[i].size(); j++){
        u = graph[i][j];
        if(scc[i] != scc[u]) {
            int root1 = scc_tree[scc[i]][0]; 
            int root2 = scc_tree[scc[u]][0];
            
            L += insert(connections[root1], root2);
        }
    }       
}

int
main() {
    int V, E;
    int u, v;
    int i;

    scanf("%d %d", &V, &E);
    std::vector<std::vector<int>> graph(E);
    
    for(i = 0; i < E; i++) {
        scanf("%d %d", &u, &v); 
        graph[--u].push_back(--v);
    }

    d = new int[V]();
    low = new int[V]();
    scc = new int[V]();
    stacked = new bool[V]();

    scc_tarjan(graph, V);
    
    std::vector<std::vector<int>> connections(V);
    find_conncections(graph, connections, V);

    printf("%d\n", R);
    printf("%d\n", L); 
    for(i = 0; i < V; i++) {
        for(unsigned j = 0; j < connections[i].size(); j++){
            printf("%d %d\n", i+1, connections[i][j]+1);
        }
    }

    delete[] d;
    delete[] low;
    delete[] scc;
    delete[] stacked;
    return 0;
}
