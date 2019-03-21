#include "graph.h"

void Graph::addEdge(int src, int dest)
{
    list_data[src].push_back(dest);
    list_data[dest].push_back(src);
}

void Graph::show()
{
    for (int i = 0; i < num_V; i++)
    {
        std::cout << i;
        for (auto j : list_data[i])
        {
            std::cout << " -> " << j;
        }
        std::cout << std::endl;
    }
}