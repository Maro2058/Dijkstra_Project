// Program to find Dijkstra's shortest path using
// priority_queue in STL f

#include <iostream>
#include <cstdlib>
#include <bits/stdc++.h>
#include<fstream>
#include <filesystem>
#include <SFML/Graphics.hpp>
#include "DLL.cpp"
#include "DLL.h"
#include <SFML/Graphics.hpp>

using namespace std;

namespace fs = std::filesystem;

#define INF 0x3f3f3f3f

// iPair ==> Integer Pair
typedef pair<int, int> iPair;


template<typename T, typename Compare = std::greater<T>>

class PriorityQueue {
    std::vector<T> data;
    Compare comp;

public:
    bool empty() const {
        return data.empty();
    }

    void push(const T& value) {
        data.push_back(value);
        std::push_heap(data.begin(), data.end(), comp);
    }

    void pop() {
        std::pop_heap(data.begin(), data.end(), comp);
        data.pop_back();
    }

    T top() const {
        return data.front();
    }
};

// This class represents a directed graph using
// adjacency list representation
class Graph {
    int V; // No. of vertices
    // In a weighted graph, we need to store vertex
    // and weight pair for every edge
    map<string, int> nameToIndex;
    map<int, string> indexToName;
    DLL<pair<int, int>>* adj;

public:
    Graph(int V); // Constructor
    Graph();
    // function to add an edge to graph
    void addEdge(const string& u, const string& v, int w);
    // function to add and remove vertex
    void addVertex(const string& vertex);
    void removeVertex(const string& vertex);
    int getvertices()const;
    // function to remove an edge from the graph
    void removeEdge(const string& u, const string& v);
    void printPath(vector<int>& prevVertex, int i);
    // prints shortest path from s
    void shortestPath(const string& s);
    // Save graph to a file
    void saveGraph(const string& filename);
    // Load graph from a file
    static void loadGraph(Graph*& g);
    static void removeGraph();
    // Display the graph
    void displayGraph();
    void editGraph();
    void addNodeName(int index, const string& name);

    void visualize();


    friend vector<string> findFile();
    Graph& operator=(const Graph& other);

};

Graph& Graph::operator=(const Graph& other) {
    if (this != &other) { // Check for self-assignment
        // Copy data members
        V = other.V;
        // Copy adjacency lists
        adj = new DLL<pair<int, int>>[V];
        for (int i = 0; i < V; ++i) {
            adj[i] = other.adj[i];
        }
        // Copy indexToName and nameToIndex maps
        indexToName = other.indexToName;
        nameToIndex = other.nameToIndex;
    }
    return *this;
}


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


int Graph::getvertices()const
{
    return V;
}

void Graph::addVertex(const string& vertex) {
    if (nameToIndex.find(vertex) != nameToIndex.end()) {
        cout << "Error: Vertex '" << vertex << "' already exists." << endl;
        return;
    }

    auto* newGraph = new Graph(V+1);
    for (int i = 0; i < V; i++) {
        newGraph->adj[i] = adj[i];
        newGraph->indexToName[i] = indexToName[i];
        newGraph->nameToIndex[indexToName[i]] = nameToIndex[indexToName[i]];
    }

    newGraph->nameToIndex[vertex] = V;
    newGraph->indexToName[V] = vertex;
    cout << "Error: Vertex " << vertex << " added." << endl;
    *this = *newGraph;
    delete newGraph;
}

void Graph::removeVertex(const string& vertex) {
    if (nameToIndex.find(vertex) == nameToIndex.end()) {

        cout << "Error: Vertex '" << vertex << "' does not exist." << endl;

        return;
    }

    int index = nameToIndex[vertex];


    // Remove the vertex from the adjacency list and maps
    for (int u = index; u < V-1; u++) {
        adj[u] = adj[u + 1];
        indexToName[u]= indexToName[u+1];
        nameToIndex[indexToName[u]] = u;
    }

    // Remove all edges associated with this vertex
    for (int u = 0; u < V; u++) {
        DLLNode<pair<int, int>>* current = adj[u].begin();

        while (current != adj[u].end()) {
            DLLNode<pair<int, int>>*nextNode = current->next;
            if (current->info.first == index) {
                adj[u].deleteNode(current->info);
            } else if(current->info.first >= index) {
                current->info.first--;
            }
            current = nextNode;
        }
    }

    auto* newGraph = new Graph(V-1);
    for (int i = 0; i < V-1; i++) {
            newGraph->adj[i] = adj[i];
            newGraph->indexToName[i] = indexToName[i];
            newGraph->nameToIndex[indexToName[i]] = nameToIndex[indexToName[i]];
    }

    cout << "Vertex " << vertex << " added." << endl;
    *this = *newGraph;
    delete newGraph;
}

