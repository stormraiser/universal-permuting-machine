#pragma once

#include <QGLWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTime>
#include <QSize>
#include "Eigen/Eigen"
#include "Cube.h"
#include <QReadWriteLock>
#include <QMutex>

class DisplayBoard: public QGLWidget{

    Q_OBJECT

private:
    static const int dispSolo = 0;
    static const int dispCross = 1;
    static const int dispParallel = 2;
    static const int dispHolo = 3;
    static const int dispFrontBack = 4;
    static const int dispHoloCross = 5;
    //static const int dispFrontBackCross = 5,
    static const int totalMode = 6;

    int displayMode;

    Cube *cube;
    double minDim;
    double zoom;
    double trackBallRadius;
    int mouseX, mouseY;
    GLuint selectBuf[1000];
    bool needSelection;
    Eigen::Vector3d trackVec0, trackVec1;
    Eigen::Affine3d trackMat;
    int clickArea;
    QReadWriteLock &cubeLock;
    QMutex renderLock;

    void setView(int index);
    void setSelection();

public:
    DisplayBoard(QReadWriteLock &_cubeLock, QWidget *parent = 0);
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void setCube(Cube *p_cube);
    void setCrop(int x, int y, int width, int height, bool invertX = false, bool invertY = false);
    int viewCount();
    int getClickArea(int x, int y);
    Eigen::Vector3d getTrackballVector(int x, int y, int area);

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

signals:
    void clicked();

public slots:
    void changeDisplayMode();
};

