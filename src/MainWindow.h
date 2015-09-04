#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QSpinBox>
#include <QLabel>
#include <QMutex>
#include <QReadWriteLock>
#include <map>
#include <Eigen/Eigen>
#include "Cube.h"
#include "DisplayWindow.h"
#include "CubeDescriptionHighlighter.h"

using namespace std;

class MainWindow: public QMainWindow{

    Q_OBJECT

private:
    Cube *cube;
    QPushButton *loadButton, *resetButton, *scrambleButton, *displayButton;
    QPushButton *newViewButton;
    QSpinBox *scrambleSpin;
    QLabel *scrambleLabel, *commandLabel;
    QLineEdit *commandEdit;
    QTextEdit *commandView;
    QReadWriteLock cubeLock;
    QMutex operationLock;
    int animationTime;
    int viewWindowCount;
    bool showAnimation;
    DisplayWindow *primaryView;
    map<int, DisplayWindow*> viewWindows;
    QTextEdit *codeEdit;
    CubeDescriptionHighlighter *highlighter;

    void boardUpdate();
    void boardUpdateGL();
    void commandScrambleCube();
    void commandResetCube();
    void setCube(Cube *_cube);

public:
    MainWindow(QWidget *parent = 0);

signals:
    void drawSignal(bool _needSelection);

private slots:
    void loadFile();
    void resetCube();
    void scrambleCube();
    void executeCommand();
    void operateCube();
    void removeViewWindow(int windowId);
    void createViewWindow();
    void boardRotate(double angle, Eigen::Vector3d axis);
};
