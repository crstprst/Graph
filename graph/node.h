#ifndef NODE_H
#define NODE_H

#include <QPoint>
#include <math.h>
#include <QDebug>

class Node
{
    int info;
    QPointF coord;

    bool isEntrance = false;
    bool isExit = false;
public:
    bool isPath = false;
    bool isWall = true;

    Node(int inf = -1);
    Node(QPointF cood, int inf = -1);

    void setInfo(int inf){info = inf;}
    void setCoord(QPointF c){coord = c;}
    void setEntrance(bool e){isEntrance = e;}
    void setExit(bool e){isExit = e;}

    QPointF getCoordinate(){return coord;}
    int getInfo(){return info;}
    bool getExit(){return isExit;}
    bool getEntrance(){return isEntrance;}

    float distance(Node a, Node b){
        return std::sqrt(std::pow(a.getCoordinate().x() - b.getCoordinate().x(), 2) +
                         std::pow(a.getCoordinate().y() - b.getCoordinate().y(), 2));
    }
    float distance(QPointF point){
        return std::sqrt(std::pow(this->coord.x() - point.x(), 2) + std::pow(this->coord.y() - point.y(), 2));
    }

    friend bool operator==(const Node& node1, const Node& node2){
        return ((node1.coord.x() == node2.coord.x()) && (node1.coord.y() == node2.coord.y()));
    }
};

#endif // NODE_H
