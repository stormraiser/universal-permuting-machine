#include "MainWindow.h"
#include "Translator.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QSizePolicy>
#include <QString>
#include <QStringList>
#include <QColor>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QScrollBar>
#include "CommandSem.h"

#include <cstdio>

void commandScanString(const char *str);
void commandDoneString();
int commandparse(CommandSem **command);

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent){
    cube = 0;
    primaryView = new DisplayWindow(-1, true,
                             Eigen::Affine3d::Identity() * Eigen::AngleAxisd(-2.617993878, Eigen::Vector3d(0, 0, 1)) * Eigen::AngleAxisd(-0.6154797087, Eigen::Vector3d(0, 1, 0)) * Eigen::AngleAxisd(0.7853981634, Eigen::Vector3d(1, 0, 0)),
                             cubeLock);
    loadButton = new QPushButton("Load model");
    resetButton = new QPushButton("Reset");
    scrambleButton = new QPushButton("Scramble");
    scrambleSpin = new QSpinBox;
    scrambleLabel = new QLabel("Moves:");
    commandLabel = new QLabel("Command:");
    commandEdit = new QLineEdit();
    commandView = new QTextEdit();
    newViewButton = new QPushButton("Create new window");

    codeEdit = new QTextEdit;
    codeEdit->setFontFamily("monospace");
    highlighter = new CubeDescriptionHighlighter(codeEdit->document());

    QTabWidget *mainTab = new QTabWidget;
    mainTab->addTab(primaryView, "Player");
    mainTab->addTab(codeEdit, "Code");

    QHBoxLayout *loadLayout = new QHBoxLayout;
    loadLayout->addWidget(loadButton);
    loadLayout->addWidget(newViewButton);
    QHBoxLayout *scrambleLayout = new QHBoxLayout;
    scrambleLayout->addWidget(scrambleLabel);
    scrambleLayout->addWidget(scrambleSpin);
    scrambleLayout->addWidget(scrambleButton);
    scrambleLayout->addWidget(resetButton);
    scrambleLayout->setStretch(2, 10);
    scrambleLayout->setStretch(3, 10);
    QHBoxLayout *commandLayout = new QHBoxLayout;
    commandLayout->addWidget(commandLabel);
    commandLayout->addWidget(commandEdit);
    commandLayout->setStretch(1, 10);
    QVBoxLayout *layoutR = new QVBoxLayout;
    layoutR->addLayout(loadLayout);
    layoutR->addLayout(scrambleLayout);
    layoutR->addLayout(commandLayout);
    layoutR->addWidget(commandView);
    layoutR->setStretch(2, 10);
    QWidget *widgetR = new QWidget;
    widgetR->setLayout(layoutR);
    widgetR->setMaximumWidth(350);
    widgetR->setMinimumWidth(250);

    commandView->setReadOnly(true);
    scrambleSpin->setRange(0, 100);
    scrambleSpin->setValue(20);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(mainTab);
    mainTab->setMinimumSize(300, 300);
    mainTab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(widgetR);

    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    animationTime = 250;
    showAnimation = true;
    viewWindowCount = 0;

    resize(900, 600);
    setWindowTitle("StormRaiser's Universal Permuting Machine");

    connect(loadButton, SIGNAL(clicked()), this, SLOT(loadFile()));
    connect(resetButton, SIGNAL(clicked()), this, SLOT(resetCube()));
    connect(scrambleButton, SIGNAL(clicked()), this, SLOT(scrambleCube()));
    connect(commandEdit, SIGNAL(returnPressed()), this, SLOT(executeCommand()));
    connect(primaryView->getBoard(), SIGNAL(clicked()), this, SLOT(operateCube()));
    connect(primaryView->getBoard(), SIGNAL(rotated(double, Eigen::Vector3d)), this, SLOT(boardRotate(double, Eigen::Vector3d)));
    connect(newViewButton, SIGNAL(clicked()), this, SLOT(createViewWindow()));
}

void MainWindow::loadFile(){
    bool f;
    QString str = QFileDialog::getOpenFileName(this, "Load cube model", QDir::current().absolutePath(), "Cube Descriptors (*.cub)");
    if (!str.isEmpty()){
        Translator *translator = new Translator();
        QFileInfo info = QFileInfo(str);
        Cube *_cube = translator->translate(string(info.absoluteDir().absolutePath().toLatin1().data()).append(1, QDir::separator().toLatin1()), string(info.fileName().toLatin1().data()));
        if (_cube != 0) {
            setCube(_cube);
            commandView->setTextColor(QColor(0, 128, 0));
            commandView->append(QString(translator->getMessage().c_str()));
            commandView->verticalScrollBar()->setValue(commandView->verticalScrollBar()->maximum());
            codeEdit->setWindowTitle(info.fileName());
            QFile file(str);
            file.open(QFile::ReadOnly);
            QTextStream stream(&file);
            codeEdit->setText(stream.readAll());
        }
        else {
            commandView->setTextColor(QColor(224, 0, 0));
            commandView->append(QString(translator->getMessage().c_str()));
            commandView->verticalScrollBar()->setValue(commandView->verticalScrollBar()->maximum());
        }
        delete translator;
    }
}

