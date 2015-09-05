#include "Objects.h"

string TranslatorTag::toString() {
    stringstream s;
    s << "Tag #" << id << ": " << name;
    if (isPrimary) {
        s << ", primary tag";
    }
    if (isColored) {
        s << ", color = (" << color[0] << ", " << color[1] << ", " << color[2] << ")";
    }
    s << endl;
    return s.str();
}

string TranslatorSymmetry::toString() {
    stringstream s;
    s << "Symmetry " << name << endl;
    if (hasTransformation) {
        s << "transformation:" << endl;
        s << transformation.matrix() << endl;
    }
    else {
        s << "no transformation" << endl;
    }
    bool f = false;
    for (int i = 0; i < tagPermutation.size(); i++) {
        if ((tagPermutation[i] != -1) && (tagPermutation[i] != i)) {
            if (!f) {
                f = true;
                s << "Tag permutation:" << endl;
            }
            else {
                s << ", ";
            }
            s << i << " -> " << tagPermutation[i];
        }
    }
    if (f) {
        s << endl;
    }
    return s.str();
}

string TranslatorGeometry::toString() {
    stringstream s;
    s << "Geometry " << name << endl;
    for (int i = 0; i < meshList.size(); i++) {
        s << "mesh object \"" << meshList[i]->name << "\", transformation:" << endl;
        s << transformationList[i].matrix() << endl;
    }
    return s.str();
}

void TranslatorGeometry::setColor(const vector<float> &color, bool isSelected) {
    float c[3], ct[4];
    if (isSelected) {
        for (int i = 0; i < 3; i++) {
            c[i] = color[i] * 0.7 + 0.3;
        }
    }
    else {
        for (int i = 0; i < 3; i++) {
            c[i] = color[i];
        }
    }
    ct[3] = 1.0f;
    for (int i = 0; i < 3; i++)
        ct[i] = c[i] * 0.9 + 0.1;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ct);
    for (int i = 0; i < 3; i++)
        ct[i] = c[i] * 0.3 + 0.7;
    glMaterialfv(GL_FRONT, GL_SPECULAR, ct);
    for (int i = 0; i < 3; i++)
        ct[i] = c[i] * 0.7 + 0.3;
    glMaterialfv(GL_FRONT, GL_AMBIENT, ct);
    glMaterialf(GL_FRONT, GL_SHININESS, 100);
}

void TranslatorGeometry::draw(const vector< vector<float> > &colors, bool isSelected) {
    for (int i = 0; i < meshList.size(); i++) {
        setColor(colors[i], isSelected);
        glPushMatrix();
        glMultMatrixd(transformationList[i].data());
        meshList[i]->draw();
        glPopMatrix();
    }
}

string TranslatorBlock::toString() {
    stringstream s;
    s << "Block #" << id << ": " << name;
    if (alias != id) {
        s << ", aliased to block #" << alias << endl;
    }
    else if (geometry != 0) {
        s << " = " << geometry->name << "(";
        for (int i = 0; i < tagList.size(); i++) {
            if (i != 0) {
                s << ", ";
            }
            s << tagList[i];
        }
        s << ")" << endl;
        s << "transformation:" << endl << transformation.matrix() << endl;
    }
    else {
        s << endl;
    }
    return s.str();
}

string TranslatorPosition::toString() {
    stringstream s;
    s << "Position #" << id << ": " << name;
    if (alias != id) {
        s << ", aliased to position #" << alias;
    }
    s << endl;
    return s.str();
}

string TranslatorOperation::toString() {
    stringstream s;
    s << "Operation " << name;
    if (!inverse.empty()) {
        s << ", inverses operation " << inverse << endl;
    }
    else {
        s << endl;
        bool f = false;
        for (int i = 0; i < positionPermutation.size(); i++) {
            if ((positionPermutation[i] >= 0) && (positionPermutation[i] != i)) {
                if (!f) {
                    f = true;
                    s << "Position permutation:" << endl;
                }
                else {
                    s << ", ";
                }
                s << i << " -> " << positionPermutation[i] << " (" << transformationId[i] << ") ";
            }
            else if (positionPermutation[i] == -2) {
                if (!f) {
                    f = true;
                    s << "Position permutation:" << endl;
                }
                else {
                    s << ", ";
                }
                s << i << ": forbidden";
            }
        }
        if (f) {
            s << endl;
        }
    }
    return s.str();
}

string TranslatorBinding::toString() {
    stringstream s;
    s << "Binding " << name << endl;
    if (selector != 0) {
        s << "Selector: " << selector->name << ", transformation:" << endl;
        s << transformation.matrix() << endl;
    }
    for (int i = 0; i < invokeList.size(); i++) {
        s << "operation sequence:";
        for (vector<int> vec : invokeList[i]) {
            for (int k : vec) {
                s << ' ' << k;
            }
        }
        s << endl;
        bool f = false;
        for (int j = 0; j < positionPermutations[i].size(); j++) {
            if ((positionPermutations[i][j] >= 0) && (positionPermutations[i][j] != j)) {
                if (!f) {
                    f = true;
                    s << "Position permutation:" << endl;
                }
                else {
                    s << ", ";
                }
                s << j << " -> " << positionPermutations[i][j];
            }
            else if (positionPermutations[i][j] == -2) {
                if (!f) {
                    f = true;
                    s << "Position permutation:" << endl;
                }
                else {
                    s << ", ";
                }
                s << j << ": forbidden";
            }
        }
        if (f) {
            s << endl;
        }
    }
    return s.str();
}

void TranslatorOperation::generateTransformation(double t) {
    for (int i = 0; i < transformationList.size(); i++) {
        activeTransformationList[i] = Eigen::Affine3d::Identity();
        for (int j = 0; j < transformationList[i].size(); j++) {
            activeTransformationList[i] = activeTransformationList[i] * transformationList[i][j].toTransformation(t);
        }
    }
}

void CubeBlock::partialTransform(const Eigen::Affine3d &transformation) {
    drawTransformation = transformation * currentTransformation;
}

void CubeBlock::transform(const Eigen::Affine3d &transformation) {
    currentTransformation = transformation * currentTransformation;
    drawTransformation = currentTransformation;
}

void CubeBlock::reset() {
    currentTransformation = startTransformation;
    drawTransformation = startTransformation;
    currentPosition = startPosition;
}

void CubeBlock::draw() {
    if (geometry != 0) {
        glPushMatrix();
        glMultMatrixd(drawTransformation.data());
        geometry->draw(colorList, isSelected);
        glPopMatrix();
    }
}

void TranslatorBinding::draw() {
    glPushMatrix();
    glMultMatrixd(transformation.data());
    selector->draw();
    glPopMatrix();
}
