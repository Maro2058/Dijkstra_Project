// Program to find Dijkstra's shortest path using
// priority_queue in STL
#include <bits/stdc++.h>
using namespace std;
#define INF 0x3f3f3f3f

// iPair ==> Integer Pair
typedef pair<int, int> iPair;

// This class represents a directed graph using
// adjacency list representation
class Graph {
    int V; // No. of vertices
    // In a weighted graph, we need to store vertex
    // and weight pair for every edge
    list<pair<int, int>>* adj;

public:
    Graph(int V); // Constructor
    Graph();
    // function to add an edge to graph
    void addEdge(int u, int v, int w);
    // function to remove an edge from the graph
    void removeEdge(int u, int v);
    void printPath(vector<int>& prevVertex, int i);
    // prints shortest path from s
    void shortestPath(int s);
    // Save graph to a file
    void saveGraph(const string& filename);
    // Load graph from a file
    void loadGraph(const string& filename);
    // Display the graph
    void displayGraph();
    void editGraph();
};

// Allocates memory for adjacency list
Graph::Graph(int V)
{
    this->V = V;
    adj = new list<iPair>[V];
}

void Graph::addEdge(int u, int v, int w)
{
    adj[u].push_back(make_pair(v, w));
    adj[v].push_back(make_pair(u, w));
}

void Graph::removeEdge(int u, int v) {
    adj[u].remove_if([v](const iPair& pair) { return pair.first == v; });
    adj[v].remove_if([u](const iPair& pair) { return pair.first == u; });
}

void Graph::printPath(vector<int>& prevVertex, int i) {
    if (prevVertex[i] == -1) {
        cout << i ;
        return;
    }
    printPath(prevVertex, prevVertex[i]);
    cout << "->" << i ;

}

// Prints shortest paths from src to all other vertices
void Graph::shortestPath(int src) {
    // Create a priority queue to store vertices that
    // are being preprocessed. This is weird syntax in C++.
    // Refer below link for details of this syntax
    // https://www.geeksforgeeks.org/implement-min-heap-using-stl/
    priority_queue<iPair, vector<iPair>, greater<iPair>> pq;


    // Create a vector for distances and initialize all
    // distances as infinite (INF)
    vector<int> dist(V, INF);
    vector<int> prevVertex(V, -1);

    // Insert source itself in priority queue and initialize
    // its distance as 0.
    //pair is (weightFromSource, Node)
    pq.push(make_pair(0, src));
    dist[src] = 0;

    /* Looping till priority queue becomes empty (or all
    distances are not finalized) */
    while (!pq.empty()) {
        // The first vertex in pair is the minimum distance
        // vertex, extract it from priority queue.
        // vertex label is stored in second of pair (it
        // has to be done this way to keep the vertices
        // sorted distance (distance must be first item
        // in pair)
        int u = pq.top().second; //u equals the node that was min distance of the prev node
        pq.pop();

        // 'i' is used to get all adjacent vertices of a
        // vertex
        list<pair<int, int> >::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); ++i) {
            // Get vertex label and weight of current
            // adjacent of u.
            int targetNode = (*i).first;
            int weight = (*i).second;

            // If there is shorted path to v through u.
            if (dist[targetNode] > dist[u] + weight) {
                // Updating distance of v
                dist[targetNode] = dist[u] + weight;
                pq.push(make_pair(dist[targetNode], targetNode));
                prevVertex[targetNode] = u;
            }
        }
    }

    // Print shortest distances stored in dist[]
    printf("Vertex  Distance from Source  Path\n");
    for (int i = 0; i < V; ++i){
        printf("%d \t\t %d\t\t", i, dist[i]);
        printPath(prevVertex, i);
        printf("\n");
    }
}

// Save graph to a file
void Graph::saveGraph(const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        file << V << endl;
        for (int u = 0; u < V; ++u) {
            for (auto v : adj[u]) {
                file << u << " " << v.first << " " << v.second << endl;
            }
        }
        file.close();
    } else {
        cout << "Unable to open file for writing" << endl;
    }
}

// Load graph from a file
void Graph::loadGraph(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        int u, v, w;
        file >> V;
        adj = new list<iPair>[V];
        while (file >> u >> v >> w) {
            addEdge(u, v, w);
        }
        file.close();
    } else {
        cout << "Unable to open file for reading" << endl;
    }
}

Graph* createNewGraph(int V, int E) {
    Graph* newGraph = new Graph(V);

    for (int i = 0; i < E; ++i) {
        int u, v, w;
        cout << "Enter edge (u v w): ";
        cin >> u >> v >> w;
        newGraph->addEdge(u, v, w);
    }

    return newGraph;
}

void Graph::displayGraph() {
    for (int u = 0; u < V; ++u) {
        cout << "Vertex " << u << " makes an edge with\n";
        for (auto v : adj[u]) {
            cout << "\tVertex " << v.first << " with weight=" << v.second << endl;
        }
        cout << endl;
    }
}

void Graph::editGraph() {

}

void tempGraph(Graph*& g) {
    g = createNewGraph(9,0);
    g->addEdge(0, 1, 4);
    g->addEdge(0, 7, 8);
    g->addEdge(1, 2, 8);
    g->addEdge(1, 7, 11);
    g->addEdge(2, 3, 7);
    g->addEdge(2, 8, 2);
    g->addEdge(2, 5, 4);
    g->addEdge(3, 4, 9);
    g->addEdge(3, 5, 14);
    g->addEdge(4, 5, 10);
    g->addEdge(5, 6, 2);
    g->addEdge(6, 7, 1);
    g->addEdge(6, 8, 6);
    g->addEdge(7, 8, 7);
    cout << "test";
}

void interactiveMenu(Graph*& g) {
    int choice;
    do {
        cout << "Menu:\n";
        cout << "1. Create New Graph\n";
        cout << "2. Add Edge\n";
        cout << "3. Remove Edge\n";
        cout << "4. Display Graph\n";
        cout << "5. Find Shortest Path\n";
        cout << "6. Save Graph\n";
        cout << "7. Load Graph\n";
        cout << "8. Exit\n";
        cout << "9. TempGraph\n";
        cout << "Enter your choice: ";
        cin >> choice;

        int u, v, w, src;
        string filename;
        switch (choice) {
            case 1:
                delete g;
                int V;
                cout << "Enter the number of vertices: ";
                cin >> V;
                int E;
                cout << "Enter the number of edges: ";
                cin >> E;

                g = createNewGraph(V,E);
                break;
            case 2:
                cout << "Enter edge (u v w): ";
                cin >> u >> v >> w;
                g->addEdge(u, v, w);
                break;
            case 3:
                cout << "Enter edge to remove (u v): ";
                cin >> u >> v;
                g->removeEdge(u, v);
                break;
            case 4:
                g->displayGraph();
                break;
            case 5:
                cout << "Enter source vertex: ";
                cin >> src;
                g->shortestPath(src);
                break;
            case 6:
                cout << "Enter filename to save graph: ";
                cin >> filename;
                g->saveGraph(filename);
                break;
            case 7:
                cout << "Enter filename to load graph: ";
                cin >> filename;
                g->loadGraph(filename);
                break;
            case 8:
                cout << "Exiting...\n";
                break;
            case 9:
                cout << "Graph Loaded\n";
                tempGraph(g);
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 8);
}

// Driver program to test methods of graph class
int main()
{
    // create the graph given in above figure
    Graph* g = nullptr;
    interactiveMenu(g);

    delete g;
    return 0;
}