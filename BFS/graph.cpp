#include "graph.h"

void Graph::addEdge(int src, int dest)
{
    list_data[src].push_back(dest);
    // list_data[dest].push_back(src);
}

void Graph::show()
{
    for (int i = 0; i < num_V; i++)
    {
        std::cout << i << " ->";
        for (auto j : list_data[i])
        {
            std::cout << " " << j;
        }
        std::cout << std::endl;
    }
}

void Graph::BFS(int s)
{
    std::list<int> queue;
    bool *visited = new bool[num_V];
    for (int i = 0; i < num_V; i++){
        visited[i] = false;
    }

    std::cout << "BFS starting from vertex " << s << " is as follows:\n";

    visited[s] = true;
    queue.push_back(s);

    std::vector<int>::iterator i;
    int temp;

    while(!queue.empty()){
        temp = queue.front();
        std::cout << temp << " ";

        queue.pop_front();

        for(i = list_data[temp].begin(); i != list_data[temp].end(); ++i){
            if(!visited[*i]){
                visited[*i] = true;
                queue.push_back(*i);
            }
        }
    }
    std::cout << std::endl;
}

void Graph::DFS(int s)
{
    std::vector<int> stack;
    bool *visited = new bool[num_V];
    for (int i = 0; i < num_V; i++){
        visited[i] = false;
    }

    std::cout << "DFS starting from vertex " << s << " is as follows:\n";
    
    DFS_impl(s, visited);
    std::cout << std::endl;
}

void Graph::DFS_impl(int s, bool visited[])
{
    visited[s] = true;
    std::cout << s << " ";

    std::vector<int>::iterator i;
    for(i = list_data[s].begin(); i != list_data[s].end(); ++i){
        if(!visited[*i]){
            DFS_impl(*i, visited);
        }
    }
}