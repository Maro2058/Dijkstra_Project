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

    // function to add an edge to graph
    void addEdge(int u, int v, int w);

    // function to remove an edge from the graph
    void removeEdge(int u, int v);

    // prints shortest path from s
    void shortestPath(int s);

    // Save graph to a file
    void saveGraph(const string& filename);

    // Load graph from a file
    void loadGraph(const string& filename);

    // Display the graph
    void displayGraph();
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

void Graph::removeEdge(int u, int v)
{
    adj[u].remove_if([v](const iPair& pair) { return pair.first == v; });
    adj[v].remove_if([u](const iPair& pair) { return pair.first == u; });
}

// Prints shortest paths from src to all other vertices
void Graph::shortestPath(int src)
{
    // Create a priority queue to store vertices that
    // are being preprocessed. This is weird syntax in C++.
    // Refer below link for details of this syntax
    // https://www.geeksforgeeks.org/implement-min-heap-using-stl/
    priority_queue<iPair, vector<iPair>, greater<iPair>> pq;


    // Create a vector for distances and initialize all
    // distances as infinite (INF)
    vector<int> dist(V, INF);

    // Insert source itself in priority queue and initialize
    // its distance as 0.
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
        int u = pq.top().second;
        pq.pop();

        // 'i' is used to get all adjacent vertices of a
        // vertex
        list<pair<int, int> >::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); ++i) {
            // Get vertex label and weight of current
            // adjacent of u.
            int v = (*i).first;
            int weight = (*i).second;

            // If there is shorted path to v through u.
            if (dist[v] > dist[u] + weight) {
                // Updating distance of v
                dist[v] = dist[u] + weight;
                pq.push(make_pair(dist[v], v));
            }
        }
    }

    // Print shortest distances stored in dist[]
    printf("Vertex Distance from Source\n");
    for (int i = 0; i < V; ++i)
        printf("%d \t\t %d\n", i, dist[i]);
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

Graph* createNewGraph() {
    int V;
    cout << "Enter the number of vertices: ";
    cin >> V;
    Graph* newGraph = new Graph(V);

    int E;
    cout << "Enter the number of edges: ";
    cin >> E;
    for (int i = 0; i < E; ++i) {
        int u, v, w;
        cout << "Enter edge (u v w): ";
        cin >> u >> v >> w;
        newGraph->addEdge(u, v, w);
    }

    return newGraph;
}

void saveGraph(Graph* graph) {

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

Graph* loadGraph() {

    //return graph;
}

void Graph::editGraph() {
    
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
        cout << "Enter your choice: ";
        cin >> choice;

        int u, v, w, src;
        string filename;
        switch (choice) {
            case 1:
                delete g;
                g = createNewGraph();
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
