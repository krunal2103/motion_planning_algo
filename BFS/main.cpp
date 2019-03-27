#include "graph.h"

int main(){
    int v = 5;
    Graph graph(v);
    graph.addEdge(0,1);
    graph.addEdge(0,4);
    graph.addEdge(1,2);
    graph.addEdge(1,3);
    graph.addEdge(1,4);
    graph.addEdge(2,3);
    graph.addEdge(3,4);
    graph.addEdge(4,0);
    graph.show();

    graph.BFS(0);
    graph.DFS(0);
    return 0;
}