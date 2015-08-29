#pragma once

#include "Eigen/Eigen"
#include <cmath>
#include "CubeSem.h"

#define PI 3.14159265358979323846264

struct TransformationPrimitive {
    enum Type {
        traEulerianRotation,
        traAxisAngleRotation,
        traTranslation
    };

    Type type;
    Eigen::Vector3d v0, v1;
    double angle, ax, ay, az;

    TransformationPrimitive(){}
    TransformationPrimitive(CubeSem *node) {
        switch (node->type) {
        case CubeSem::semRotateStmt:
            setEulerianRotation(node->numberList[0], node->numberList[1], node->numberList[2]);
            break;
        case CubeSem::semAxisAngleStmt:
            if (node->numberList.size() == 4) {
                setAxisAngleRotation(0, 0, 0, node->numberList[0], node->numberList[1], node->numberList[2], node->numberList[3]);
            } else {
                setAxisAngleRotation(node->numberList[0], node->numberList[1], node->numberList[2], node->numberList[3], node->numberList[4], node->numberList[5], node->numberList[6]);
            }
            break;
        case CubeSem::semTranslateStmt:
            setTranslation(node->numberList[0], node->numberList[1], node->numberList[2]);
            break;
        default:
            break;
        }
    }

    void setEulerianRotation(double _ax, double _ay, double _az) {
        type = traEulerianRotation;
        ax = _ax / 180 * PI;
        ay = _ay / 180 * PI;
        az = _az / 180 * PI;
    }

    void setAxisAngleRotation(double x0, double y0, double z0, double x1, double y1, double z1, double _angle) {
        type = traAxisAngleRotation;
        v0 = Eigen::Vector3d(x0, y0, z0);
        v1 = Eigen::Vector3d(x1 - x0, y1 - y0, z1 - z0);
        v1.normalize();
        angle = _angle / 180 * PI;
    }

    void setTranslation(double x, double y, double z) {
        type = traTranslation;
        v1 = Eigen::Vector3d(x, y, z);
    }

    TransformationPrimitive inversed() {
        TransformationPrimitive ret;
        ret.type = type;
        switch (type) {
        case traAxisAngleRotation:
            ret.v0 = v0;
            ret.v1 = v1;
            ret.angle = -angle;
            break;
        case traTranslation:
            ret.v1 = -v1;
            break;
        }
        return ret;
    }

    Eigen::Affine3d toTransformation(double t = 1) {
        switch (type) {
        case traEulerianRotation:
            return Eigen::Affine3d::Identity()
                   * Eigen::AngleAxisd(az * t, Eigen::Vector3d(0, 0, 1))
                   * Eigen::AngleAxisd(ay * t, Eigen::Vector3d(0, 1, 0))
                   * Eigen::AngleAxisd(ax * t, Eigen::Vector3d(1, 0, 0));
        case traAxisAngleRotation:
            return Eigen::Affine3d::Identity() * Eigen::Translation<double, 3>(v0) * Eigen::AngleAxisd(angle * t, v1) * Eigen::Translation<double, 3>(-v0);
        case traTranslation:
            return Eigen::Affine3d::Identity() * Eigen::Translation<double, 3>(v1 * t);
        }
    }

    void transform(const Eigen::Affine3d &t) {
        v0 = t * v0;
        v1 = t.linear() * v1;
    }

    bool operator==(const TransformationPrimitive &other) const {
        if (type != other.type) {
            return false;
        }
        switch (type) {
        case traAxisAngleRotation:
            return ((v0 - other.v0).squaredNorm() < 1e-6) && ((v1 - other.v1).squaredNorm() < 1e-6) && (fabs(angle - other.angle) < 1e-3);
        case traTranslation:
            return (v0 - other.v0).squaredNorm() < 1e-6;
        default:
            return false;
        }
    }

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};
