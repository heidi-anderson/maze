#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

class Graph {
private:
    //adjency list
    vector<vector<int>> adjList;

public:
    // constructor
    Graph(int n) : adjList(n) {}

    void addEdge(int u, int v) // u "to vertices", v "from vertices"
    {
        adjList[u - 1].push_back(v - 1);
    }

    vector<int> findShortestPath(int start, int end)
    {
        // nodes are initially "unreachable" so i set distance to a big number
        vector<int> distance(adjList.size(), 10000);
        vector<int> prev(adjList.size(), -1); // stores prev vertex of optiomal path
        queue<int> q; // holds the vertexes
        int currentNode; // curr node for traversal
        vector<int> path;
        int i;

        distance[start - 1] = 0; // distance from the start to itself is 0
        q.push(start - 1); // begin traversing w starting node

        // traverse and pop out the node until the queue is empty
        while (!q.empty()) {
            currentNode = q.front(); // take the first node in the queue
            q.pop(); // pop it out

            // look at neigbors of the node
            for (auto neighbor : adjList[currentNode]) {
                // if we haven't visited this neighbor yet
                if (distance[neighbor] == 10000) {
                    // add it to the queue to look at l8er
                    q.push(neighbor);
                    // set the distance of neigbor
                    distance[neighbor] = distance[currentNode] + 1;
                    prev[neighbor] = currentNode; // set prev to current
                }
            }
        }

        // backtrack from the end node to start node to build the path
        for (i = end - 1; i != -1; i = prev[i])
            path.push_back(i + 1);
        // flip her around
        reverse(path.begin(), path.end());

        // if end node is unreachable we have no path
        if (path.size() == 1 && path[0] != start)
            path =  {};

        return path;
    }

};

int main(int argc, char* argv[])
{
    int startVertex, endVertex; // start and end of maze
    ifstream inputFile(argv[1]); // input maze

    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <inputfile>" << endl;
        exit(0);
    }

    if (!inputFile.is_open())
    {
        cout << "Could not open the file " << argv[1] << endl;
        exit(0);
    }

    // get start and end
    if (!(inputFile >> startVertex >> endVertex))
    {
        cout << "Could not read start and end vertices" << endl;
        exit(0);
    }

    int numVertices = max(startVertex, endVertex);
    int u, v; // to vertex and from vertex

    //get the number of vertices
    while (inputFile >> u)
    {
        numVertices = max(numVertices, u);
        while (inputFile.peek() != '\n' && inputFile >> v && v != -1) {
            numVertices = max(numVertices, v);
        }
    }

    inputFile.clear();
    inputFile.seekg(0, ios::beg);
    // skip the first line which contains start and end vertices
    inputFile.ignore(numeric_limits<streamsize>::max(), '\n');

    Graph graph(numVertices);

    // add edges
    while (inputFile >> u)
    {
        while (inputFile.peek() != '\n' && inputFile >> v && v != -1) {
            graph.addEdge(u, v);
        }
    }

    inputFile.close(); // close file

    vector<int> shortestPath = graph.findShortestPath(startVertex, endVertex);

    if (!shortestPath.empty()) {
        cout << argv[1] << endl;
        cout << "Start -> ";
        for (size_t i = 0; i < shortestPath.size(); ++i) {
            cout << shortestPath[i];
            if (i < shortestPath.size() - 1) {
                cout << " -> ";
            }
        }
        cout << " -> Exit\n";
        cout << "Total cost: " << shortestPath.size() + 1 << endl;
    } else {
        cout << "No path found." << endl;
    }

    return 0;
}
