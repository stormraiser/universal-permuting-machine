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
#include "CodeEdit.h"

using namespace std;

class MainWindow: public QMainWindow{

    Q_OBJECT

private:
    Cube *cube;
    QPushButton *loadAndPlayButton, *resetButton, *scrambleButton, *displayButton;
    QPushButton *newButton, *loadButton, *saveButton, *playButton;
    QPushButton *newViewButton;
    QSpinBox *scrambleSpin;
    QLabel *scrambleLabel, *commandLabel;
    QTabWidget *mainTab;
    QLineEdit *commandEdit;
    QTextEdit *commandView;
    QReadWriteLock cubeLock;
    QMutex operationLock;
    int animationTime;
    int viewWindowCount;
    int codeTabCount;
    int newCodeCount;
    bool showAnimation;
    DisplayWindow *primaryView;
    map<int, DisplayWindow*> viewWindows;
    map<int, CodeEdit*> codeTabs;

    void boardUpdate();
    void boardRepaint();
    void commandScrambleCube();
    void commandResetCube();
    void setCube(Cube *_cube);
    void loadAndPlay(QString filename);

protected:
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);

public:
    MainWindow(QWidget *parent = 0);

signals:
    void drawSignal(bool _needSelection);

private slots:
    void load();
    void save();
    void newCodeTab();
    void play();
    void loadAndPlay();
    void resetCube();
    void scrambleCube();
    void executeCommand();
    void operateCube();
    void removeViewWindow(int windowId);
    void createViewWindow();
    void boardRotate(double angle, Eigen::Vector3d axis);
    void closeTab(int index);
    void changeCodeTabTitle(int id, QString title);
};
