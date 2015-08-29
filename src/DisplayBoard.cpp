#include "DisplayBoard.h"
#include <GL/glu.h>
#include <QtDebug>
#include <cmath>
#include <cstdio>
#include <algorithm>
#include <QThread>

using namespace std;

DisplayBoard::DisplayBoard(QReadWriteLock &_cubeLock, QWidget *parent): QGLWidget(parent), cubeLock(_cubeLock){
    cube = 0;
    mouseX = 0;
    mouseY = 0;
    zoom = 0.25;
    trackBallRadius = 20;
    trackMat = Eigen::Affine3d::Identity() * Eigen::AngleAxisd(-2.617993878, Eigen::Vector3d(0, 0, 1)) * Eigen::AngleAxisd(-0.6154797087, Eigen::Vector3d(0, 1, 0)) * Eigen::AngleAxisd(0.7853981634, Eigen::Vector3d(1, 0, 0));
    setMouseTracking(true);
    displayMode = dispSolo;
    needSelection = true;
}

void DisplayBoard::setCube(Cube *p_cube){
    cube = p_cube;
    makeCurrent();
    cube->prepareDraw();
}

void DisplayBoard::initializeGL(){
    double w, h;
    float diffuse0[3] = {0.8f, 0.8f, 0.8f}, ambient0[3] = {0.2f, 0.2f, 0.2f}, specular0[3] = {0.9f, 0.9f, 0.9f}, pos0[4] = {300, 200, 500, 0};
    float diffuse1[3] = {0.2f, 0.2f, 0.2f}, ambient1[3] = {0.05f, 0.05f, 0.05f}, specular1[3] = {0.0f, 0.0f, 0.0f}, pos1[4] = {-300, -200, -500, 0};
    glEnable(GL_LIGHTING);
    glViewport(0, 0, width(), height());
    glEnable(GL_DEPTH_TEST);
    glSelectBuffer(1000, selectBuf);
    glCullFace(GL_BACK);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);
    glLightfv(GL_LIGHT0, GL_POSITION, pos0);
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
    glLightfv(GL_LIGHT1, GL_POSITION, pos1);
    //glClearColor(0.85, 0.85, 0.85, 1.0);
    glClearColor(0, 0, 0, 1.0);
}

void DisplayBoard::changeDisplayMode() {
    displayMode = (displayMode + 1) % totalMode;
    update();
}

int DisplayBoard::viewCount() {
    switch (displayMode) {
    case dispSolo:
        return 1;
    case dispCross:
    case dispParallel:
    case dispFrontBack:
        return 2;
    case dispHolo:
    //case dispFrontBackCross:
        return 4;
    case dispHoloCross:
        return 8;
    }
}

void DisplayBoard::resizeGL(int width, int height){
}

void DisplayBoard::setCrop(int x, int y, int width, int height, bool invertX, bool invertY) {
    double s = trackBallRadius / zoom;
    double w, h;
    if (width > height) {
        h = s;
        w = s / height * width;
        minDim = height;
    }
    else {
        w = s;
        h = s / width * height;
        minDim = width;
    }
    if (invertX) {
        w = -w;
    }
    if (invertY) {
        h = -h;
    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-w / 2, w / 2, -h / 2, h / 2, 200, 800);
    glViewport(x, y, width, height);
}

