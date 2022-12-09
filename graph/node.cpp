#include "node.h"

Node::Node(int inf)
{
    info = inf;
}

Node::Node(QPointF cood, int inf)
{
    coord = cood;
    info = inf;
}
