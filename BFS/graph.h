#ifndef BFS_GRAPH_H_
#define BFS_GRAPH_H_

#include <iostream>
#include <vector>
#include <list>

class Graph
{
  private:
    int num_V;
    std::vector<std::vector<int> > list_data;

    void DFS_impl(int s, bool visited[]);

  public:
    Graph(int V) : num_V(V)
    {
        list_data.resize(V);
    }
    void addEdge(int src, int dest);
    void show();

    void BFS(int s);
    void DFS(int s);
};

#endif // BFS_GRAPH_H_