void DisplayBoard::setView(int index){
    switch (displayMode) {
    case dispSolo:
        setCrop(0, 0, width(), height());
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0, 0, 500, 0, 0, 0, 0, 1, 0);
        glMultMatrixd(trackMat.data());
        break;
    case dispCross:
        switch (index) {
        case 0:
            setCrop(0, 0, width() / 2, height());
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(10, 0, 500, 0, 0, 0, 0, 1, 0);
            glMultMatrixd(trackMat.data());
            break;
        case 1:
            setCrop(width() - width() / 2, 0, width() / 2, height());
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(-10, 0, 500, 0, 0, 0, 0, 1, 0);
            glMultMatrixd(trackMat.data());
            break;
        }
        break;
    case dispParallel:
        switch (index) {
        case 0:
            setCrop(0, 0, width() / 2, height());
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(-10, 0, 500, 0, 0, 0, 0, 1, 0);
            glMultMatrixd(trackMat.data());
            break;
        case 1:
            setCrop(width() - width() / 2, 0, width() / 2, height());
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(10, 0, 500, 0, 0, 0, 0, 1, 0);
            glMultMatrixd(trackMat.data());
            break;
        }
        break;
    case dispHolo:
        {
            int l = width() < height() ? width() : height();
            switch (index) {
            case 0:
                setCrop(width() / 2 - l / 6, height() / 2 - l / 2, l / 3, l / 3, false, true);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                gluLookAt(0, 0, 500, 0, 0, 0, 0, 1, 0);
                glMultMatrixd(trackMat.data());
                break;
            case 1:
                setCrop(width() / 2 + l / 6, height() / 2 - l / 6, l / 3, l / 3, true, false);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                gluLookAt(500, 0, 0, 0, 0, 0, 0, 0, -1);
                glMultMatrixd(trackMat.data());
                break;
            case 2:
                setCrop(width() / 2 - l / 6, height() / 2 + l / 6, l / 3, l / 3, false, true);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                gluLookAt(0, 0, -500, 0, 0, 0, 0, -1, 0);
                glMultMatrixd(trackMat.data());
                break;
            case 3:
                setCrop(width() / 2 - l / 2, height() / 2 - l / 6, l / 3, l / 3, true, false);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                gluLookAt(-500, 0, 0, 0, 0, 0, 0, 0, -1);
                glMultMatrixd(trackMat.data());
                break;
            }
        }
        break;
    case dispFrontBack:
        switch (index) {
        case 0:
            setCrop(0, 0, width() / 2, height());
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(0, 0, 500, 0, 0, 0, 0, 1, 0);
            glMultMatrixd(trackMat.data());
            break;
        case 1:
            setCrop(width() - width() / 2, 0, width() / 2, height());
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(0, 0, -500, 0, 0, 0, 0, 1, 0);
            glMultMatrixd(trackMat.data());
            break;
        }
        break;
    case dispHoloCross:
        {
            int l = width() < height() ? width() : height();
            switch (index) {
            case 0:
                setCrop(width() / 2 - l / 4, height() / 2 - l / 2, l / 4, l / 4, false, true);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                gluLookAt(10, 0, 500, 0, 0, 0, 0, 1, 0);
                glMultMatrixd(trackMat.data());
                break;
            case 1:
                setCrop(width() / 2, height() / 2 - l / 2, l / 4, l / 4, false, true);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                gluLookAt(-10, 0, 500, 0, 0, 0, 0, 1, 0);
                glMultMatrixd(trackMat.data());
                break;
            case 2:
                setCrop(width() / 2 + l / 4, height() / 2 - l / 4, l / 4, l / 4, true, false);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                gluLookAt(500, -10, 0, 0, 0, 0, 0, 0, -1);
                glMultMatrixd(trackMat.data());
                break;
            case 3:
                setCrop(width() / 2 + l / 4, height() / 2, l / 4, l / 4, true, false);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                gluLookAt(500, 10, 0, 0, 0, 0, 0, 0, -1);
                glMultMatrixd(trackMat.data());
                break;
            case 4:
                setCrop(width() / 2, height() / 2 + l / 4, l / 4, l / 4, false, true);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                gluLookAt(10, 0, -500, 0, 0, 0, 0, -1, 0);
                glMultMatrixd(trackMat.data());
                break;
            case 5:
                setCrop(width() / 2 - l / 4, height() / 2 + l / 4, l / 3, l / 4, false, true);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                gluLookAt(-10, 0, -500, 0, 0, 0, 0, -1, 0);
                glMultMatrixd(trackMat.data());
                break;
            case 6:
                setCrop(width() / 2 - l / 2, height() / 2, l / 4, l / 4, true, false);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                gluLookAt(-500, 10, 0, 0, 0, 0, 0, 0, -1);
                glMultMatrixd(trackMat.data());
                break;
            case 7:
                setCrop(width() / 2 - l / 2, height() / 2 - l / 4, l / 4, l / 4, true, false);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                gluLookAt(-500, -10, 0, 0, 0, 0, 0, 0, -1);
                glMultMatrixd(trackMat.data());
                break;
            }
        }
        break;
    }
}

