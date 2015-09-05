#include "DisplayWindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

DisplayWindow::DisplayWindow(int _windowId, bool _isPrimary, Eigen::Affine3d _primaryMatrix, QReadWriteLock &_cubeLock, QGLWidget *shareWidget, QWidget *parent): QWidget(parent) {
    windowId = _windowId;
    isPrimary = _isPrimary;

    board = new DisplayBoard(_isPrimary, _primaryMatrix, _cubeLock, shareWidget);
    modeLabel = new QLabel("Display mode");
    modeCombo = new QComboBox;
    for (int i = 0; i < DisplayBoard::totalMode; i++) {
        modeCombo->addItem(DisplayBoard::getModeName(i));
    }
    connect(modeCombo, SIGNAL(currentIndexChanged(int)), board, SLOT(setDisplayMode(int)));
    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->addWidget(modeLabel);
    controlLayout->addWidget(modeCombo);

    if (!isPrimary) {
        syncCheck = new QCheckBox("Syncronize with primary view");
        resetButton = new QPushButton("Reset view");

        syncCheck->setChecked(true);
        connect(syncCheck, SIGNAL(toggled(bool)), board, SLOT(setTrackPrimary(bool)));
        connect(resetButton, SIGNAL(clicked()), board, SLOT(resetView()));
        controlLayout->addWidget(syncCheck);
        controlLayout->addWidget(resetButton);
    }
    setWindowTitle(QString("Player"));

    controlLayout->addStretch(1);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(board);
    mainLayout->addLayout(controlLayout);
    setLayout(mainLayout);
    resize(400, 400);
}

DisplayBoard *DisplayWindow::getBoard() {
    return board;
}

void DisplayWindow::closeEvent(QCloseEvent *event) {
    emit closed(windowId);
}
