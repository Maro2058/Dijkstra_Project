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

    map<string, int> nameToIndex;
    map<int, string> indexToName;

public:
    Graph(int V); // Constructor
    Graph();
    // function to add an edge to graph
    void addEdge(const string& u, const string& v, int w);
    // function to remove an edge from the graph
    void removeEdge(const string& u, const string& v);
    void printPath(vector<int>& prevVertex, int i);
    // prints shortest path from s
    void shortestPath(const string& s);
    // Save graph to a file
    void saveGraph(const string& filename);
    // Load graph from a file
    void loadGraph(const string& filename);
    // Display the graph
    void displayGraph();
    void editGraph();
    void addNodeName(int index, const string& name);

};

// Allocates memory for adjacency list
Graph::Graph(int V)
{
    this->V = V;
    adj = new list<iPair>[V];
}

void Graph::addNodeName(int index, const string& name) {
    nameToIndex[name] = index;
    indexToName[index] = name;
}

void Graph::addEdge(const string& u, const string& v, int w) {
    int uIndex = nameToIndex[u];
    int vIndex = nameToIndex[v];
    adj[uIndex].push_back(make_pair(vIndex, w));
    adj[vIndex].push_back(make_pair(uIndex, w));
}

void Graph::removeEdge(const string& u, const string& v) {
    int uIndex = nameToIndex[u];
    int vIndex = nameToIndex[v];
    adj[uIndex].remove_if([vIndex](const iPair& pair) { return pair.first == vIndex; });
    adj[vIndex].remove_if([uIndex](const iPair& pair) { return pair.first == uIndex; });
}

void Graph::printPath(vector<int>& prevVertex, int i) {
    if (prevVertex[i] == -1) {
        cout << indexToName[i] ;
        return;
    }
    printPath(prevVertex, prevVertex[i]);
    cout << "->" << indexToName[i] ;

}

// Prints shortest paths from src to all other vertices
void Graph::shortestPath(const string& s) {
    int src = nameToIndex[s];

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
        cout << indexToName[i] << "\t\t  " << dist[i] << "\t\t";
        printPath(prevVertex, i);
        cout << endl;

    }
}

// Save graph to a file
void Graph::saveGraph(const string& filename) {

}

// Load graph from a file
void Graph::loadGraph(const string& filename) {

}

Graph* createNewGraph(int V, int E) {
    Graph* newGraph = new Graph(V);

    for (int i = 0; i < V; ++i) {
        string name;
        cout << "Enter name for vertex " << i << ": ";
        cin >> name;
        newGraph->addNodeName(i, name);
    }

    for (int i = 0; i < E; ++i) {
        string u, v;
        int w;
        cout << "Enter edge (u v w): ";
        cin >> u >> v >> w;
        newGraph->addEdge(u, v, w);
    }

    return newGraph;
}

void Graph::displayGraph() {
    for (int u = 0; u < V; ++u) {
        cout << "Vertex " << indexToName[u] << " makes an edge with\n";
        for (auto v : adj[u]) {
            cout << "\tVertex " << indexToName[v.first] << " with weight=" << v.second << endl;
        }
        cout << endl;
    }
}

void Graph::editGraph() {

}

void tempGraph(Graph*& g) {
    g = new Graph(9);
    g->addNodeName(0, "A");
    g->addNodeName(1, "B");
    g->addNodeName(2, "C");
    g->addNodeName(3, "D");
    g->addNodeName(4, "E");
    g->addNodeName(5, "F");
    g->addNodeName(6, "G");
    g->addNodeName(7, "H");
    g->addNodeName(8, "I");
    g->addEdge("A", "B", 4);
    g->addEdge("A", "H", 8);
    g->addEdge("B", "C", 8);
    g->addEdge("B", "H", 11);
    g->addEdge("C", "D", 7);
    g->addEdge("C", "I", 2);
    g->addEdge("C", "F", 4);
    g->addEdge("D", "E", 9);
    g->addEdge("D", "F", 14);
    g->addEdge("E", "F", 10);
    g->addEdge("F", "G", 2);
    g->addEdge("G", "H", 1);
    g->addEdge("G", "I", 6);
    g->addEdge("H", "I", 7);
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
        string uName, vName, srcName, filename;
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
                g->addEdge(uName, vName, w);
                break;
            case 3:
                cout << "Enter edge to remove (u v): ";
                cin >> u >> v;
                g->removeEdge(uName, vName);
                break;
            case 4:
                g->displayGraph();
                break;
            case 5:
                cout << "Enter source vertex: ";
                cin >> srcName;
                g->shortestPath(srcName);
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
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
cin.clear();
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