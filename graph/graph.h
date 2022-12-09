#ifndef GRAPH_H
#define GRAPH_H

#include "edge.h"
#include <vector>
#include <QRandomGenerator>
#include <map>
#include <QMainWindow>
#include <stack>
#include <QMessageBox>

using namespace std;

class Graph
{
private:
    int** matrix;
    vector<Node> nodes;
    vector<Edge> edges;
    map<int , vector<int>> list;
    bool cycle = false;
    int root = 0;
    
    pair<int, int> readLabyrinth(std::string fileName);

public:
    bool isOriented = false;

    Graph();
    Graph(Graph& g);
    ~Graph();

    void addNode(Node n) {nodes.push_back(n);}
    void addEdge(Edge a) {edges.push_back(a);}
    void setNodes(vector<Node> n){nodes = n;}
    void setEdges(vector<Edge> e){edges = e;}

    int getNumberOfNodes(){return (int)nodes.size();}
    vector<Node>* getNodesAddress() {return &nodes;}
    vector<Edge>* getEdgesAddress() {return &edges;}
    vector<Node> getNodes() {return nodes;}
    vector<Edge> getEdges(){return edges;}
    int** getMatrix(){return matrix;}
    int getRoot() { return root; }

    void updateMatrix();
    void uploadMatrix();
    void updateList();
    void uploadList();

    void clearNodes(){nodes.clear();}
    void clearEdges(){edges.clear();}

    void generateGraph(QMainWindow* q);

    void BFS();
    std::vector<int> DFS(Node start);
    std::vector<int> DFS(std::vector<Node> start);

    std::vector<Node> findRoots();
    bool hasCycle() { return cycle; }
};

void printToFile(std::string fileName, std::vector<int> v);

#endif // GRAPH_H