void Graph::addEdge(const string& u, const string& v, int w) {
    if (nameToIndex.find(u) == nameToIndex.end() && nameToIndex.find(v) == nameToIndex.end()) {
        cout << "Error: Vertex '" << u <<"' and Vertex '"<< v <<"' do not exist." << endl;
        return;
    }
    if (nameToIndex.find(u) == nameToIndex.end()) {
        cout << "Error: Vertex '" << u << "' does not exist." << endl;
        return;
    }
    if (nameToIndex.find(v) == nameToIndex.end()) {
        cout << "Error: Vertex '" << v << "' does not exist." << endl;
        return;
    }

    int uIndex = nameToIndex[u];
    int vIndex = nameToIndex[v];
    cout << "\nAdding edge from '" << u << "' to '" << v << "' with weight " << w << endl;
    adj[uIndex].push_back(make_pair(vIndex, w));
    adj[vIndex].push_back(make_pair(uIndex, w));
    cout << "Edge added from '" << u << "' to '" << v << "' with weight " << w << endl;
}

void Graph::removeEdge(const string& u, const string& v) {
    if (nameToIndex.find(u) == nameToIndex.end() && nameToIndex.find(v) == nameToIndex.end()) {
        cout << "Error: Vertex '" << u <<"' and Vertex '"<< v <<"' do not exist." << endl;
        return;
    }
    if (nameToIndex.find(u) == nameToIndex.end()) {
        cout << "Error: Vertex '" << u << "' does not exist." << endl;
        return;
    }
    if (nameToIndex.find(v) == nameToIndex.end()) {
        cout << "Error: Vertex '" << v << "' does not exist." << endl;
        return;
    }

    int uIndex = nameToIndex[u];
    int vIndex = nameToIndex[v];

    // Remove all edges from u to v
    DLLNode<iPair>* current = adj[uIndex].begin();
    while (current != adj[uIndex].end()) {
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
    cout << "Edge(s) removed between '" << u << "' and '" << v << "'.\n";
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
    if (nameToIndex.find(s) == nameToIndex.end()) {
        cout << "Error: Vertex " << s << " does not exist." << endl;
        return;
    }
    int src = nameToIndex[s];

    // Create a priority queue to store vertices that
    // are being preprocessed.
    PriorityQueue<iPair, greater<iPair>> pq;


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

vector<string> findFile(const string& foldername) {
    vector<string> txt_files;
    // Iterate over each file in the folder
    for (const auto& entry : fs::directory_iterator(foldername)) {
        // Check if the entry is a regular file and has a .txt extension
        if (fs::is_regular_file(entry.path()) && entry.path().extension() == ".txt") {
            string filename1 = entry.path().filename().string();
            txt_files.push_back(filename1); // Store the filename in a vector
        }
    }

    if (txt_files.empty()) {
        std::cerr << "No graphs found in folder: " << foldername << std::endl;
        return txt_files;
    }
    return txt_files;
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

    vector<string> txt_files = findFile(foldername);

    // Iterate over each file in the folder
    for (const auto& entry : txt_files ) { // This loop iterates over each entry found in the directory.
            if(entry == Filename)
            {
                cout<<"name already exists, Do you wish to overwrite?"<<endl<<"1.Yes\n2.No"<<endl;
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

void Graph::loadGraph(Graph*& g) {
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

    vector<string> txt_files = findFile(foldername);

    // Print available files for selection
    cout << "Available .txt files in folder: " <<endl;
    for (size_t i = 0; i < txt_files.size(); ++i) {
        cout << i + 1 << ". " << txt_files[i] << endl;
    }

    // User selects a file
    int choice;
    cout << "Enter the number corresponding to the file you want to load: ";
    cin >> choice;

    while (choice < 1 || choice > static_cast<int>(txt_files.size())) {
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

void Graph::removeGraph() {
    fs::path folderPath = fs::current_path();
    string foldername = folderPath.string();
    vector<string> txt_files = findFile(foldername);

    // Print available files for selection
    cout << "Available .txt files in folder: " <<endl;
    for (size_t i = 0; i < txt_files.size(); ++i) {
        cout << i + 1 << ". " << txt_files[i] << endl;
    }

    int choice;
    cout << "Enter the number corresponding to the file you want to remove: ";
    cin >> choice;

    while (choice < 1 || choice > static_cast<int>(txt_files.size())) {
        cerr << "Invalid choice. Please enter a valid number." << std::endl;
        return;
    }
    string filename = txt_files[choice -1];
    if(remove(filename.c_str()) == 0)
        cout<< "File successfully deleted "<<endl;
    else
        cout<<"Deletion Failed"<<endl;
    }

Graph* createNewGraph(int V, int E) {
    auto* newGraph = new Graph(V);
    vector<string> vertexNames(V);

    // Input vertex names
    cin.ignore();
    for (int i = 0; i < V; ++i) {
        string name;
        cout << endl;
        cout << "Enter name for vertex " << i << ": ";
        getline(cin, name);
        vertexNames[i] = name;
        newGraph->addNodeName(i, name);

    }

    // Input edges
    for (int i = 0; i < E; ++i) {
        string u, v;
        int w;
        bool validInput = false;

        // Validate starting vertex
        while (!validInput) {
            cout << "\nEnter your edge's Starting Point: ";
            getline(cin, u);

            if (find(vertexNames.begin(), vertexNames.end(), u) == vertexNames.end()) {
                cout << "Error: Vertex '" << u << "' does not exist. Please try again." << endl;
            } else {
                validInput = true;
            }
        }

        validInput = false;

        // Validate destination vertex
        while (!validInput) {
            cout << "Enter your edge's Destination Point: ";
            getline(cin, v);

            if (find(vertexNames.begin(), vertexNames.end(), v) == vertexNames.end()) {
                cout << "Error: Vertex '" << v << "' does not exist. Please try again." << endl;
            } else {
                validInput = true;
            }
        }

        // Validate edge weight

        while (true) {
            cout << "Enter your edge's weight (distance): ";
            cin >> w;

            if (cin.fail()) {
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
                cout << "Error: Weight must be an integer. Please try again." << endl;
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
                break; // Input is valid, break out of the loop
            }
        }
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
    int choice;
    do {

        cout << "\nEdit Graph Menu:\n";
        cout << "1. Add Vertex \n";
        cout << "2. Remove Vertex (WIP)\n";
        cout << "3. Add Edge\n";
        cout << "4. Remove Edge\n";
        cout << "5. Back to Main Menu\n";
        cout << "Enter your choice: ";

        cin >> choice;
        cin.ignore();
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
        }

        string vertexName, uName, vName;
        int w;

        switch (choice) {
            case 1:
                system("cls");
                displayGraph();

                cout << "Enter vertex name to add: ";
                getline(cin, vertexName);

                system("cls");
                addVertex(vertexName);
                break;

            case 2:
                system("cls");
                displayGraph();

                cout << "Enter vertex name to remove: ";
                getline(cin, vertexName);

                system("cls");
                removeVertex(vertexName);
                break;

            case 3:
                system("cls");
                displayGraph();

                cout << "Enter your edge's Starting Point: ";
                getline(cin, uName);
                cout << "Enter your edge's Destination Point: ";
                getline(cin, vName);
                while (true) {
                    cout << "Enter your edge's weight (distance): ";
                    cin >> w;
                    if (cin.fail()) {
                        cin.clear(); // Clear the error flag
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
                        cout << "Error: Weight must be an integer. Please try again." << endl;
                    } else {
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the newline character from the input buffer
                        break; // Input is valid, break out of the loop
                    }
                }
                system("cls");
                addEdge(uName, vName, w);
                break;

            case 4:
                system("cls");
                displayGraph();

                cout << "Enter Starting Point of the edge to remove: ";
                getline(cin, uName);
                cout << "Enter Destination Point of the edge to remove: ";
                getline(cin, vName);

                system("cls");
                removeEdge(uName, vName);
                break;

            case 5:
                system("cls");
                return; // Back to Main Menu

            default:
                system("cls");
                cout << "Invalid choice. Please try again.\n";
        }
    } while (true);
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

void Graph :: visualize() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Graph Visualization");

    std::map<int, sf::CircleShape> vertexShapes;
    std::map<int, sf::Vector2f> vertexPositions;

    float radius = 20.0f;
    float angleStep = 360.0f / V;
    float centerX = window.getSize().x / 2;
    float centerY = window.getSize().y / 2;
    float distance = 200.0f;

    // Load a font
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) { // Ensure the font file is in the project directory
        std::cerr << "Failed to load font 'arial.ttf'" << std::endl;
        return;
    }

    std::map<int, sf::Text> vertexLabels;

    for (int i = 0; i < V; ++i) {
        sf::CircleShape circle(radius);
        float angle = i * angleStep * (3.14159f / 180.0f);
        float x = centerX + distance * cos(angle);
        float y = centerY + distance * sin(angle);

        circle.setPosition(x, y);
        circle.setFillColor(sf::Color::Blue);

        vertexShapes[i] = circle;
        vertexPositions[i] = sf::Vector2f(x + radius, y + radius); // Center of the circle

        // Create vertex label
        sf::Text label;
        label.setFont(font);
        label.setString(indexToName[i]); // You can replace i with the actual vertex name if available
        label.setCharacterSize(15);
        label.setFillColor(sf::Color::White);
        label.setPosition(x + radius / 2, y + radius / 2); // Adjust position as needed

        vertexLabels[i] = label;
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Set background color
        window.clear(sf::Color(50, 50, 50)); // Change RGB values for different background color

        // Draw edges
        for (int u = 0; u < V; ++u) {
            for (DLLNode<pair<int, int>>* i = this->adj[u].begin(); i != this->adj[u].end(); i = i->next) {
                int v = i->info.first;
                sf::Vertex line[] = {
                        sf::Vertex(vertexPositions[u], sf::Color::White),
                        sf::Vertex(vertexPositions[v], sf::Color::White)
                };
                window.draw(line, 2, sf::Lines);
            }
        }

        // Draw vertices
        for (const auto& [index, shape] : vertexShapes) {
            window.draw(shape);
        }

        // Draw vertex labels
        for (const auto& [index, label] : vertexLabels) {
            window.draw(label);
        }

        window.display();
    }
}


void interactiveMenu(Graph*& g) {
    int choice;
    do {
        cout << "\nMenu:\n";
        cout << "1. Create New Graph\n";
        cout << "2. Edit Graph\n";
        cout << "3. Display Graph\n";
        cout << "4. Find Shortest Path\n";
        cout << "5. Save Graph\n";
        cout << "6. Load Graph\n";
        cout << "7. Remove Graph\n";
        cout << "8. Exit\n";
        cout << "9. TempGraph\n";
        cout << "Enter your choice: ";

        cin >> choice;
        cin.ignore();
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
        }

        string vertexName, uName, vName, srcName, filename;

        switch (choice) {
            case 1:
                system("cls");

                delete g;
                int V, E;

                while (true) {
                    cout << "Enter the number of vertices: ";
                    cin >> V;

                    if (cin.fail()) {
                        cin.clear(); // Clear the error flag
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
                        cout << "Error: Must be an integer. Please try again." << endl;
                    } else {
                        break; // Input is valid, break out of the loop
                    }
                }

                while (true) {
                    cout << "Enter the number of edges: ";
                    cin >> E;

                    if (cin.fail()) {
                        cin.clear(); // Clear the error flag
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
                        cout << "Error: Must be an integer. Please try again." << endl;
                    } else {
                        break; // Input is valid, break out of the loop
                    }
                }
                g = createNewGraph(V, E);
                cout << "\nNew graph created with " << V << " vertices and " << E << " edges.\n";
                break;

            case 2:
                system("cls");

                if (g == nullptr) {
                    cout << "Error: No graph exists. Please create/load a new graph first.\n";
                    break;
                }
                cout << "Editing the graph...\n";
                g->editGraph();
                cout << "Graph edited.\n";
                break;

            case 3:
                system("cls");

                if (g == nullptr) {
                    cout << "Error: No graph exists. Please create/load a new graph first.\n";
                    break;
                }

                g->visualize();
                break;

            case 4:
                system("cls");

                if (g == nullptr) {
                    cout << "Error: No graph exists. Please create/load a new graph first.\n";
                    break;
                }
                cout << "Enter source vertex for shortest path: ";
                getline(cin,srcName);

                g->shortestPath(srcName);
                break;

            case 5:
                system("cls");

                if (g == nullptr) {
                    cout << "Error: No graph exists. Please create/load a new graph first.\n";
                    break;
                }
                cout << "Enter filename to save graph: ";
                getline(cin,filename);
                g->saveGraph(filename);
                cout << "Graph saved to " << filename << ".\n";
                break;

            case 6:
                system("cls");
                Graph::loadGraph(g);
                cout << "Graph loaded from file.\n";
                break;
            case 7:
                system("cls");
                Graph::removeGraph();
                break;
            case 8:
                cout << "Exiting...\n";
                break;
            case 9:
                tempGraph(g);
                cout << "Temporary graph loaded.\n";
                break;

            default:
                system("cls");
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 7);
}


int main()
{

    Graph* g = nullptr;
    interactiveMenu(g);

    delete g;
    return 0;
}
