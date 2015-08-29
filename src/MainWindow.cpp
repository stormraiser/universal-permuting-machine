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
#include "CommandSem.h"

#include <cstdio>

CommandSem *command;

void commandScanString(const char *str);
void commandDoneString();
int commandparse();

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent){
    cube = 0;
    board = new DisplayBoard(cubeLock);
    loadButton = new QPushButton("Load model");
    resetButton = new QPushButton("Reset");
    scrambleButton = new QPushButton("Scramble");
    scrambleSpin = new QSpinBox;
    scrambleLabel = new QLabel("Moves:");
    commandLabel = new QLabel("Command:");
    commandEdit = new QLineEdit();
    commandView = new QTextEdit();
    displayButton = new QPushButton("Change display");

    QHBoxLayout *loadLayout = new QHBoxLayout;
    loadLayout->addWidget(loadButton);
    loadLayout->addWidget(displayButton);
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
    mainLayout->addWidget(board);
    board->setMinimumSize(300, 300);
    board->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(widgetR);

    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    animationTime = 250;
    showAnimation = true;

    resize(900, 600);
    setWindowTitle("StormRaiser's Universal Permuting Machine");

    connect(loadButton, SIGNAL(clicked()), this, SLOT(loadFile()));
    connect(resetButton, SIGNAL(clicked()), this, SLOT(resetCube()));
    connect(scrambleButton, SIGNAL(clicked()), this, SLOT(scrambleCube()));
    connect(displayButton, SIGNAL(clicked()), board, SLOT(changeDisplayMode()));
    connect(commandEdit, SIGNAL(returnPressed()), this, SLOT(executeCommand()));
    connect(board, SIGNAL(clicked()), this, SLOT(operateCube()));
}

void MainWindow::loadFile(){
    bool f;
    QString str = QFileDialog::getOpenFileName(this, "Load cube model", QDir::current().absolutePath(), "Cube Descriptors (*.cub)");
    if (!str.isEmpty()){
        Translator *translator = new Translator();
        QFileInfo info = QFileInfo(str);
        Cube *_cube = translator->translate(string(info.absoluteDir().absolutePath().toLatin1().data()).append(1, QDir::separator().toLatin1()), string(info.fileName().toLatin1().data()));
        if (_cube != 0) {
            cube = _cube;
            board->setCube(cube);
            _cube->reset();
            commandView->setTextColor(QColor(0, 128, 0));
            commandView->insertPlainText(QString(translator->getMessage().c_str()));
        }
        else {
            commandView->setTextColor(QColor(224, 0, 0));
            commandView->insertPlainText(QString(translator->getMessage().c_str()));
        }
        delete translator;
    }
}

void MainWindow::resetCube(){
    if (cube != 0) {
        //cubeLock.lockForWrite();
        cube->reset();
        commandView->setTextColor(QColor(0, 0, 224));
        commandView->insertPlainText(QString("Reset\n"));
        //cubeLock.unlock();
        boardUpdate();
    }
}

void MainWindow::scrambleCube(){
    if (cube != 0) {
        //cubeLock.lockForWrite();
        commandView->setTextColor(QColor(0, 0, 224));
        commandView->insertPlainText(QString("Scramble %1 steps\n").arg(scrambleSpin->value()));
        cube->scramble(scrambleSpin->value());
        //cubeLock.unlock();
        boardUpdate();
    }
}

void MainWindow::executeCommand() {
    commandScanString(commandEdit->text().toLatin1().data());
    if (commandparse() != 0) {
        commandView->setTextColor(QColor(224, 0, 0));
        commandView->insertPlainText(QString("Syntax error\n"));
    }
    else {
        commandView->setTextColor(QColor(0, 0, 224));
        commandView->insertPlainText(QString("Command: %1\n").arg(commandEdit->text()));
        commandEdit->clear();
        for (string str : command->stringList) {
            switch (cube->setActiveBinding(str)) {
            case 0:
                operateCube();
                break;
            case -1:
                commandView->setTextColor(QColor(224, 0, 0));
                commandView->insertPlainText(QString("Unknown binding: %1\n").arg(QString(str.c_str())));
                break;
            case -2:
                commandView->setTextColor(QColor(224, 0, 0));
                commandView->insertPlainText(QString("Binding invalid: %1\n").arg(QString(str.c_str())));
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
    board->update();
}

void MainWindow::boardUpdateGL() {
    board->updateGL();
}
