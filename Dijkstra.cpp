// Program to find Dijkstra's shortest path using
// priority_queue in STL

#include <bits/stdc++.h>
#include<fstream>
#include <filesystem>
#include "DLL.h"
using namespace std;
namespace fs = std::filesystem;
#define INF 0x3f3f3f3f


// iPair ==> Integer Pair
typedef pair<int, int> iPair;



template<typename T>
class PriorityQueue{

};

// This class represents a directed graph using
// adjacency list representation
class Graph {
    int V; // No. of vertices
    // In a weighted graph, we need to store vertex
    // and weight pair for every edge
    map<string, int> nameToIndex;
    map<int, string> indexToName;

public:
    DLL<pair<int, int>>* adj;
    Graph(int V); // Constructor
    Graph();
    // function to add an edge to graph
    void addEdge(const string& u, const string& v, int w);
    void addEdge(int u, int v, int w);
    // function to add and remove vertex
    void addVertex(const string& vertex);
    void removeVertex(const string& vertex);
    // function to remove an edge from the graph
    void removeEdge(const string& u, const string& v);
    void printPath(vector<int>& prevVertex, int i);
    // prints shortest path from s
    void shortestPath(const string& s);
    // Save graph to a file
    void saveGraph(const string& filename);
    // Load graph from a file
    // Display the graph
    void displayGraph();
    void editGraph();
    void addNodeName(int index, const string& name);

};

// Allocates memory for adjacency list
Graph::Graph(int V)
{
    this->V = V;
    adj = new DLL<iPair>[V];
}

void Graph::addNodeName(int index, const string& name) {
    nameToIndex[name] = index;
    // cant add same name or same index!!
    indexToName[index] = name;
}
void Graph::addVertex(const string& vertex) {
    if (nameToIndex.find(vertex) != nameToIndex.end()) {
        cout << "Vertex " << vertex << " already exists." << endl;
        return;
    }

    int newIndex = V;
    V++;

    // Expand the adjacency list
    DLL<pair<int, int>>* newAdj = new DLL<iPair>[V];
    for (int i = 0; i < V - 1; ++i) {
        newAdj[i] = adj[i];
    }
    delete[] adj;
    adj = newAdj;

    // Add the new vertex to the maps
    nameToIndex[vertex] = newIndex;
    indexToName[newIndex] = vertex;

    cout << "Vertex " << vertex << " added." << endl;
}

void Graph::removeVertex(const string& vertex) {
    if (nameToIndex.find(vertex) == nameToIndex.end()) {
        cout << "Vertex " << vertex << " does not exist." << endl;
        return;
    }

    int index = nameToIndex[vertex];

    // Remove all edges associated with this vertex
    for (int u = 0; u < V; ++u) {
        DLLNode<pair<int, int>>* current = adj[u].begin();
        while (current != nullptr) {
            DLLNode<pair<int, int>>* nextNode = current->next;
            if (current->info.first == index) {
                adj[u].deleteNode(current->info);
            }
            current = nextNode;
        }
    }

    // Remove the vertex from the adjacency list and maps
    for (int u = index; u < V - 1; ++u) {
        adj[u] = adj[u + 1];
        indexToName[u] = indexToName[u + 1];
        nameToIndex[indexToName[u]] = u;
    }
    V--;

    DLL<iPair>* newAdj = new DLL<iPair>[V];
    for (int i = 0; i < V; ++i) {
        newAdj[i] = adj[i];
    }
    delete[] adj;
    adj = newAdj;

    nameToIndex.erase(vertex);
    indexToName.erase(V);

    cout << "Vertex " << vertex << " removed." << endl;
}


void Graph::addEdge(const string& u, const string& v, int w) {
    int uIndex = nameToIndex[u];
    int vIndex = nameToIndex[v];
    cout << "Adding edge from " << u << " to " << v << " with weight " << w << endl;
    adj[uIndex].push_back(make_pair(vIndex, w));
    adj[vIndex].push_back(make_pair(uIndex, w));
}


void Graph::addEdge(int u, int v, int w) {
    adj[u].push_back(make_pair(v, w));
    adj[v].push_back(make_pair(u, w));
}