void DisplayBoard::paintGL(){
    //renderLock.lock();
    GLuint color[4];
    if (cube != 0) {
        if (needSelection) {
            //glClearColor(0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glDisable(GL_LIGHTING);
            glShadeModel(GL_FLAT);
            for (int i = 0; i < viewCount(); i++) {
                setView(i);
                //cubeLock.lockForRead();
                cube->drawSelection();
                //cubeLock.unlock();
            }
            glReadPixels(mouseX, height() - mouseY - 1, 1, 1, GL_RGBA, GL_UNSIGNED_INT, color);
            //cubeLock.lockForWrite();
            cube->setSelection(color);
            //cubeLock.unlock();
            glEnable(GL_LIGHTING);
            glShadeModel(GL_SMOOTH);
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (int i = 0; i < viewCount(); i++) {
            setView(i);
            //cubeLock.lockForRead();
            cube->draw();
            //cubeLock.unlock();
        }
    }
    else {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    //renderLock.unlock();
}

int DisplayBoard::getClickArea(int x, int y) {
    switch (displayMode) {
    case dispSolo:
        return 0;
    case dispCross:
    case dispParallel:
    case dispFrontBack:
        return x < width() / 2 ? 0 : 1;
    case dispHolo:
        return x - width() / 2 + y - height() / 2 > 0 ?
            (x - width() / 2 < y - height() / 2 ? 0 : 1) :
            (x - width() / 2 > y - height() / 2 ? 2 : 3);
    case dispHoloCross:
        return x - width() / 2 + y - height() / 2 > 0 ?
            (x - width() / 2 < y - height() / 2 ? (x < width() / 2 ? 0 : 1) : (y > height() / 2 ? 2 : 3)) :
            (x - width() / 2 > y - height() / 2 ? (x > width() / 2 ? 4 : 5) : (y < height() / 2 ? 6 : 7));
    }
}

Eigen::Vector3d DisplayBoard::getTrackballVector(int x, int y, int area) {
    double cx, cy, r;
    switch (displayMode) {
    case dispSolo:
        cx = width() / 2;
        cy = height() / 2;
        r = min(width(), height()) * zoom;
        break;
    case dispCross:
    case dispParallel:
    case dispFrontBack:
        cy = height() / 2;
        r = min(width() / 2, height()) * zoom;
        switch (area) {
        case 0:
            cx = width() / 4;
            break;
        case 1:
            cx = width() * 3 / 4;
            break;
        }
        break;
    case dispHolo:
        {
            int l = (width() < height() ? width() : height()) / 3;
            r = l * zoom;
            switch (area) {
            case 0:
                cx = width() / 2;
                cy = height() / 2 + l;
                break;
            case 1:
                cx = width() / 2 + l;
                cy = height() / 2;
                r = l * zoom;
                break;
            case 2:
                cx = width() / 2;
                cy = height() / 2 - l;
                r = l * zoom;
                break;
            case 3:
                cx = width() / 2 - l;
                cy = height() / 2;
                r = l * zoom;
                break;
            }
        }
        break;
    case dispHoloCross:
        {
            int l = (width() < height() ? width() : height()) / 4;
            r = l * zoom;
            switch (area) {
            case 0:
                cx = width() / 2 - l / 2;
                cy = height() / 2 + l * 3 / 2;
                break;
            case 1:
                cx = width() / 2 + l / 2;
                cy = height() / 2 + l * 3 / 2;
                break;
            case 2:
                cx = width() / 2 + l * 3 / 2;
                cy = height() / 2 + l / 2;
                break;
            case 3:
                cx = width() / 2 + l * 3 / 2;
                cy = height() / 2 - l / 2;
                break;
            case 4:
                cx = width() / 2 + l / 2;
                cy = height() / 2 - l * 3 / 2;
                break;
            case 5:
                cx = width() / 2 - l / 2;
                cy = height() / 2 - l * 3 / 2;
                break;
            case 6:
                cx = width() / 2 - l * 3 / 2;
                cy = height() / 2 - l / 2;
                break;
            case 7:
                cx = width() / 2 - l * 3 / 2;
                cy = height() / 2 + l / 2;
                break;
            }
        }
        break;
    }

    double xt = (x - cx) / r;
    double yt = (cy - y) / r;
    double zt = xt * xt + yt * yt < 0.5 ? sqrt(1 - xt * xt - yt * yt) : 0.5 / sqrt(xt * xt + yt * yt);

    switch (displayMode) {
    case dispSolo:
    case dispCross:
    case dispParallel:
        return Eigen::Vector3d(xt, yt, zt).normalized();
    case dispFrontBack:
        switch (area) {
        case 0:
            return Eigen::Vector3d(xt, yt, zt).normalized();
        case 1:
            return Eigen::Vector3d(-xt, yt, -zt).normalized();
        }
    case dispHolo:
        switch (area) {
        case 0:
            return Eigen::Vector3d(xt, -yt, zt).normalized();
        case 1:
            return Eigen::Vector3d(zt, xt, -yt).normalized();
        case 2:
            return Eigen::Vector3d(xt, yt, -zt).normalized();
        case 3:
            return Eigen::Vector3d(-zt, -xt, -yt).normalized();
        }
    case dispHoloCross:
        switch (area) {
        case 0:
        case 1:
            return Eigen::Vector3d(xt, -yt, zt).normalized();
        case 2:
        case 3:
            return Eigen::Vector3d(zt, xt, -yt).normalized();
        case 4:
        case 5:
            return Eigen::Vector3d(xt, yt, -zt).normalized();
        case 6:
        case 7:
            return Eigen::Vector3d(-zt, -xt, -yt).normalized();
        }
    }
}

void DisplayBoard::mousePressEvent(QMouseEvent *event){
    if ((event->buttons() & Qt::LeftButton) != 0) {
        if (cube != 0) {
            emit clicked();
        }
    }
    if ((event->buttons() & Qt::RightButton) != 0) {
        clickArea = getClickArea(event->x(), event->y());
        trackVec0 = getTrackballVector(event->x(), event->y(), clickArea);
    }
}

void DisplayBoard::mouseMoveEvent(QMouseEvent *event){
    mouseX = event->x();
    mouseY = event->y();
    if ((event->buttons() & Qt::RightButton) != 0) {
        trackVec1 = getTrackballVector(event->x(), event->y(), clickArea);
        Eigen::Vector3d axis = trackVec0.cross(trackVec1).normalized();
        double angle = acos(trackVec0.dot(trackVec1));
        trackMat = Eigen::AngleAxisd(angle, axis) * trackMat;
        trackVec0 = trackVec1;
    }
    update();
}

void DisplayBoard::mouseReleaseEvent(QMouseEvent *event){
}

void DisplayBoard::wheelEvent(QWheelEvent *event) {
    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;
    if (!numDegrees.isNull()) {
        zoom *= pow(2, numDegrees.y() / 90.0);
    }
    else if (!numPixels.isNull()) {
        zoom *= pow(2, numPixels.y() / 100.0);
    }
    update();
    event->accept();
}

void DisplayBoard::keyPressEvent(QKeyEvent *event){
}
