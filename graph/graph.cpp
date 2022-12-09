#include "graph.h"
#include <fstream>
#include <QMainWindow>
#include "mainwindow.h"
#include <list>

Graph::Graph()
{
    matrix = new int*[(int)nodes.size()];
    for(int i = 0; i < (int)nodes.size(); i++){
        matrix[i] = new int[(int)nodes.size()];
    }
}

Graph::Graph(Graph &g)
{
    matrix = new int*[(int)nodes.size()];
    for (int i = 0; i < (int)nodes.size(); i++) {
        matrix[i] = new int[(int)nodes.size()];
    }

    for (int i = 0; i < (int)nodes.size(); i++) {
        for (int j = 0; j < (int)nodes.size(); j++) {
            matrix[i][j] = g.matrix[i][j];
        }
    }
}

Graph::~Graph()
{
    for (int i = 0; i < (int)nodes.size(); ++i) {
        delete matrix[i];
    }
    delete matrix;
}

void Graph::updateMatrix()
{
    matrix = new int*[(int)nodes.size()];
    for(int i = 0; i < (int)nodes.size(); i++){
        matrix[i] = new int[(int)nodes.size()];
        for(int j = 0; j < getNumberOfNodes(); j++){
            matrix[i][j] = 0;
        }
    }

    for(Edge& e: edges){
        matrix[e.getFirstNode().getInfo() - 1][e.getSecondNode().getInfo() - 1] = 1;
        if(!isOriented){
            matrix[e.getSecondNode().getInfo() - 1][e.getFirstNode().getInfo() - 1] = 1;
        }
    }
    uploadMatrix();
}

void Graph::uploadMatrix()
{
    ofstream output("matrix.txt");
    if(output.is_open()){
        output << (int)nodes.size() << std::endl;
        for (int i = 0; i < (int)nodes.size(); i++) {
            for (int j = 0; j < (int)nodes.size(); j++) {
                output << matrix[i][j] << " ";
            }
            output << std::endl;
        }
    }
    output.close();
}

void Graph::updateList()
{
    for(int i = 0; i < (int)nodes.size(); i++){
        list[i].clear();
    }

    for(int i = 0; i < (int)nodes.size(); i++){
        for(int j = 0; j  < (int)nodes.size(); j++){
            if(matrix[i][j] > 0)
                list[i].push_back(j + 1);
        }
    }
    uploadList();
}

void Graph::uploadList()
{
    ofstream out("list.txt");
    if(out.is_open()){
        out << (int)nodes.size() << std::endl;
        for(int i = 0; i < (int)nodes.size(); i++){
            out << i + 1 << ": ";
            for(int j = 0; j < (int)list[i].size(); j++){
                out << list[i][j] << " ";
            }
            out << std::endl;
        }
    }
    out.close();
}

void Graph::generateGraph(QMainWindow *q)
{
    const auto [n, m] = readLabyrinth("labyrinth.txt");
    int offset = 17;

    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            QPointF coord(j * (n + offset) + q->size().width() / 2 - 100, i * (n + offset) + q->size().height() / 2 - 100);
            int inf = i * n + j + 1;
            Node n(coord, inf);
            if(matrix[i][j] == 3){
                n.setEntrance(true);
            }
            if(matrix[i][j] == 2){
                n.setExit(true);
            }
            if(matrix[i][j] > 0){
                n.isWall = false;
            }
            nodes.push_back(n);
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if(matrix[i][j] > 0){
                Node currentNode = nodes[i * n + j];
                if(j < m - 1 && matrix[i][j + 1] > 0){
                    edges.push_back(Edge(currentNode, nodes[i * n + j + 1]));
                }
                if(j > 0 && matrix[i][j - 1] > 0){
                    edges.push_back(Edge(currentNode, nodes[i * n + j - 1]));
                }
                if(i > 0 && matrix[i - 1][j] > 0){
                    edges.push_back(Edge(currentNode, nodes[(i - 1) * n + j]));
                }
                if(i < n - 1 && matrix[i + 1][j] > 0){
                    edges.push_back(Edge(currentNode, nodes[(i + 1) * n + j]));
                }

            }
        }
    }
    updateMatrix();
    updateList();

    BFS();
}

