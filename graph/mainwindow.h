#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graph.h"
#include <QPushButton>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void mouseReleaseEvent(QMouseEvent* ev);
    void mouseMoveEvent(QMouseEvent * ev);
    void paintEvent(QPaintEvent* p);

    void generateNodes();
    void buildPathsAndCycles(int n, int nPaths, int nCycles);

    int getNodeRadius(){return nodeRadius;}

    ~MainWindow();
private slots:
    void on_radioButton_released();

    void on_pushButton_clicked();

    void on_nNodes_valueChanged(int arg1);

    void on_nPaths_valueChanged(int arg1);

    void on_nCycles_valueChanged(int arg1);

private:
    Graph graph;
    Ui::MainWindow *ui;

    const int nodeRadius = 10;
    bool isFirstNode;
    Node firstNode;

    int nNodes = 0;
    int nPaths = 0;
    int nCycles = 0;
};
#endif // MAINWINDOW_H
