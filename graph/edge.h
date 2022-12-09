#ifndef EDGE_H
#define EDGE_H
#include "node.h"
#include <vector>

class Edge
{
    Node firstNode, secondNode;

public:
    Edge();
    Edge(Node fN, Node sN){
        firstNode = fN;
        secondNode = sN;
    }

    Node getFirstNode(){return firstNode;}
    Node getSecondNode(){return secondNode;}
    friend bool operator == (const Edge& edge1, const Edge& edge2){
        return ((edge1.firstNode == edge2.firstNode) && (edge1.secondNode == edge2.secondNode));
    }

};

#endif // EDGE_H
