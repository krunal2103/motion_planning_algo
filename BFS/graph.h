#include <iostream>
#include <vector>

class Graph
{
  private:
    int num_V;
    std::vector<std::vector<int> > list_data;

  public:
    Graph(int V) : num_V(V)
    {
        list_data.resize(V);
    }
    void addEdge(int src, int dest);
    void show();
};