void MainWindow::resetCube(){
    if (cube != 0) {
        commandView->setTextColor(QColor(0, 0, 224));
        commandView->append(QString("Reset"));
        commandView->verticalScrollBar()->setValue(commandView->verticalScrollBar()->maximum());
        cubeLock.lockForWrite();
        cube->reset();
        cubeLock.unlock();
        boardUpdate();
    }
}

void MainWindow::commandResetCube(){
    if (cube != 0) {
        cubeLock.lockForWrite();
        cube->reset();
        cubeLock.unlock();
        boardUpdate();
    }
}

void MainWindow::scrambleCube(){
    if (cube != 0) {
        commandView->setTextColor(QColor(0, 0, 224));
        commandView->append(QString("Scramble %1 steps").arg(scrambleSpin->value()));
        commandView->verticalScrollBar()->setValue(commandView->verticalScrollBar()->maximum());
        cubeLock.lockForWrite();
        cube->scramble(scrambleSpin->value());
        cubeLock.unlock();
        boardUpdate();
    }
}

void MainWindow::commandScrambleCube(){
    if (cube != 0) {
        cubeLock.lockForWrite();
        cube->scramble(1);
        cubeLock.unlock();
        boardUpdate();
    }
}

void MainWindow::executeCommand() {
    commandScanString(commandEdit->text().toLatin1().data());
    CommandSem *command = 0;
    if (commandparse(&command) != 0) {
        commandView->setTextColor(QColor(224, 0, 0));
        commandView->append(QString("Syntax error"));
        commandView->verticalScrollBar()->setValue(commandView->verticalScrollBar()->maximum());
    }
    else {
        commandView->setTextColor(QColor(0, 0, 224));
        commandView->append(QString("Command: %1").arg(commandEdit->text()));
        commandView->verticalScrollBar()->setValue(commandView->verticalScrollBar()->maximum());
        commandEdit->clear();
        for (string str : command->stringList) {
            if (str == string("scramble")) {
                commandScrambleCube();
            }
            else if (str == string("reset")) {
                commandResetCube();
            }
            else switch (cube->setActiveBinding(str)) {
            case 0:
                operateCube();
                break;
            case -1:
                commandView->setTextColor(QColor(224, 0, 0));
                commandView->append(QString("Unknown binding: %1").arg(QString(str.c_str())));
                commandView->verticalScrollBar()->setValue(commandView->verticalScrollBar()->maximum());
                break;
            case -2:
                commandView->setTextColor(QColor(224, 0, 0));
                commandView->append(QString("Binding invalid: %1").arg(QString(str.c_str())));
                commandView->verticalScrollBar()->setValue(commandView->verticalScrollBar()->maximum());
                break;
            }
        }
    }
    commandDoneString();
    CommandSem::clear();
}

void MainWindow::operateCube() {
    operationLock.lock();
    if (showAnimation) {
        QTime time;
        time.start();
        //cubeLock.lockForWrite();
        int n = cube->prepareAnimation();
        //cubeLock.unlock();
        boardUpdateGL();
        int k = time.elapsed();
        while (k < n * animationTime) {
            //cubeLock.lockForWrite();
            cube->doAnimation(1.0 * k / animationTime);
            //cubeLock.unlock();
            boardUpdateGL();
            k = time.elapsed();
        }
        //cubeLock.lockForWrite();
        cube->doneAnimation();
        //cubeLock.unlock();
        boardUpdate();
    }
    else {
        //cubeLock.lockForWrite();
        cube->operate();
        //cubeLock.unlock();
        boardUpdate();
    }
    operationLock.unlock();
}

void MainWindow::boardUpdate() {
    primaryView->getBoard()->update();
    for (auto p : viewWindows) {
        p.second->getBoard()->update();
    }
}

void MainWindow::boardUpdateGL() {
    primaryView->getBoard()->updateGL();
    for (auto p : viewWindows) {
        p.second->getBoard()->updateGL();
    }
}

void MainWindow::removeViewWindow(int windowId) {
    viewWindows.erase(windowId);
}

void MainWindow::createViewWindow() {
    DisplayWindow *window = new DisplayWindow(viewWindowCount, false, primaryView->getBoard()->getLocalMatrix(), cubeLock, primaryView->getBoard());
    viewWindows.insert(pair<int, DisplayWindow*>(viewWindowCount, window));
    viewWindowCount++;
    window->getBoard()->setCube(cube);
    connect(window, SIGNAL(closed(int)), this, SLOT(removeViewWindow(int)));
    connect(window->getBoard(), SIGNAL(clicked()), this, SLOT(operateCube()));
    window->show();
}

void MainWindow::boardRotate(double angle, Eigen::Vector3d axis) {
    Eigen::Affine3d mat = Eigen::Affine3d::Identity() * Eigen::AngleAxisd(angle, axis);
    Eigen::Affine3d inv = Eigen::Affine3d::Identity() * Eigen::AngleAxisd(-angle, axis);
    for (auto p : viewWindows) {
        p.second->getBoard()->primaryRotate(mat, inv);
    }
}

void MainWindow::setCube(Cube *_cube) {
    cube = _cube;
    primaryView->getBoard()->setCube(cube);
    cube->prepareDraw();
    for (auto p : viewWindows) {
        p.second->getBoard()->setCube(cube);
    }
    _cube->reset();
    boardUpdate();
}
