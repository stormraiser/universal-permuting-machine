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
#include "Cube.h"
#include "DisplayBoard.h"

class MainWindow: public QMainWindow{

    Q_OBJECT

private:
    Cube *cube;
    DisplayBoard *board;
    QPushButton *loadButton, *resetButton, *scrambleButton, *displayButton;
    QSpinBox *scrambleSpin;
    QLabel *scrambleLabel, *commandLabel;
    QLineEdit *commandEdit;
    QTextEdit *commandView;
    QReadWriteLock cubeLock;
    QMutex operationLock;
    int animationTime;
    bool showAnimation;

    void boardUpdate();
    void boardUpdateGL();

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
};