void Graph::BFS()
{
    int start;

    for(auto& n: nodes){
        if(n.getEntrance()){
            start = n.getInfo();
            break;
        }
    }

    vector<bool> visited(nodes.size(), false);
    vector<int> parent(nodes.size(), 0);
    std::list<int> queue;
    vector<int> result;

    visited[start] = true;
    queue.push_back(start);

    while(!queue.empty()){
        start = queue.front();
        result.push_back(start);
        queue.pop_front();

        for(auto adj: list[start - 1]){
            if(!visited[adj]){
                visited[adj] = true;
                queue.push_back(adj);
                parent[adj - 1] = start - 1;
            }
        }
    }
    for(auto a: result){
        if(nodes[a - 1].getExit()){
            while(!nodes[parent[a - 1]].getExit() && !nodes[parent[a-1]].getEntrance()){
                nodes[parent[a - 1]].isPath = true;
                parent[a - 1] = parent[parent[a - 1]];
            }
        }
    }
}

std::vector<int> Graph::DFS(Node start)
{
    std::stack<Node> st;

    st.push(start);
    
    Node node;
    std::vector<int> dfs;
    std::vector<bool> discovered(nodes.size(), false);

    while (!st.empty()) {
        node = st.top();
        if (!discovered[node.getInfo() - 1]) {
            dfs.push_back(node.getInfo());
        }
        else {
            cycle = true;
        }
        st.pop();

        if (!discovered[node.getInfo() - 1]) {
            discovered[node.getInfo() - 1] = true;
            for (auto& adj : list[node.getInfo() - 1]) {
                st.push(nodes[adj - 1]);
            }
        }
    }
    return dfs;
}

void printToFile(std::string fileName, std::vector<int> v){
    std::ofstream f_out(fileName);
    if(f_out.is_open()){
        for(int i: v){
            f_out << i << ' ';
        }
    }
    f_out.close();
}

std::vector<int> Graph::DFS(std::vector<Node> start)
{
    std::stack<Node> st;
    for (Node n : start){
        st.push(n);
    }
    Node node;
    std::vector<int> dfs;
    std::vector<bool> discovered(nodes.size(), false);

    while(!st.empty()){
        node = st.top();
		if (!discovered[node.getInfo() - 1]) {
            dfs.push_back(node.getInfo());
		}
        else {
            cycle = true;
        }
        st.pop();

        if(!discovered[node.getInfo() - 1]){
            discovered[node.getInfo() - 1] = true;
            for(auto& adj: list[node.getInfo() - 1]){
                st.push(nodes[adj - 1]);
            }
        }
    }
    return dfs;
}

std::vector<Node> Graph::findRoots()
{
	if (hasCycle()) {
		qDebug() << "Graph has cycle";
		return {};
	}
    
    std::vector<bool> hasIncoming(nodes.size(), false);
    
    for (Node& n : nodes) {
		std::vector<int> dfs = DFS(n);

        if (dfs.size() == nodes.size()) {
            root = dfs[0];
        }
        
        for (int i = 1; i < dfs.size(); i++) {
            hasIncoming[dfs[i] - 1] = true;
        }
    }
        
	std::vector<Node> roots;
    
    for (int i = 0; i < hasIncoming.size(); i++) {
        if (!hasIncoming[i]) {
            roots.push_back(nodes[i]);
        }     
    }
    
    return roots;
}

pair<int, int> Graph::readLabyrinth(string fileName)
{
    std::ifstream input(fileName);
    int matrixHeight, matrixWidth;
    
    input >> matrixHeight >> matrixWidth;

    matrix = new int*[matrixHeight];
    for(int i = 0; i < matrixHeight; i++){
        matrix[i] = new int[matrixWidth];
        for(int j = 0; j < matrixWidth; j++){
            input >> matrix[i][j];
        }
    }

    input.close();

    pair<int, int> matrixDims(matrixHeight, matrixWidth);

    return matrixDims;
}





