#pragma once

#include <vector>
#include "Objects.h"
#include "MeshObject.h"
#include <GL/gl.h>

using namespace std;

struct Cube {
    vector<CubeBlock*> blocks;
    vector<MeshObject*> meshObjects;
    vector<TranslatorGeometry*> geometries;
    vector<TranslatorOperation*> operations;
    vector<TranslatorBinding*> bindings;
    map<string, TranslatorBinding*> bindingMap;
    vector< vector<int> > bandages;

    int positionCount;
    int activeBinding, uintBits;
    GLint redBits, greenBits, blueBits;
    int animationStage, animationBinding;

    void partialOperate(int operationId, double t);
    void operate(int operationId);
    void operate();
    void prepareDraw();
    void deleteDisplayList();
    void draw();
    void drawSelection();
    void reset();
    void scramble(int step);
    void checkBindingValidity();
    bool setActiveBinding(int k);
    int setActiveBinding(string str);
    void colorEncode(int k, GLuint &red, GLuint &green, GLuint &blue);
    int colorDecode(GLuint *color);
    int prepareAnimation();
    int prepareAnimation(int index);
    void doAnimation(double t);
    void doneAnimation();
    void setSelection(GLuint *color);
};
