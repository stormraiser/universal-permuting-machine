#pragma once

#include <map>
#include <string>
#include <sstream>
#include "Eigen/Eigen"
#include "Eigen/StdVector"
#include <GL/gl.h>
#include <cstdio>

#include "TransformationPrimitive.h"
#include "CubeSem.h"
#include "MeshObject.h"

using namespace std;

struct TranslatorTag {
    int id;
    string name;
    vector<float> color;
    bool isPrimary;
    bool isColored;
    Location firstLocation;

    string toString();
};

struct TranslatorSymmetry {
    string name;
    vector<int> tagPermutation;
    Eigen::Affine3d transformation;
    bool hasTransformation;

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    string toString();
};

struct TranslatorGeometry {
    string name;
    vector<MeshObject*> meshList;
    vector< Eigen::Affine3d, Eigen::aligned_allocator<Eigen::Affine3d> > transformationList;

    string toString();
    void setColor(const vector<float> &color, bool isSelected = false);
    void draw(const vector< vector<float> > &colors, bool isSelected = false);
};

struct TranslatorBlock {
    int id;
    int aliasedId;
    string name;
    TranslatorGeometry *geometry;
    vector<int> tagList;
    Eigen::Affine3d transformation;
    int alias;
    int bandageGroup;
    int equivalenceGroup;

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    string toString();
};

struct TranslatorPosition {
    int id;
    int aliasedId;
    string name;
    int alias;
    int equivalenceGroup;

    string toString();
};

struct TranslatorOperation {
    int id;
    string name;
    string inverse;
    vector< vector<TransformationPrimitive> > transformationList;
    vector< Eigen::Affine3d, Eigen::aligned_allocator<Eigen::Affine3d> > activeTransformationList;
    vector<int> positionPermutation, inversePermutation;
    vector<int> transformationId;

    string toString();
    void generateTransformation(double t = 1);
};

struct TranslatorBinding {
    int id;
    string name;
    vector< vector< vector<int> > > invokeList;
    vector< vector<int> > positionPermutations;
    vector< vector<bool> > positionFlags;
    Eigen::Affine3d transformation;
    MeshObject *selector;
    int validIndex;

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    string toString();
    void draw();
};

struct CubeBlock {
    int startPosition, currentPosition;
    Eigen::Affine3d startTransformation, currentTransformation, drawTransformation;
    TranslatorGeometry *geometry;
    vector< vector<float> > colorList;
    bool isSelected;

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    void partialTransform(const Eigen::Affine3d &transformation);
    void transform(const Eigen::Affine3d &transformation);
    void reset();
    void draw();
};
