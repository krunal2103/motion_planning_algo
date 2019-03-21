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
    graph.show();
    return 0;
}