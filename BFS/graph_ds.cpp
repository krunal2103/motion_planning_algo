#include <iostream>
#include <vector>

class Graph{
private:
    int num_V;
    std::vector<std::vector<int> > list_data;
public:
    Graph(int V):num_V(V) {
        list_data.resize(V);
    }
    void addEdge(int src, int dest);
    void show();
};

void Graph::addEdge(int src, int dest){
    list_data[src].push_back(dest);
    list_data[dest].push_back(src);
}

void Graph::show(){
    for(int i = 0; i < num_V; i++){
        std::cout << i;
        for(auto j : list_data[i]){
            std::cout << " -> " << j;
        }
        std::cout << std::endl;
    }
}

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