void Graph::removeEdge(const string& u, const string& v) {
    int uIndex = nameToIndex[u];
    int vIndex = nameToIndex[v];

    // Remove all edges from u to v
    DLLNode<iPair>* current = adj[uIndex].begin();
    while (current != nullptr) {
        DLLNode<iPair>* nextNode = current->next;
        if (current->info.first == vIndex) {
            adj[uIndex].deleteNode(current->info);
        }
        current = nextNode;
    }

    // Remove all edges from v to u
    current = adj[vIndex].begin();
    while (current != nullptr) {
        DLLNode<iPair>* nextNode = current->next;
        if (current->info.first == uIndex) {
            adj[vIndex].deleteNode(current->info);
        }
        current = nextNode;
    }
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

        for (DLLNode<pair<int,int>>* i = adj[u].begin(); i != adj[u].end(); i = i->next) {
            // Get vertex label and weight of current
            // adjacent of u.
            int targetNode = i->info.first;
            int weight = i->info.second;

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
void Graph::saveGraph(const string& f) {
    string Filename = f +".txt";

    fs::path folderPath = fs::current_path();
    string foldername = folderPath.string();
    if (!fs::exists(foldername)) {
        std::cerr << "Folder does not exist: " << foldername << std::endl;
        return;
    }

    vector<string> txt_files;

    // Iterate over each file in the folder
    //fs::directory_iterator(foldername) this creates an iterator to loop through the contents of the directory specified by foldername.
    for (const auto& entry : fs::directory_iterator(foldername) ) { // This loop iterates over each entry found in the directory.
        // Check if the entry is a regular file and has a .txt extension
        if (fs::is_regular_file(entry.path()) && entry.path().extension() == ".txt") {
            string filename1 = entry.path().filename().string();
            if(filename1 == Filename)
            {
                cout<<"name already exists, Do you want to overwrite?"<<endl<<"1.Yes\n2.No"<<endl;
                int choice;
                cin>>choice;
                while(choice > 2 || choice < 1)
                {
                    cout<<"Out of range please pick between 1 and 2"<<endl;
                    cin.clear();
                    cin>>choice;
                }
                switch(choice)
                {
                    case 1:
                        break;
                    case 2:
                        cout<<"Save Operation cancelled"<<endl;
                        return;
                }
            }
        }
    }
    ofstream outFile(Filename);
    if (!outFile) {
        cerr << "Error opening file for writing: " << Filename << endl;
        return;
    }

    // Write number of vertices and edges
    outFile << V << endl;

    // Write vertex names
    for (int u = 0; u < V; ++u) {
        outFile << u << "|" << indexToName[u];
        for (DLLNode<pair<int,int>>* i = adj[u].begin(); i != adj[u].end(); i = i->next) { //get all the pairs
            outFile << "|" << i->info.first << "," << i->info.second;
        }
        outFile << endl;
    }

    outFile.close();
    if (outFile.fail()) {
        cerr << "Error occurred while saving the graph to file." << endl;
    } else {
        cout << "Graph saved successfully to " << Filename << endl;
    }
}

void loadGraph(Graph*& g) {
    if (g) {
        delete g;// delete the graph if there is anything in there already
        g = nullptr;
    }
    fs::path folderPath = fs::current_path();
    string foldername = folderPath.string();
    if (!fs::exists(foldername)) {
        std::cerr << "Folder does not exist: " << foldername << std::endl;
        return;
    }

    vector<string> txt_files;

    // Iterate over each file in the folder
    for (const auto& entry : fs::directory_iterator(foldername)) {
        // Check if the entry is a regular file and has a .txt extension
        if (fs::is_regular_file(entry.path()) && entry.path().extension() == ".txt") {
            string filename1 = entry.path().filename().string();
            txt_files.push_back(filename1); // Store the filename in a vector
            cout << "Found file: " << filename1 << std::endl;
        }
    }

    if (txt_files.empty()) {
        std::cerr << "No .txt files found in folder: " << foldername << std::endl;
        return;
    }

    // Print available files for selection
    cout << "Available .txt files in folder: " <<endl;
    for (size_t i = 0; i < txt_files.size(); ++i) {
        cout << i + 1 << ". " << txt_files[i] << endl;
    }

    // User selects a file
    int choice;
    cout << "Enter the number corresponding to the file you want to load: ";
    cin >> choice;

    if (choice < 1 || choice > static_cast<int>(txt_files.size())) {
        cerr << "Invalid choice. Please enter a valid number." << std::endl;
        return;
    }

    ifstream inFile(txt_files[choice - 1]);
    if (!inFile) {
        cerr << "Error opening file for reading " << endl;
        return;
    }
    // Read number of vertices
    int numVertices;
    inFile >> numVertices;
    g = new Graph(numVertices);
    inFile.ignore(numeric_limits<streamsize>::max(), '\n');// Ignore the rest of the line
    // Read vertex names and edges
    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string token;
        vector<string> tokens;

        // Split the line by '|'
        while (getline(ss, token, '|')) {
            tokens.push_back(token);
        }

        // First token is the index
        int index = stoi(tokens[0]);

        // Second token is the name
        string name = tokens[1];
        g->addNodeName(index, name);

        // Remaining tokens are edges
        for (size_t i = 2; i < tokens.size(); ++i) {
            stringstream ss1(tokens[i]);
            string vertex, weight;
            getline(ss1, vertex, ',');
            getline(ss1, weight);
            int vIndex = stoi(vertex);
            int w = stoi(weight);
            g->adj[index].push_back(make_pair(vIndex, w));
        }

    }

    inFile.close();
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
        for (DLLNode<pair<int,int>>* i = adj[u].begin(); i != adj[u].end(); i = i->next) {
            cout << "\tVertex " << indexToName[i->info.first] << " with weight=" << i->info.second << endl;
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

    g->displayGraph();
}

void interactiveMenu(Graph*& g) {
    int choice;
    do {
        cout << "\nMenu:\n";
        cout << "1. Create New Graph\n";
        cout << "2. Add Vertex\n";
        cout << "3. Remove Vertex\n";
        cout << "4. Add Edge\n";
        cout << "5. Remove Edge\n";
        cout << "6. Display Graph\n";
        cout << "7. Find Shortest Path\n";
        cout << "8. Save Graph\n";
        cout << "9. Load Graph\n";
        cout << "10. Exit\n";
        cout << "11. TempGraph\n";
        cout << "Enter your choice: ";
        cin >> choice;

        string vertexName, uName, vName, srcName, filename;
        int w;
        switch (choice) {
            case 1:
                delete g;
                int V, E;
                cout << "Enter the number of vertices: ";
                cin >> V;
                cout << "Enter the number of edges: ";
                cin >> E;

                g = createNewGraph(V, E);
                cout << "New graph created with " << V << " vertices and " << E << " edges.\n";
                break;

            case 2:
                cout << "Enter vertex name to add: ";
                cin >> vertexName;
                g->addVertex(vertexName);
                break;

            case 3:
                cout << "Enter vertex name to remove: ";
                cin >> vertexName;
                g->removeVertex(vertexName);
                break;

            case 4:
                cout << "Enter edge (source destination weight): ";
                cin >> uName >> vName >> w;
                g->addEdge(uName, vName, w);
                cout << "Edge added from " << uName << " to " << vName << " with weight " << w << ".\n";
                break;

            case 5:
                cout << "Enter edge to remove (source destination): ";
                cin >> uName >> vName;
                g->removeEdge(uName, vName);
                cout << "Edge removed between " << uName << " and " << vName << ".\n";
                break;

            case 6:
                cout << "Displaying the graph:\n";
                g->displayGraph();
                break;

            case 7:
                cout << "Enter source vertex for shortest path: ";
                cin >> srcName;
                g->shortestPath(srcName);
                break;

            case 8:
                cout << "Enter filename to save graph: ";
                cin >> filename;
                g->saveGraph(filename);
                cout << "Graph saved to " << filename << ".\n";
                break;

            case 9:
                loadGraph(g);
                cout << "Graph loaded from file.\n";
                break;

            case 10:
                cout << "Exiting...\n";
                break;

            case 11:
                cout << "Loading temporary graph...\n";
                tempGraph(g);
                cout << "Temporary graph loaded.\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 10);
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
