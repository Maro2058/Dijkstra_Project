
#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <iostream>
#include <cstdlib>
#include <bits/stdc++.h>
#include <fstream>
#include <filesystem>
#include "DLL.h"
#include "PriorityQueue.h"
using namespace std;

namespace fs = std::filesystem;

#define INF 0x3f3f3f3f

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
    friend vector<string> findFile();
    Graph& operator=(const Graph& other);
};

void interactiveMenu(Graph*& g);

#endif //DIJKSTRA_H
