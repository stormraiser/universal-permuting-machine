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
    Eigen::Affine3d primaryMatrix, localMatrix, rotateMatrix;
    int clickArea;
    QReadWriteLock &cubeLock;
    QMutex renderLock;
    bool trackPrimary;
    bool isPrimary;
    bool mouseOver;

    void setView(int index);
    void setSelection();
    void flipMousePosition();

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);

public:
    DisplayBoard(bool _isPrimary, Eigen::Affine3d _primaryMatrix, QReadWriteLock &_cubeLock, QGLWidget *shareWidget = 0, QWidget *parent = 0);
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void setCube(Cube *p_cube);
    void setCrop(int x, int y, int width, int height, bool invertX = false, bool invertY = false);
    void primaryRotate(const Eigen::Affine3d &mat, const Eigen::Affine3d &inv);
    int viewCount();
    int getClickArea(int x, int y);
    Eigen::Vector3d getTrackballVector(int x, int y, int area);
    Eigen::Affine3d getLocalMatrix();

    static const int totalMode = 6;
    static QString getModeName(int mode);

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

signals:
    void clicked();
    void rotated(double angle, Eigen::Vector3d axis);

public slots:
    void changeDisplayMode();
    void setDisplayMode(int _displayMode);
    void setTrackPrimary(bool _trackTrimary);
    void resetView();
};

