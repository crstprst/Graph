#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QPainter>
#include <math.h>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isFirstNode = false;

    QLabel *label = new QLabel(this);
    label->setText("Number of nodes: \n\nNumber of paths: \n\nNumber of cycles: ");
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    QPoint labelTopLeft(10, 10);
    QPoint labelBotRight(110, 90);
    label->setGeometry(QRect(labelTopLeft, labelBotRight));

    //graph.generateGraph(this);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::RightButton)
    {
        Node n(ev->position());
        vector<Node> nodes = graph.getNodes();
        for(Node& n: nodes){
            if(n.distance(ev->position()) < nodeRadius * 2){
                return;
            }
        }
        n.setInfo(graph.getNumberOfNodes() + 1);
        graph.addNode(n);
        update();
    }

    else if (ev->button() == Qt::LeftButton)
    {
        vector<Node> nodes = graph.getNodes();
        vector<Edge> edges = graph.getEdges();
        Node selected;
        bool ok = false;

        for(Node& n: nodes)
        {
            if (n.distance(ev->position()) <= nodeRadius)
            {
                selected = n;
                ok = true;
                break;
            }
        }
        if (ok)
        {
            if (!isFirstNode)
            {
                firstNode = selected;
                isFirstNode = true;
            }
            else
            {
                for(Edge& e: edges){
                    if(e == Edge(firstNode, selected)){
                        qDebug("Exista deja muchia!!");
                        isFirstNode = false;
                        return;
                    }
                }
                graph.addEdge(Edge(firstNode, selected));
                isFirstNode= false;
                update();
            }
        }
        else
        {
            isFirstNode = false;
        }
    }
    else if (ev->button() == Qt::MiddleButton){
        std::vector<int> dfs = graph.DFS(graph.findRoots());
		printToFile("dfs.txt", dfs);
        qDebug() << "Root is:" << graph.getRoot();
    }
    graph.updateMatrix();
    graph.updateList();

}

void MainWindow::mouseMoveEvent(QMouseEvent *ev)
{
    if(ev->buttons() == Qt::RightButton){
        vector<Node>* nodes = graph.getNodesAddress();
        vector<Edge>* edges = graph.getEdgesAddress();
        Node* selected = new Node();

        for(Node& n: (*nodes)){
            if(n.distance(ev->position()) < 1.5 * nodeRadius){
                selected = &n;
                break;
            }
        }

        selected->setCoord(ev->position());

        for(Edge& e: (*edges)){
            if(e.getFirstNode().getInfo() == selected->getInfo()){
                e = Edge(*selected, e.getSecondNode());
            }
            if(e.getSecondNode().getInfo() == selected->getInfo()){
                e = Edge(e.getFirstNode(), *selected);
            }
        }
    }
    update();
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    vector<Node> nodes = graph.getNodes();
    for(Node& n:nodes)
    {
        QPointF coord = n.getCoordinate();
        QRect r(coord.x() - nodeRadius, coord.y()- nodeRadius, 2*nodeRadius, 2*nodeRadius);
        QPen pen;
        QBrush brush;
        pen.setColor(Qt::black);
        if(!n.isWall){
            brush.setColor(Qt::white);
        }
        if(n.getEntrance()){
            brush.setColor(Qt::blue);
        }
        if(n.getExit()){
            brush.setColor(Qt::red);
        }
        if(n.isPath){
            brush.setColor(Qt::green);
        }
        pen.setWidth(2);
        p.setPen(pen);
        brush.setStyle(Qt::SolidPattern);
        //p.setBrush(brush);
        p.drawEllipse(r);
        QString num = QString::number(n.getInfo());
        p.drawText(r, Qt::AlignCenter, num);
    }
    vector<Edge> edges = graph.getEdges();
    for(Edge& e:edges)
    {
        QLineF mainLine(e.getFirstNode().getCoordinate(), e.getSecondNode().getCoordinate());
        mainLine.setLength(mainLine.length() - nodeRadius);

        p.drawLine(mainLine);

        if(graph.isOriented){
            QLineF l1, l2;

            l1.setP1(mainLine.p2());
            l1.setAngle(mainLine.angle() - 135);
            l1.setLength(10);

            l2.setP1(mainLine.p2());
            l2.setAngle(mainLine.angle() + 135);
            l2.setLength(10);

            p.drawLine(l1);
            p.drawLine(l2);
        }
    }

}

void MainWindow::generateNodes()
{
    for(int i = 0; i < nNodes; i++){
        QPointF coord(std::cos(2 * M_PI * i/nNodes) * nNodes * nodeRadius + MainWindow::size().width() / 2,
                      std::sin(2 * M_PI * i/nNodes) * nNodes * nodeRadius + MainWindow::size().height() / 2);

        graph.addNode(Node(coord, i + 1));
    }
    graph.updateMatrix();
    graph.updateList();
    buildPathsAndCycles(nNodes, nPaths, nCycles);
}

void MainWindow::buildPathsAndCycles(int n, int nPaths, int nCycles)
{
    for(int i = 0; i < nPaths; i ++){
        int s = 0;
        int t = n - 1;
        vector<bool> pathNode(n, false);

        pathNode[s] = true;

        int u = s;

        for (int j = 1; j < n; j++) {
            int k = QRandomGenerator::global()->bounded(0, n - j);
            int l = 0;
            int v;
            for (v = 0; v < n; v++) {
                if(pathNode[v]){
                    continue;
                }
                if(l == k){
                    break;
                }
                l++;
            }
            if(graph.getMatrix()[u][v] == 0){
                graph.addEdge(Edge(graph.getNodes()[u], graph.getNodes()[v]));
            }
            graph.updateMatrix();
            pathNode[v] = true;
            if(v == t){
                break;
            }
            u = v;
        }
    }


    for(int i = 0; i < nCycles; i++){
        int cycleLength = QRandomGenerator::global()->bounded(3, n);
        std::vector<int> cycle;

        while((int)cycle.size() < cycleLength){
            int node = QRandomGenerator::global()->bounded(0, n);
            auto it = std::find(cycle.begin(), cycle.end(), node);
            if(it == cycle.end()){
                cycle.push_back(node);
            }
        }
        Node first, second;
        for(int j = 0; j < cycleLength - 1; j++){
            first = graph.getNodes()[cycle[j]];
            second = graph.getNodes()[cycle[j + 1]];
            if(graph.getMatrix()[cycle[j]][cycle[j+1]] == 0)
                graph.addEdge(Edge(first, second));
        }
        first = graph.getNodes()[cycle[cycleLength - 1]];
        second = graph.getNodes()[cycle[0]];
        if(graph.getMatrix()[cycle[cycleLength - 1]][cycle[0]] == 0)
            graph.addEdge(Edge(first, second));
        graph.updateMatrix();
        graph.updateList();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_radioButton_released()
{
    if(ui->radioButton->isChecked()){
        graph.isOriented = true;
    }
    else{
        graph.isOriented = false;
    }
    graph.updateMatrix();
    graph.updateList();
    update();
}

void MainWindow::on_pushButton_clicked()
{
    graph.clearNodes();
    graph.clearEdges();
    update();
    generateNodes();
}


void MainWindow::on_nNodes_valueChanged(int arg1)
{
    nNodes = arg1;
}


void MainWindow::on_nPaths_valueChanged(int arg1)
{
    nPaths = arg1;
}


void MainWindow::on_nCycles_valueChanged(int arg1)
{
    nCycles = arg1;
}

