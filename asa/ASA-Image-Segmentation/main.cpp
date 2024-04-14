#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <vector>
#include <queue>
/*****************************************************************************
*                                   STRUCTS 
******************************************************************************/
struct edge {
  int s;
  int t;
  int cap;
  int flow;
};

/*****************************************************************************
*                                 PROTOTYPES 
******************************************************************************/
void parse_input(std::vector<std::vector<edge*> > &graph, int m, int n);

/*****************************************************************************
*                               GLOBAL VARIABLES
******************************************************************************/
bool *visited;
int size;
int flow = 0;
std::vector<edge*> *line_aux;

int  
BFS(std::vector<std::vector<edge*> > &graph,std::vector<edge*> &parent, int s, int t) {  
  visited = new bool[size];
  memset(visited, false, sizeof(bool)*size);
  
  std::queue<int> q;
  q.push(s);
  visited[s] = true;
  parent[s] = NULL;
  while(!q.empty()) {
    int u = q.front(); q.pop();
    for(unsigned i = 0; i < graph[u].size(); i++) {
      edge *e = graph[u][i];
      if(visited[e->t] == false && (e->t != s) && (e->cap > e->flow)) {
        q.push(e->t);
        parent[e->t] = e;
        visited[e->t] = true;
      } 
    }
  }
  return (visited[t] == true);
}

int
EdmondsKarp(std::vector<std::vector<edge*> > &graph, std::vector<edge*> &parent, int s, int t) {
  edge *v; 
  while (BFS(graph, parent, s, t)) {
    int dflow = INT_MAX;
    for(v = parent[t]; v != NULL; v = parent[v->s]) {
      dflow = std::min(dflow, (v->cap - v->flow)); 
    }
    for(v = parent[t]; v != NULL; v = parent[v->s]) {
      v->flow += dflow;
    }
    flow += dflow;
  }
  return flow;
} 

int
main() {
  int m, n, i;

  scanf("%d %d", &m, &n); size = (m*n) + 2;

  std::vector<std::vector<edge*> > graph(size);
  std::vector<edge*> parent(size);
  line_aux = new std::vector<edge*>(size);

  parse_input(graph, m, n);

  printf("%d\n\n", EdmondsKarp(graph, parent, 0, size-1));
  
  BFS(graph, parent, 0, size-1);
  for(i = 1; i <= size-2; i++) {
    if(visited[i] == true) {printf("C ");}
    else {printf("P ");}
    if(i % n == 0) {printf("\n");}
  }  
  
  delete[] visited;
  return 0;
}

void
parse_input_fg(std::vector<std::vector<edge*> > &graph, int m, int n) {
  int i;
  for(i = 1; i <= (m*n); i++) {
    int d;
    scanf("%d", &d);
    std::vector<edge*> &v = *line_aux;

    if(d != 0) {
      edge *edge_foward = new edge;
      edge_foward->s = 0;
      edge_foward->t = i;
      edge_foward->cap = d;
      edge_foward->flow = 0;
      
      edge *edge_backward = new edge;
      edge_backward->s = i;
      edge_backward->t = 0;
      edge_backward->cap = d;
      edge_backward->flow = 0;
 
      graph[0].push_back(edge_foward);
      graph[i].push_back(edge_backward);
      v[i] = edge_foward;  
    } else v[i] = NULL;
  }
}

void
parse_input_bg(std::vector<std::vector<edge*> > &graph, int m, int n) {
  int i;
  for(i = 1; i <= (m*n); i++) {
    int d;
    scanf("%d", &d);
    std::vector<edge*> &v = *line_aux;

    if(d != 0) {
      edge *edge_foward = new edge;
      edge_foward->s = i;
      edge_foward->t = (m*n)+1;
      edge_foward->cap = d;
      edge_foward->flow = 0;
      
      edge *edge_backward = new edge;
      edge_backward->s = size-1;
      edge_backward->t = i;
      edge_backward->cap = d;
      edge_backward->flow = 0;
      
      edge *e = v[i];
      if(e != NULL) {
        int dflow = std::min(e->cap, edge_foward->cap);
        e->flow += dflow;
        edge_foward->flow += dflow;
        flow += dflow;
      }
      
      graph[i].push_back(edge_foward);
      graph[size-1].push_back(edge_backward);
    }
  }
}

void
parse_input_horiz_wt(std::vector<std::vector<edge*> > &graph, int m, int n) {
  int i = 1;
  while(i <= m*n) {
    if(i%n != 0) {
      int d;
      scanf("%d", &d);
      
      if(d != 0) {
        edge *edge_foward = new edge;
        edge_foward->s = i;
        edge_foward->t = i+1;
        edge_foward->cap = d;
        edge_foward->flow = 0;

        edge *edge_backward = new edge;
        edge_backward->s = i+1;
        edge_backward->t = i;
        edge_backward->cap = d;
        edge_backward->flow = 0;
        
        graph[i].push_back(edge_foward);
        graph[i+1].push_back(edge_backward);
        i++;
      } else {i++;}
    } else {i++;}
  }
}

void
parse_input_vert_wt(std::vector<std::vector<edge*> > &graph, int m, int n) {
  int i;
  for(i = 1; i <= (m-1)*n; i++) {
    int d;
    scanf("%d", &d);

    if(d != 0) {
      edge *edge_foward = new edge;
      edge_foward->s = i;
      edge_foward->t = i+n;
      edge_foward->cap = d;
      edge_foward->flow = 0;
      
      edge *edge_backward = new edge;
      edge_backward->s = i+n;
      edge_backward->t = i;
      edge_backward->cap = d;
      edge_backward->flow = 0;
      
      graph[i].push_back(edge_foward);
      graph[i+n].push_back(edge_backward);
    }
  }
}

void 
parse_input(std::vector<std::vector<edge*> > &graph, int m, int n) {
  parse_input_fg(graph, m, n);
  parse_input_bg(graph, m, n);
  parse_input_horiz_wt(graph, m, n);
  parse_input_vert_wt(graph, m, n);
}
