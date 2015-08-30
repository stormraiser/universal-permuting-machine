#pragma once

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include "DisplayBoard.h"

class DisplayWindow: public QWidget {

    Q_OBJECT

private:
    QLabel *modeLabel;
    QComboBox *modeCombo;
    QCheckBox *syncCheck;
    QPushButton *resetButton;
    DisplayBoard *board;
    bool isPrimary;
    int windowId;

protected:
    void closeEvent(QCloseEvent *event);

public:
    DisplayWindow(int _windowId, bool _isPrimary, Eigen::Affine3d _primaryMatrix, QReadWriteLock &_cubeLock, QGLWidget *shareWidget = 0, QWidget *parent = 0);
    DisplayBoard *getBoard();

signals:
    void closed(int _windowId);
};
