#include <iostream>
#include <vector>

// hardcoded graph
// only construct 1 at a time
struct Graph {
    const static int size = 5;

    int g[size][size] = {
        {0, 7, 2, 0, 0}, // A/0
        {7, 0, 5, 7, 2}, // B/1
        {2, 5, 0, 2, 0}, // C/2
        {0, 7, 2, 0, 1}, // D/3
        {0, 2, 0, 1, 0}  // C/4
    };
};

// for debugging
template <typename X>
void printV(std::vector<X> vec) {
    for (X num : vec) std::cout << num << ", ";
    std::cout << "\n";
}

// simply checks if a value is already in the vector
bool in(int val, std::vector<int> vec) {
    for (unsigned int i = 0; i < vec.size(); ++i) {
        if (vec[i] == val)
            return true;
    }
    return false;
}

// returns a vector for connected vertices
// index is node # and value is T/F
// vertice is not adjacent to itself
std::vector<bool> getNeighbors(int vertex, const Graph& graph, const std::vector<int>& visited) {
    std::vector<bool> temp(graph.size);
    for (int i = 0; i < graph.size; ++i) {
        if (in(i,visited))
            temp[i] = false;
        else
            temp[i] = graph.g[vertex][i];
    }
    return temp;
}

// returns the vertice # from PQ with the smallest current path
int minPath(std::vector<int> PQ, std::vector<int> path) {
   int minIdx = 0;
    int minimum = path[PQ[0]];
    for (unsigned int i = 0; i < PQ.size(); ++i) {
        if (path[PQ[i]] < minimum) {
            minimum = path[PQ[i]];
            minIdx = i;
        }
    }
    return PQ[minIdx];
}

// returns the index # of the smallest vertice path
// primarily for removing the lowest priority vertex in updatePQ()
int minIndex(std::vector<int> PQ, std::vector<int> path) {
    int minIdx = 0;
    int minimum = path[PQ[0]];
    for (unsigned int i = 0; i < PQ.size(); ++i) {
        if (path[PQ[i]] < minimum) {
            minimum = path[PQ[i]];
            minIdx = i;
        }
    }
    return minIdx;
}

// updates the priority queue
// adds the new lowest priority adjacent vertices not in visited vector
// if one of the adjacency values is already in the priority queue:
//     the path through the lowest priority value to be removed is calculated
//     and if lower it updates the value in path, otherwise nothing
// then removes lowest priority vertex
void updatePQ(std::vector<int>& PQ, std::vector<bool>& adjacent, std::vector<int>& visited, Graph graph, std::vector<int>& path) {
    int minIdx = minIndex(PQ,path);
    visited.push_back(PQ[minIdx]);
    adjacent = getNeighbors(PQ[minIdx], graph, visited);
    // std::cout << "Removing: " << PQ[minIdx] << "\n";
    for (unsigned int i = 0; i < adjacent.size(); ++i) {
        if (adjacent[i] == true) {
            if (in(i, PQ)) {
                // std::cout << path[i] << " already in PQ\n";
                int newPath = path[PQ[minIdx]] + graph.g[PQ[minIdx]][i];
                if (newPath < path[i]) {
                    path[adjacent[i]] = newPath;
                }
            } else {
                PQ.push_back(i);
            }
        }
    }
    PQ.erase(PQ.begin() + minIdx);
}

void djikstras(Graph graph, int vert) {
    // initialization is irrelivent since passing to func before use
    std::vector<bool> adjacent(graph.size);
    std::vector<int> PQ = {vert};

    std::vector<int> path(graph.size, INT32_MAX);
    path[vert] = 0; // setting path to initial node from itself to 0
    std::vector<int> previous(graph.size, -1);
    previous[vert] = vert;
    std::vector<int> visited;

    while (!PQ.empty()) {
        int current = minPath(PQ, path);
        adjacent = getNeighbors(current, graph, visited); // adjacent is updated
        // updates cost for the current node's neighbors
        for (unsigned int i = 0; i < adjacent.size(); ++i) {
            if (adjacent[i] != false) {
                // cost of path from previous node to current plus path cost to the previous node
                int newPath = graph.g[current][i] + path[current];
                if (newPath < path[i]) {
                    previous[i] = current;
                    path[i] = newPath;
                }
            }
        }
        updatePQ(PQ, adjacent, visited, graph, path);
        // printV(PQ);
    }
    // prints out the results
    std::cout << "Shortest Distance from vertex " << vert << " to:\n";   
    for (int i = 0; i < graph.size; ++i) {
        std::cout << i << " is " << path[i] << "\n";
    }
    

}


int main() {

    Graph graph;

    // doesn't return anything useful
    // just prints out the results
    djikstras(graph, 0);

    return 0;
}