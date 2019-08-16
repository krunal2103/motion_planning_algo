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
    /**
     * @brief Construct a new Graph object
     * 
     * @param V Number of vertices
     */
    explicit Graph(int V) : num_V(V)
    {
        list_data.resize(V);
    }

    /**
     * @brief Add an edge to the Graph
     * 
     * @param src Starting vertex of the edge
     * @param dest Ending vertex of the edge
     */
    void addEdge(int src, int dest);

    /**
     * @brief Displays the whole Graph
     * 
     */
    void show();

    /**
     * @brief Implements Breadth-First-Search on the Graph
     * 
     * @param s Starting vertex for the search
     */
    void BFS(int s);

    /**
     * @brief Implements Depth-First-Search on the Graph
     * 
     * @param s Starting vertex for the search
     */
    void DFS(int s);
};

#endif // BFS_GRAPH_H_